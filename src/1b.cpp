#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
	std::istream_iterator<int> input_begin(std::cin), input_end;
	const std::vector<int> entries(input_begin, input_end);

	for (auto iter_a = entries.begin(); iter_a != entries.end() - 2; ++iter_a) {
		const int a = *iter_a;
		for (auto iter_b = iter_a + 1; iter_b != entries.end() - 1; ++iter_b) {
			const int b = *iter_b;
			const int c = 2020 - a - b;
			if (std::find(iter_b + 1, entries.end(), c) != entries.end()) {
				std::cout << a * b * c << std::endl;
				return 0;
			}
		}
	}

	std::cerr << "No matching entries found" << std::endl;
	return 1;
}
