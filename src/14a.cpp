#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <util.h>

class Mask {
public:
	Mask() : basis(0UL), mask(0x000f'ffff'ffffUL) {}
	Mask(const std::string& str) :
		basis(ParseString(str, '1')), mask(ParseString(str, 'X')) {
	}

	unsigned long ApplyToValue(const unsigned long value) const {
		return basis | (value & mask);
	}

private:
	unsigned long basis;
	unsigned long mask;

	unsigned long ParseString(const std::string& str, const char c) const {
		unsigned long value = 0UL;

		for (int i = 0; i < 36; ++i) {
			if (str[i] == c) {
				value |= 1UL << (35 - i);
			}
		}

		return value;
	}
};

int main() {
	const std::vector<std::string> input = util::ReadInput();
	std::unordered_map<unsigned long, unsigned long> memory;
	Mask mask;

	for (const std::string& line : input) {
		if (line.starts_with("mask =")) {
			const std::string& mask_str = line.substr(7, 36);
			mask = Mask(mask_str);
		} else {
			const size_t endbracket_index = line.find(']');
			const unsigned long address = std::stoul(
				line.substr(4, endbracket_index - 4));

			const size_t value_index = line.find('=') + 2;
			const unsigned long value = std::stoul(
				line.substr(value_index, line.size() - value_index));
			const unsigned long masked_value = mask.ApplyToValue(value);
			memory[address] = masked_value;
		}
	}

	const unsigned long result = std::transform_reduce(
		memory.begin(), memory.end(), 0UL, std::plus<unsigned long>(),
		[](const std::pair<unsigned long, unsigned long>& entry) {
			return entry.second;
		}
	);
	std::cout << result << std::endl;
	return 0;
}
