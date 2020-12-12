#include <iostream>
#include <iterator>
#include <unordered_set>

int main() {
	std::istream_iterator<int> input_begin(std::cin), input_end;
	const std::unordered_set<int> entries(input_begin, input_end);

	for (const int i : entries) {
		const int complement = 2020 - i;
		if (entries.contains(complement)) {
			std::cout << i * complement << std::endl;
			return 0;
		}
	}

	std::cerr << "No matching entries found" << std::endl;
	return 1;
}
