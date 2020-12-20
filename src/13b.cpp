#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <utility>

struct Bus {
	int id;
	int offset;
};

std::vector<Bus> ParseSchedule(const std::string& schedule) {
	std::vector<Bus> buses;

	size_t str_index = 0;
	int iterations = 0;
	bool eol = false;

	while (!eol) {
		size_t comma_index = schedule.find(',', str_index);
		if (comma_index == std::string::npos) {
			comma_index = schedule.size();
			eol = true;
		}

		const std::string token = schedule.substr(
			str_index, comma_index - str_index);
		if (token[0] != 'x') {
			const int id = std::stoi(token);
			int offset = (id - iterations) % id;
			if (offset < 0) {
				offset += id;
			}
			buses.emplace_back(id, offset);
		}

		str_index = comma_index + 1;
		++iterations;
	}

	return buses;
}

// See https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
long InverseMultiply(unsigned long r0, unsigned long r1) {
	long s0 = 1;
	long s1 = 0;

	while (r1 != 1) {
		const unsigned long q = r0 / r1;
		r0 = r0 % r1;
		s0 = s0 - q * s1;

		std::swap(r0, r1);
		std::swap(s0, s1);
	}

	return s1;
}

int main() {
	std::string schedule_line;

	std::cin >> schedule_line >> schedule_line;
	const std::vector<Bus> buses = ParseSchedule(schedule_line);

	// See https://en.wikipedia.org/wiki/Chinese_remainder_theorem
	const unsigned long product = std::transform_reduce(
		buses.begin(), buses.end(), 1L, std::multiplies<unsigned long>(),
		[](const Bus& bus) -> unsigned long {
			return bus.id;
		});

	unsigned long result = std::transform_reduce(
		buses.begin(), buses.end(), 0L,
		[product](const unsigned long a, const unsigned long b) ->
				unsigned long {
			return (a + b) % product;
		},
		[product](const Bus& bus) -> unsigned long {
			const unsigned long a = product / bus.id;
			long inv = InverseMultiply(a, bus.id);
			if (inv < 0) {
				inv += bus.id;
			}

			return (a * inv * bus.offset) % product;
		});

	std::cout << result << std::endl;
	return 0;
}
