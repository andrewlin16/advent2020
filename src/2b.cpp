#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <util.h>

struct PolicyCase {
	int pos_a;
	int pos_b;
	char letter;
	std::string password;

	PolicyCase() {}

	PolicyCase(const std::string& p) {
		const int dash_index = p.find('-');
		const int space_index = p.find(' ');

		pos_a = std::stoi(p.substr(0, dash_index));

		const int upper_start = dash_index + 1;
		const int upper_count = space_index - upper_start;
		pos_b = std::stoi(p.substr(upper_start, upper_count));

		letter = p[space_index + 1];

		const int colon_index = p.find(':');
		const int pass_start = colon_index + 2;
		const int pass_count = p.size() - pass_start;
		password = p.substr(pass_start, pass_count);
	}

	bool IsValid() const {
		return password[pos_a - 1] == letter ^ password[pos_b - 1] == letter;
	}
};

int main() {
	const std::vector<std::string> entries = util::read_input();

	const int count = std::count_if(entries.begin(), entries.end(),
		[](const std::string& entry) {
			return PolicyCase(entry).IsValid();
		});
	std::cout << count << std::endl;

	return 0;
}
