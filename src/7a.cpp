#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <util.h>

const std::regex kBagFindRegex("(\\w+ \\w+) bag");

int main() {
	const std::vector<std::string> input = util::ReadInput();
	// Map of color -> [color], where values are colors that directly contain
	// the key color.
	std::unordered_map<std::string, std::vector<std::string>> map;

	for (const std::string& line : input) {
		std::sregex_iterator bag_iter(line.begin(), line.end(), kBagFindRegex);
		std::sregex_iterator bag_iter_end;

		const std::string& subject = (*bag_iter++)[1];

		for (; bag_iter != bag_iter_end; ++bag_iter) {
			const std::string& object = (*bag_iter)[1];
			if (object != "no other") {
				map[object].push_back(subject);
			}
		}
	}

	std::unordered_set<std::string> visited_colors;
	std::vector<std::string> visit_list(map["shiny gold"]);

	while (!visit_list.empty()) {
		const std::string color = visit_list.back();
		visit_list.pop_back();

		if (!visited_colors.contains(color)) {
			const std::vector<std::string>& next_visits = map[color];

			visited_colors.insert(std::move(color));
			visit_list.insert(
				visit_list.end(), next_visits.begin(), next_visits.end());
		}
	}

	std::cout << visited_colors.size() << std::endl;
	return 0;
}
