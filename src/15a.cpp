#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
	std::istream_iterator<int> input_begin(std::cin), input_end;
	std::vector<int> numbers(input_begin, input_end);

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
