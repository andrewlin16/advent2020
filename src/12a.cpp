#include <array>
#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>

#include <util.h>

const std::array<std::pair<int, int>, 4> kDirections = {{
	{+1,  0},
	{ 0, -1},
	{-1,  0},
	{ 0, +1},
}};

int main() {
	const std::vector<std::string> input = util::ReadInput();

	int x = 0;
	int y = 0;
	int facing = 0;

	for (const std::string& line : input) {
		const char action = line[0];
		const int value = std::stoi(line.substr(1, line.size() - 1));

		switch (action) {
			case 'N':
				y -= value;
				break;
			case 'S':
				y += value;
				break;
			case 'E':
				x += value;
				break;
			case 'W':
				x -= value;
				break;
			case 'L':
				facing = (facing + value / 90) % 4;
				break;
			case 'R':
				facing = (facing + 3 * value / 90) % 4;
				break;
			case 'F':
				const auto [dx, dy] = kDirections[facing];
				x += dx * value;
				y += dy * value;
				break;
		}
	}

	const int result = std::abs(x) + std::abs(y);
	std::cout << result << std::endl;
	return 0;
}
