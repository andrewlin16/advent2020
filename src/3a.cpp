#include <iostream>
#include <string>
#include <vector>

#include <util.h>

int main() {
	const std::vector<std::string> input = util::read_input();

	const int width = input[0].size();
	int col = 0;
	int trees = 0;
	for (const std::string& row : input) {
		if (row[col] == '#') {
			trees++;
		}
		col = (col + 3) % width;
	}

	std::cout << trees << std::endl;
	return 0;
}
