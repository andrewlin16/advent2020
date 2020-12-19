#include <iostream>
#include <iterator>
#include <set>

int main() {
	std::istream_iterator<int> input_begin(std::cin), input_end;
	const std::set<int> adapters(input_begin, input_end);

	int joltage = 0;
	int count_1 = 0;
	int count_3 = 1;

	for (auto iter = adapters.begin(); iter != adapters.end(); ++iter) {
		const int value = *iter;
		const int diff = value - joltage;

		if (diff == 1) {
			++count_1;
		} else if (diff == 3) {
			++count_3;
		}

		joltage = value;
	}

	std::cout << (count_1 * count_3) << std::endl;
	return 0;
}
