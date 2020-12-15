#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include <util.h>

int main() {
	const std::vector<std::vector<std::string>> input = util::ReadInputGroups();
	int sum = 0;

	for (const std::vector<std::string>& group : input) {
		std::unordered_set<char> answers;

		for (const std::string& line : group) {
			answers.insert(line.begin(), line.end());
		}

		sum += answers.size();
	}

	std::cout << sum << std::endl;
	return 0;
}
