#include <iostream>
#include <string>
#include <vector>

#include <util.h>

size_t ClampDec(const size_t i) {
	return i == 0 ? 0 : i - 1;
}

size_t ClampInc(const size_t i, const size_t size) {
	return i >= size - 1 ? size - 1 : i + 1;
}

int GetAdjacents(const std::vector<std::string>& grid, const size_t r,
		const size_t c, const size_t rows, const size_t cols) {
	int adjacents = 0;

	for (size_t y = ClampDec(r); y <= ClampInc(r, rows); ++y) {
		const std::string& row = grid[y];

		for (size_t x = ClampDec(c); x <= ClampInc(c, cols); ++x) {
			if (!(y == r && x == c) && row[x] == '#') {
				++adjacents;
			}
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
					const int adjacents = GetAdjacents(input_copy, y, x, rows,
						cols);
					if (adjacents == 0) {
						cell = '#';
						++occupied;
						mod = true;
					}
				} else if (cell == '#') {
					const int adjacents = GetAdjacents(input_copy, y, x, rows,
						cols);
					if (adjacents >= 4) {
						cell = 'L';
						--occupied;
						mod = true;
					}
				}
			}
		}
	}

	std::cout << occupied << std::endl;
	return 0;
}
