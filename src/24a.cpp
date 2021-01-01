#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include <util.h>

using Coordinate = std::pair<int, int>;
using CoordinateSet = std::unordered_set<Coordinate, util::PairHash<int, int>>;

void FlipCoordinate(CoordinateSet& set, const Coordinate c) {
	const auto iter = set.find(c);
	if (iter == set.end()) {
		set.insert(c);
	} else {
		set.erase(iter);
	}
}

int main() {
	const std::vector<std::string> input = util::ReadInput();
	CoordinateSet flips;

	for (const std::string& line : input) {
		char last_move = 0;
		Coordinate coord;

		for (const char c : line) {
			switch (c) {
				case 'n':
					coord.second += 1;
					break;
				case 's':
					coord.second -= 1;
					break;
				case 'e':
					coord.first += (last_move == 'n') ? 0 : 1;
					break;
				case 'w':
					coord.first -= (last_move == 's') ? 0 : 1;
					break;
				default:
					// unrecognized direction
					break;
			}
			last_move = c;
		}

		FlipCoordinate(flips, coord);
	}

	const size_t result = flips.size();
	std::cout << result << std::endl;
	return 0;
}
