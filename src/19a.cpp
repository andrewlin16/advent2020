#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <regex>
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

std::string GenerateRegex(const RuleMap& rule_map, const int rule_num) {
	const Rule& rule = rule_map.at(rule_num);

	if (std::holds_alternative<char>(rule)) {
		return std::string(1, std::get<char>(rule));
	}

	const MultiRule& multi_rule = std::get<MultiRule>(rule);
	const std::string inner_regex = std::transform_reduce(
		multi_rule.begin(), multi_rule.end(), std::string(),
		[](const std::string& lhs, const std::string& rhs) -> std::string {
			if (lhs.empty()) {
				return rhs;
			}
			return lhs + "|" + rhs;
		}, [&rule_map](const RuleSequence& seq) -> std::string {
			return std::transform_reduce(
				seq.begin(), seq.end(), std::string(),
				std::plus<std::string>(), [&rule_map](const int seq_num) {
					return GenerateRegex(rule_map, seq_num);
				});
		});

	if (multi_rule.size() == 1) {
		return inner_regex;
	} else {
		return "(" + inner_regex + ")";
	}
}

int main() {
	const std::vector<std::vector<std::string>> input = util::ReadInputGroups();

	const RuleMap rule_map = ParseRules(input[0]);
	const std::string regex_str = GenerateRegex(rule_map, 0);
	const std::regex rule0_regex(regex_str);

	const std::vector<std::string>& messages = input[1];
	const int result = std::count_if(messages.begin(), messages.end(),
		[&rule0_regex](const std::string& msg) {
			return std::regex_match(msg, rule0_regex);
		});

	std::cout << result << std::endl;
	return 0;
}
