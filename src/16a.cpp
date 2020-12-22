#include <algorithm>
#include <iostream>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

#include <util.h>

const std::regex kRangeRegex("(\\d+)-(\\d+)");

class Range {
public:
	Range(const int l, const int u): lower_bound(l), upper_bound(u) {}

	bool Check(const int value) const {
		return value >= lower_bound && value <= upper_bound;
	}
private:
	int lower_bound;
	int upper_bound;
};

const std::vector<Range> ParseField(const std::string& str) {
	std::vector<Range> ranges;
	std::sregex_iterator iter(str.begin(), str.end(), kRangeRegex);
	std::sregex_iterator iter_end;

	for (; iter != iter_end; ++iter) {
		const int lower_bound = std::stoi(iter->str(1));
		const int upper_bound = std::stoi(iter->str(2));
		ranges.emplace_back(lower_bound, upper_bound);
	}

	return ranges;
}

int main() {
	const std::vector<std::vector<std::string>> input = util::ReadInputGroups();

	const std::vector<std::string>& field_lines = input[0];
	std::vector<Range> ranges;

	std::for_each(field_lines.begin(), field_lines.end(),
		[&ranges](const std::string& line) {
			const std::vector<Range> line_ranges = ParseField(line);
			ranges.insert(ranges.end(), line_ranges.begin(), line_ranges.end());
		});

	const std::vector<std::string>& ticket_lines = input[2];

	int sum = 0;
	for (const std::string& line : ticket_lines | std::ranges::views::drop(1)) {
		const std::vector<int> line_fields = util::ParseIntList(line);
		for (const int value : line_fields) {
			const bool invalid = std::none_of(
				ranges.begin(), ranges.end(), [value](const Range& range) {
					return range.Check(value);
				});
			if (invalid) {
				sum += value;
			}
		}
	}

	std::cout << sum << std::endl;
	return 0;
}
