#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include <util.h>

using Coordinate = std::pair<int, int>;
using CoordinateSet = std::unordered_set<Coordinate, util::PairHash<int, int>>;
using Neighbors = std::array<Coordinate, 6>;

void FlipCoordinate(CoordinateSet& set, const Coordinate c) {
	const auto iter = set.find(c);
	if (iter == set.end()) {
		set.insert(c);
	} else {
		set.erase(iter);
	}
}

Neighbors GetNeighbors(const Coordinate c) {
	const auto [x, y] = c;
	return {{
		{x - 1, y + 1},	// nw
		{x, y + 1},	// ne
		{x - 1, y},	// w
		{x + 1, y},	// e
		{x, y - 1},	// sw
		{x + 1, y - 1},	// se
	}};
}

int main() {
	const std::vector<std::string> input = util::ReadInput();
	CoordinateSet state;

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

		FlipCoordinate(state, coord);
	}

	for (int i = 0; i < 100; ++i) {
		CoordinateSet new_state(state);
		for (const Coordinate& c : state) {
			const Neighbors neighbors = GetNeighbors(c);
			new_state.insert(neighbors.begin(), neighbors.end());
		}

		auto iter = new_state.begin();
		while (iter != new_state.end()) {
			const Coordinate& coord = *iter;

			const Neighbors neighbors = GetNeighbors(coord);
			int num_neighbors = std::count_if(
				neighbors.begin(), neighbors.end(),
				[&state](const Coordinate& n) {
					return state.contains(n);
				});
			bool black = state.contains(coord);
			bool new_black = (black ?
				(num_neighbors == 1 || num_neighbors == 2) :
				num_neighbors == 2);

			if (!new_black) {
				iter = new_state.erase(iter);
			} else {
				++iter;
			}
		}

		state = std::move(new_state);
	}

	const size_t result = state.size();
	std::cout << result << std::endl;
	return 0;
}
