#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include <util.h>

// Tuple of (count, color).
using BagRule = std::pair<int, std::string>;
// Map of color -> [(int, color)], where values are a list of tuples of
// (count, color) that the key color contains.
using BagMap = std::unordered_map<std::string, std::vector<BagRule>>;

const std::regex kBagFindRegex("(\\d+ )?(\\w+ \\w+) bag");

int CalculateTotalBags(const BagMap& map, const std::vector<BagRule>& bags) {
	if (bags.empty()) {
		return 0;
	}

	return std::transform_reduce(
		bags.begin(), bags.end(), 0, std::plus<int>(),
		[&map](const BagRule& rule) -> int {
			const int count = rule.first;
			const std::string& color = rule.second;
			return count * (CalculateTotalBags(map, map.at(color)) + 1);
		});
}

int main() {
	const std::vector<std::string> input = util::ReadInput();
	BagMap map;

	for (const std::string& line : input) {
		std::sregex_iterator bag_iter(line.begin(), line.end(), kBagFindRegex);
		std::sregex_iterator bag_iter_end;

		const std::string& subject = (*bag_iter++)[2];
		std::vector<BagRule> rules;

		for (; bag_iter != bag_iter_end; ++bag_iter) {
			const std::smatch& match = *bag_iter;
			const std::string& object = match[2];

			if (object != "no other") {
				const int count = std::stoi(match[1]);
				rules.emplace_back(count, object);
			}
		}

		map[subject] = std::move(rules);
	}

	const int result = CalculateTotalBags(map, {{1, "shiny gold"}}) - 1;
	std::cout << result << std::endl;
	return 0;
}
