#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> ParseSchedule(const std::string& schedule) {
	std::vector<int> bus_ids;

	size_t index = 0;
	bool eol = false;
	while (!eol) {
		size_t comma_index = schedule.find(',', index);
		if (comma_index == std::string::npos) {
			comma_index = schedule.size();
			eol = true;
		}

		const std::string token = schedule.substr(index, comma_index - index);
		if (token[0] != 'x') {
			const int id = std::stoi(token);
			bus_ids.push_back(id);
		}

		index = comma_index + 1;
	}

	return bus_ids;
}

int WaitTime(const int timestamp, const int bus_id) {
	return bus_id - (timestamp % bus_id);
}

int main() {
	int timestamp;
	std::string schedule_line;

	std::cin >> timestamp >> schedule_line;
	const std::vector<int> schedule = ParseSchedule(schedule_line);

	const auto iter = std::min_element(schedule.begin(), schedule.end(),
		[timestamp](const int a, const int b) {
			return WaitTime(timestamp, a) < WaitTime(timestamp, b);
		});

	const int bus_id = *iter;
	const int wait = WaitTime(timestamp, bus_id);

	const int result = bus_id * wait;
	std::cout << result << std::endl;
	return 0;
}
