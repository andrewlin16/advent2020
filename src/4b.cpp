#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

typedef std::function<bool(const std::string&)> Validator;

Validator year_validator(int lower, int upper) {
	return [lower, upper](const std::string& s ) {
		const int year = std::stoi(s);
		return year >= lower && year <= upper;
	};
}

const std::unordered_set<std::string> kValidEyeColors = {
	"amb", "blu", "brn", "gry", "grn", "hzl", "oth"
};

const std::unordered_map<std::string, Validator>
	kValidators = {
		{"byr", year_validator(1920, 2020)},
		{"iyr", year_validator(2010, 2020)},
		{"eyr", year_validator(2020, 2030)},
		{"hgt", [](const std::string& s) {
			if (s.size() <= 2) {
				return false;
			}

			const int value = std::stoi(s.substr(0, s.size() - 2));
			const std::string unit = s.substr(s.size() - 2, 2);

			if (unit == "cm") {
				return value >= 150 && value <= 193;
			} else if (unit == "in") {
				return value >= 59 && value <= 76;
			} else {
				return false;
			}
		}},
		{"hcl", [](const std::string& s) {
			if (s.size() != 7) {
				return false;
			}

			if (s[0] != '#') {
				return false;
			}

			return std::all_of(s.begin() + 1, s.end(), [](char c) {
				return std::isxdigit(c) && !std::isupper(c);
			});
		}},
		// alternatively:
		//   std::bind(
		//     &decltype(kValidEyeColors)::contains, &kValidEyeColors,
		//     std::placeholders::_1)
		// but like, why?
		{"ecl", [](const std::string& s) {
			return kValidEyeColors.contains(s);
		}},
		{"pid", [](const std::string& s) {
			if (s.size() != 9) {
				return false;
			}

			return std::all_of(s.begin(), s.end(), [](char c) {
				return std::isdigit(c);
			});
		}},
	};

int main() {
	int num_valid = 0;
	int passport_num_valid = 0;
	std::string line;

	while (std::getline(std::cin, line)) {
		if (line.empty()) {
			if (passport_num_valid == kValidators.size()) {
				num_valid++;
			}
			passport_num_valid = 0;
		} else {
			std::istringstream ss(line);
			std::for_each(
				std::istream_iterator<std::string>(ss),
				std::istream_iterator<std::string>(),
				[&passport_num_valid](const std::string& s) {
					const std::string field = s.substr(0, 3);
					const auto validator = kValidators.find(field);

					if (validator != kValidators.end()) {
						const Validator& validator_func = validator->second;
						const std::string value = s.substr(4, s.size() - 4);
						if (validator_func(value)) {
							passport_num_valid++;
						}
					}
				});
		}
	}

	if (passport_num_valid == kValidators.size()) {
		num_valid++;
	}

	std::cout << num_valid << std::endl;
	return 0;
}
