#include <iostream>
#include <string>
#include <vector>

namespace util {

const std::vector<std::string> ReadInput() {
	std::vector<std::string> input;
	std::string line;

	while (std::getline(std::cin, line)) {
		input.emplace_back(std::move(line));
	}

	return input;
}

}
