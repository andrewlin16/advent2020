#include <iostream>
#include <iterator>
#include <map>

int main() {
	std::istream_iterator<int> input_iter(std::cin), input_end;
	std::map<int, long> adapter_map;

	for (; input_iter != input_end; ++input_iter) {
		const int adapter = *input_iter;
		adapter_map[adapter] = 0;
	}

	adapter_map.rbegin()->second = 1;

	for (auto iter = std::next(adapter_map.rbegin());
			iter != adapter_map.rend(); ++iter) {
		const int joltage = iter->first;
		long arrangements = 0;

		for (int i = 1; i <= 3; ++i) {
			const int next_joltage = joltage + i;
			const auto iter = adapter_map.find(next_joltage);
			if (iter != adapter_map.end()) {
				arrangements += iter->second;
			}
		}

		iter->second = arrangements;
	}

	const long result = adapter_map[1] + adapter_map[2] + adapter_map[3];
	std::cout << result << std::endl;
	return 0;
}
