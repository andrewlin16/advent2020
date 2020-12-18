#include <cstdlib>
#include <iostream>
#include <string>

#include <util.h>

int main() {
	const std::vector<std::string> input = util::ReadInput();

	int x = 0;
	int y = 0;
	int waypoint_x = 10;
	int waypoint_y = 1;

	for (const std::string& line : input) {
		const char action = line[0];
		const int value = std::stoi(line.substr(1, line.size() - 1));
		const int times = (value / 90) % 4;

		switch (action) {
			case 'N':
				waypoint_y += value;
				break;
			case 'S':
				waypoint_y -= value;
				break;
			case 'E':
				waypoint_x += value;
				break;
			case 'W':
				waypoint_x -= value;
				break;
			case 'L':
				for (int k = 0; k < times; ++k) {
					std::swap(waypoint_x, waypoint_y);
					waypoint_x = -waypoint_x;
				}
				break;
			case 'R':
				for (int k = 0; k < times; ++k) {
					std::swap(waypoint_x, waypoint_y);
					waypoint_y = -waypoint_y;
				}
				break;
			case 'F':
				x += waypoint_x * value;
				y += waypoint_y * value;
				break;
		}
	}

	const int result = std::abs(x) + std::abs(y);
	std::cout << result << std::endl;
	return 0;
}
