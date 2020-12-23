#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
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

class Field {
public:
	Field(const std::string& str) {
		name = str.substr(0, str.find(':'));

		std::sregex_iterator iter(str.begin(), str.end(), kRangeRegex);
		std::sregex_iterator iter_end;

		for (; iter != iter_end; ++iter) {
			const int lower_bound = std::stoi(iter->str(1));
			const int upper_bound = std::stoi(iter->str(2));
			ranges.emplace_back(lower_bound, upper_bound);
		}
	}

	bool Check(int value) const {
		return std::any_of(ranges.begin(), ranges.end(),
			[value](const Range& range) {
				return range.Check(value);
			});
	}

	bool IsDepartureField() const {
		return name.starts_with("departure");
	}

private:
	std::string name;
	std::vector<Range> ranges;
};

std::vector<Field> ParseFieldsSection(const std::vector<std::string>& lines) {
	std::vector<Field> fields;

	std::transform(lines.begin(), lines.end(), std::back_inserter(fields),
		[](const std::string& line) {
			return Field(line);
		});

	return fields;
}

std::vector<int> FindIndexMapping(const std::vector<Field>& fields,
		const std::vector<std::string>& lines) {
	const size_t num_fields = fields.size();

	// Start with each ticket index potentially representing all fields.
	std::vector<int> index_mappings(num_fields);
	std::iota(index_mappings.begin(), index_mappings.end(), 0);
	std::vector<std::vector<int>> field_mappings(num_fields, index_mappings);

	// Figure out which ticket indexes represent which fields by going through
	// each nearby ticket and removing potential index->field mappings which
	// would result in an invalid ticket.
	for (auto line_iter = lines.begin() + 1; line_iter != lines.end();
			++line_iter) {
		const std::string& line = *line_iter;
		const std::vector<int> line_fields = util::ParseIntList(line);

		// This ticket line is valid if all fields in the ticket can be mapped
		// to some field.
		const bool line_valid = std::all_of(
			line_fields.begin(), line_fields.end(),
			[&fields](const int value) {
				return std::any_of(fields.begin(), fields.end(),
					[value](const Field& field) {
						return field.Check(value);
					});
			});

		// If the ticket is not valid, discard this ticket.
		if (!line_valid) {
			continue;
		}

		// For each ticket index in this nearby ticket, remove any field->index
		// mappings for which the value in the ticket index would be invalid.
		for (size_t i = 0; i < num_fields; ++i) {
			const int field_value = line_fields[i];
			std::vector<int>& mapping = field_mappings[i];
			auto iter = mapping.begin();
			while (iter != mapping.end()) {
				const int field_index = *iter;
				const Field& field = fields[field_index];

				if (!field.Check(field_value)) {
					iter = mapping.erase(iter);
				} else {
					++iter;
				}
			}
		}
	}

	// Determine which indexes map to which fields via process of elimination.
	std::vector<bool> singles(num_fields, false);
	bool all_single = false;
	while (!all_single) {
		all_single = true;
		for (size_t i = 0; i < num_fields; ++i) {
			// Skip indexes that have already been resolved into a single field
			// mapping.
			if (singles[i]) {
				continue;
			}

			const std::vector<int>& field_mapping = field_mappings[i];
			if (field_mapping.size() == 1) {
				singles[i] = true;

				// This index has only 1 potential field mapping, so this index
				// must map to that field.
				const int mapping = field_mapping[0];
				index_mappings[i] = mapping;

				// Also, none of the other indexes can now map to that field, so
				// remove this mapping from all other potential field mappings.
				for (size_t j = 0; j < num_fields; ++j) {
					if (singles[j]) {
						continue;
					}
					std::vector<int>& m = field_mappings[j];
					const auto iter = std::find(m.begin(), m.end(), mapping);
					if (iter != m.end()) {
						m.erase(iter);
					}
				}
			} else {
				// There is an index that has not been resolved that still has
				// greater than 1 potential field mapping, so more iterations
				// are needed until all indexes have been mapped to a field.
				all_single = false;
			}
		}
	}

	return index_mappings;
}

int main() {
	const std::vector<std::vector<std::string>> input = util::ReadInputGroups();

	const std::vector<Field> fields = ParseFieldsSection(input[0]);
	const std::vector<int> index_mapping = FindIndexMapping(fields, input[2]);

	const std::vector<int> ticket = util::ParseIntList(input[1][1]);
	const unsigned long answer = std::transform_reduce(
		ticket.begin(), ticket.end(), index_mapping.begin(), 1UL,
		std::multiplies<>(),
		[&fields](const int value, const int field_index) -> unsigned long {
			const Field& field = fields[field_index];
			return field.IsDepartureField() ? value : 1UL;
		});

	std::cout << answer << std::endl;
	return 0;
}
