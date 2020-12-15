#include <iostream>
#include <string>
#include <vector>

namespace util {

const std::vector<std::string> ReadInput() {
	std::vector<std::string> input;
	std::string line;

	while (std::getline(std::cin, line)) {
		input.push_back(std::move(line));
	}

	return input;
}

const std::vector<std::vector<std::string>> ReadInputGroups() {
	std::vector<std::vector<std::string>> input;
	std::vector<std::string> group;
	std::string line;

	while (std::getline(std::cin, line)) {
		if (line.empty()) {
			input.push_back(std::move(group));
			group.clear();
		} else {
			group.push_back(std::move(line));
		}
	}

	input.push_back(std::move(group));
	return input;
}

}
