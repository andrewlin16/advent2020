#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <util.h>

using Cell = std::tuple<int, int, int>;

std::vector<Cell> GetNeighbors(const Cell& cell) {
	const auto [x, y, z] = cell;
	std::vector<Cell> neighbors;
	neighbors.reserve(26);

	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			for (int dz = -1; dz <= 1; ++dz) {
				if (dx != 0 || dy != 0 || dz != 0) {
					neighbors.emplace_back(x + dx, y + dy, z + dz);
				}
			}
		}
	}

	return neighbors;
}

int main() {
	const std::vector<std::string> input = util::ReadInput();
	std::set<Cell> state;

	const size_t rows = input.size();
	for (size_t r = 0; r < rows; ++r) {
		const std::string& row = input[r];
		const size_t cols = row.size();
		for (size_t c = 0; c < cols; ++c) {
			if (row[c] == '#') {
				state.emplace(c, r, 0);
			}
		}
	}

	for (int i = 0; i < 6; ++i) {
		std::set<Cell> new_state(state);
		for (const Cell& cell : state) {
			const std::vector<Cell> neighbors = GetNeighbors(cell);
			new_state.insert(neighbors.begin(), neighbors.end());
		}

		auto iter = new_state.begin();
		while (iter != new_state.end()) {
			const Cell& cell = *iter;

			const std::vector<Cell> neighbors = GetNeighbors(cell);
			int num_neighbors = std::count_if(
				neighbors.begin(), neighbors.end(), [&state](const Cell& n) {
					return state.contains(n);
				});
			bool active = state.contains(cell);
			bool new_active = (active ?
				(num_neighbors == 2 || num_neighbors == 3) :
				num_neighbors == 3);

			if (!new_active) {
				iter = new_state.erase(iter);
			} else {
				++iter;
			}
		}

		state = std::move(new_state);
	}

	std::cout << state.size() << std::endl;
	return 0;
}