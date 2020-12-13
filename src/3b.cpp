#include <array>
#include <iostream>
#include <string>
#include <vector>

#include <util.h>

struct Tracker {
	const int x_inc;
	const int per_y;
	const int width;

	int row;
	int col;
	int trees;

	Tracker(int _xi, int _py, int _w)
		: x_inc(_xi), per_y(_py), width(_w), row(0), col(0), trees(0) {}

	void Tick(const std::string& map_row) {
		if (row == 0) {
			if (map_row[col] == '#') {
				trees++;
			}
			col = (col + x_inc) % width;
		}

		row = (row + 1) % per_y;
	}
};

int main() {
	const std::vector<std::string> input = util::read_input();

	const int width = input[0].size();
	std::array<Tracker, 5> trackers = {
		Tracker(1, 1, width),
		Tracker(3, 1, width),
		Tracker(5, 1, width),
		Tracker(7, 1, width),
		Tracker(1, 2, width),
	};

	for (const std::string& row : input) {
		for (Tracker& tracker : trackers) {
			tracker.Tick(row);
		}
	}

	long result = 1;
	for (const Tracker& tracker : trackers) {
		result *= tracker.trees;
	}
	std::cout << result << std::endl;
	return 0;
}
