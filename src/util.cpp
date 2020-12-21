#include <functional>
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

void Tokenize(
		const std::string& str, const char token,
		const std::function<void(std::string, int)> visit_func) {
	size_t index = 0;
	int iterations = 0;
	bool eol = false;

	while (!eol) {
		size_t token_index = str.find(token, index);
		if (token_index == std::string::npos) {
			token_index = str.size();
			eol = true;
		}

		visit_func(str.substr(index, token_index - index), iterations);
		index = token_index + 1;
		++iterations;
	}
}

}
