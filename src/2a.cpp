#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <util.h>

struct PolicyCase {
	int lower_bound;
	int upper_bound;
	char letter;
	std::string password;

	PolicyCase() {}

	PolicyCase(const std::string& p) {
		const int dash_index = p.find('-');
		const int space_index = p.find(' ');

		lower_bound = std::stoi(p.substr(0, dash_index));

		const int upper_start = dash_index + 1;
		const int upper_count = space_index - upper_start;
		upper_bound = std::stoi(p.substr(upper_start, upper_count));

		letter = p[space_index + 1];

		const int colon_index = p.find(':');
		const int pass_start = colon_index + 2;
		const int pass_count = p.size() - pass_start;
		password = p.substr(pass_start, pass_count);
	}

	bool IsValid() const {
		const int count = std::count(password.begin(), password.end(), letter);
		return count >= lower_bound && count <= upper_bound;
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
