#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

const std::set<std::string> kRequiredFields = {
	"byr",
	"iyr",
	"eyr",
	"hgt",
	"hcl",
	"ecl",
	"pid",
};

bool IsValid(const std::set<std::string>& passport) {
	return std::includes(passport.begin(), passport.end(),
		kRequiredFields.begin(), kRequiredFields.end());
}

int main() {
	int num_valid = 0;
	std::string line;
	std::set<std::string> passport;

	while (std::getline(std::cin, line)) {
		if (line.empty()) {
			if (IsValid(passport)) {
				num_valid++;
			}
			passport.clear();
		} else {
			std::istringstream ss(line);
			std::for_each(
				std::istream_iterator<std::string>(ss),
				std::istream_iterator<std::string>(),
				[&passport](const std::string& s) {
					passport.insert(s.substr(0, 3));
				});
		}
	}

	if (IsValid(passport)) {
		num_valid++;
	}

	std::cout << num_valid << std::endl;
	return 0;
}
