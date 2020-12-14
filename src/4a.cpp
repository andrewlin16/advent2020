#include <algorithm>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <util.h>

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
	const std::vector<std::vector<std::string>> input = util::ReadInputGroups();
	int num_valid = 0;

	for (const std::vector<std::string>& group : input) {
		std::set<std::string> passport;

		for (const std::string& line : group) {
			std::istringstream ss(line);
			std::for_each(
				std::istream_iterator<std::string>(ss),
				std::istream_iterator<std::string>(),
				[&passport](const std::string& s) {
					passport.insert(s.substr(0, 3));
				});
		}

		if (IsValid(passport)) {
			num_valid++;
		}
	}

	std::cout << num_valid << std::endl;
	return 0;
}
