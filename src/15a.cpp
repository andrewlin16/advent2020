#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <util.h>

int main() {
	std::string input;
	std::getline(std::cin, input);
	std::vector<int> numbers = util::ParseIntList(input);

	while (numbers.size() < 2020) {
		const int last_spoken = numbers.back();
		const auto begin = numbers.rbegin();
		const auto end = numbers.rend();

		const auto iter = std::find(begin + 1, end, last_spoken);
		numbers.push_back(iter == end ? 0 : std::distance(begin, iter));
	}

	std::cout << numbers.back() << std::endl;
	return 0;
}
