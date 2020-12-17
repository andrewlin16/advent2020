#include <array>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <util.h>

const std::array<std::pair<int, int>, 8> kDirections = {{
	{-1, -1},
	{-1,  0},
	{-1, +1},
	{ 0, -1},
	{ 0, +1},
	{+1, -1},
	{+1,  0},
	{+1, +1},
}};

bool InBounds(const int y, const int x, const size_t rows, const size_t cols) {
	if (y < 0 || x < 0) {
		return false;
	}

	if ((size_t) y >= rows) {
		return false;
	}

	if ((size_t) x >= cols) {
		return false;
	}

	return true;
}

int GetAdjacents(const std::vector<std::string>& grid, const int r,
		const int c, const size_t rows, const size_t cols) {
	int adjacents = 0;

	for (const auto [dy, dx] : kDirections) {
		int y = r + dy;
		int x = c + dx;

		while (InBounds(y, x, rows, cols)) {
			const char cell = grid[y][x];
			if (cell == '#') {
				adjacents++;
				break;
			} else if (cell == 'L') {
				break;
			}
			y += dy;
			x += dx;
		}
	}

	return adjacents;
}

int main() {
	std::vector<std::string> input = util::ReadInput();
	const int rows = input.size();
	const int cols = input[0].size();

	int occupied = 0;
	bool mod = true;

	while (mod) {
		std::vector<std::string> input_copy = input;
		mod = false;

		for (int y = 0; y < rows; ++y) {
			std::string& row = input[y];

			for (int x = 0; x < cols; ++x) {
				char& cell = row[x];
				if (cell == 'L') {
					const int adjacents = GetAdjacents(
						input_copy, y, x, rows, cols);
					if (adjacents == 0) {
						cell = '#';
						occupied++;
						mod = true;
					}
				} else if (cell == '#') {
					const int adjacents = GetAdjacents(
						input_copy, y, x, rows, cols);
					if (adjacents >= 5) {
						cell = 'L';
						occupied--;
						mod = true;
					}
				}
			}
		}
	}

	std::cout << occupied << std::endl;
	return 0;
}
