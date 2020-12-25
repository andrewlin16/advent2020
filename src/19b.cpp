#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <util.h>

// Sequence of rule numbers, like `1 2` from rule 0 in the first example.
using RuleSequence = std::vector<int>;
// List of rule sequences, like `1 3 | 3 1` from rule 2 in the first example.
// Single rule sequences like rule 0 in the first example are just "MultiRules"
// containing one rule sequence.
using MultiRule = std::vector<RuleSequence>;
// Rule is either a single character or a multi-rule (which can contain a single
// rule sequence).
using Rule = std::variant<char, MultiRule>;
// RuleMap is just a mapping of rule number to Rule.
using RuleMap = std::unordered_map<int, Rule>;

const MultiRule kRule8 = {{42}, {42, 8}};
const MultiRule kRule11 = {{42, 31}, {42, 11, 31}};

RuleMap ParseRules(const std::vector<std::string>& lines) {
	RuleMap rule_map;

	for (const std::string& line : lines) {
		const int colon_index = line.find(':');
		const int rule_num = std::stoi(line.substr(0, colon_index));

		const int rule_def_index = colon_index + 2;
		const std::string rule_def = line.substr(rule_def_index, line.size() -
			rule_def_index);

		if (rule_def[0] == '"') {
			rule_map.emplace(rule_num, rule_def[1]);
		} else if (rule_num == 8) {
			rule_map.emplace(rule_num, kRule8);
		} else if (rule_num == 11) {
			rule_map.emplace(rule_num, kRule11);
		} else {
			MultiRule rule;
			util::Tokenize(rule_def, '|',
				[&rule](const std::string& rule_seq_def, int i) {
					RuleSequence seq;
					util::Tokenize(rule_seq_def, ' ',
						[&seq](const std::string& tok, int j) {
							if (!tok.empty()) {
								seq.push_back(std::stoi(tok));
							}
						});
					rule.push_back(seq);
			});
			rule_map.emplace(rule_num, rule);
		}
	}

	return rule_map;
}

// Returns:
// - true if rule expansion definitely matches input string
//   - rule expansion is all terminal rules and all terminals match input string
// - false if rule expansion definitely does not match input string
//   - either rule expansion is longer than input (since each rule must expand
//     into at least one character),
//   - or there is a terminal that doesn't match the input
// - int representing index in rule expansion where a rule needs to be expanded
//   if result is inconclusive (requires further expansion)
//   - rule expansion contains non-terminal rule that needs to be expanded
std::variant<bool, size_t> RuleExpansionMatches(const RuleMap& rule_map,
		const std::string& str, const std::vector<int> expansion) {
	const size_t expansion_size = expansion.size();
	const size_t str_size = str.size();

	// Assuming each rule expands into at least one character, if there are more
	// rules in the expansion than characters, then we can trivially conclude
	// that the rule expansion cannot match the input.
	if (expansion_size > str_size) {
		return false;
	}

	for (size_t i = 0; i < expansion.size(); ++i) {
		const int rule_num = expansion[i];
		const Rule& rule = rule_map.at(rule_num);

		if (std::holds_alternative<MultiRule>(rule)) {
			// Expansion contains non-terminal rule, return rule num that needs
			// to be expanded.
			return i;
		}

		const char rule_char = std::get<char>(rule);
		const char str_char = str[i];
		if (rule_char != str_char) {
			// Expansion contains non-matching terminal rule, so this expansion
			// cannot match the input string.
			return false;
		}
	}

	// All rules in the expansion were terminal (otherwise the loop above would
	// have returned inconclusive already), so the expansion must match the size
	// of the input for the match to be true (i.e. expansion does not match if
	// it was all terminal yet shorter than input).
	return expansion_size == str_size;
}

bool Matches(const RuleMap& rule_map, const std::string& str) {
	// Input must match rule 0 through expansion.
	std::vector<std::vector<int>> search = {{0}};

	while (!search.empty()) {
		std::vector<int> expansion = std::move(search.back());
		search.pop_back();

		// Check if rule expansion matches input string or requires further
		// expansion.
		const std::variant<bool, size_t> match_result = RuleExpansionMatches(
			rule_map, str, expansion);

		if (std::holds_alternative<bool>(match_result)) {
			// The expansion definitely does or does not match the input string.
			// If it matches, then return true, otherwise prune the expansion
			// from the search space.
			if (std::get<bool>(match_result)) {
				return true;
			}
		} else {
			// The rule expansion requires further expansion, so add possible
			// rule expansions to the search space by replacing the non-terminal
			// rule in the expansion with its possible expansions.
			const size_t index = std::get<size_t>(match_result);
			auto iter = expansion.begin() + index;
			const int rule_num = *iter;
			expansion.erase(iter);

			const MultiRule& rule = std::get<MultiRule>(rule_map.at(rule_num));
			for (const std::vector<int>& seq : rule) {
				std::vector<int> new_expansion = expansion;
				new_expansion.insert(
					new_expansion.begin() + index, seq.begin(), seq.end());
				search.emplace_back(std::move(new_expansion));
			}
		}
	}

	// Exhausted all possible rule expansions, so the input must not match.
	return false;
}

int main() {
	const std::vector<std::vector<std::string>> input = util::ReadInputGroups();

	const RuleMap rule_map = ParseRules(input[0]);
	const std::vector<std::string>& messages = input[1];
	const int result = std::count_if(messages.begin(), messages.end(),
		[&rule_map](const std::string& msg) {
			return Matches(rule_map, msg);
		});

	std::cout << result << std::endl;
	return 0;
}
