#include <bitset>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>

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
	std::istream_iterator<std::string> input_iter(std::cin), input_end;

	std::bitset<1024> seats;

	for (; input_iter != input_end; ++input_iter) {
		const int seat_id = PassToSeatId(*input_iter);
		seats.set(seat_id);
	}

	for (int i = 0x08; i <= 0x3f7; ++i) {
		if (!seats.test(i)) {
			std::cout << i << std::endl;
			return 0;
		}
	}

	std::cerr << "Seat not found" << std::endl;
	return 1;
}
