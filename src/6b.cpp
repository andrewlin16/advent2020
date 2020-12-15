#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
#include <set>
#include <string>
#include <vector>

#include <util.h>

int main() {
	const std::vector<std::vector<std::string>> input = util::ReadInputGroups();
	int sum = 0;

	for (const std::vector<std::string>& group : input) {
		const std::string& initial_state = group[0];
		std::set<char> answers(initial_state.begin(), initial_state.end());

		for (const std::string& line : group | std::ranges::views::drop(1)) {
			std::set<char> line_answers(line.begin(), line.end());
			std::set<char> intersect;

			std::set_intersection(
				answers.begin(), answers.end(),
				line_answers.begin(), line_answers.end(),
				std::inserter(intersect, intersect.end()));
			answers = std::move(intersect);
		}

		sum += answers.size();
	}

	std::cout << sum << std::endl;
	return 0;
}
