#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

int PassToSeatId(const std::string& pass) {
	// Boarding passes can be trivially mapped to seat IDs by mapping:
	//   [FL] -> 0
	//   [BR] -> 1
	// and then interpreting the result as a binary number.
	return std::accumulate(pass.begin(), pass.end(), 0, [](int accum, char c) {
		int ret = accum << 1;
		if (c == 'B' || c == 'R') {
			ret |= 1;
		}
		return ret;
	});
}

int main() {
	std::istream_iterator<std::string> input_begin(std::cin), input_end;

	const std::vector<std::string> seat_ids(input_begin, input_end);
	const int max_seat_id = std::ranges::max(
		seat_ids | std::ranges::views::transform(PassToSeatId));
	std::cout << max_seat_id << std::endl;

	return 0;
}
