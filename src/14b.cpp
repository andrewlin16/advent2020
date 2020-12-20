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
	Mask() : set(0UL), clear(0x0f'ffff'ffffUL), floating() {}
	Mask(const std::string& str) :
			set(0UL), clear(0x0f'ffff'ffffUL), floating() {
		for (int i = 0; i < 36; ++i) {
			const unsigned long bit = 1UL << (35 - i);
			if (str[i] == '1') {
				set |= bit;
			} else if (str[i] == 'X') {
				clear ^= bit;
				floating.push_back(bit);
			}
		}
	}

	std::vector<unsigned long> GetAddresses(const unsigned long address) const {
		const size_t num_floats = floating.size();
		const size_t num_addresses = 1UL << num_floats;
		std::vector<unsigned long> addresses(num_addresses);

		const unsigned long base_address = (address & clear) | set;
		addresses[0] = base_address;

		for (size_t i = 0; i < num_floats; ++i) {
			const unsigned long bit = floating[i];
			const size_t limit = 1 << i;
			for (size_t k = 0; k < limit; ++k) {
				addresses[limit + k] = addresses[k] | bit;
			}
		}

		return addresses;
	}

private:
	unsigned long set;
	unsigned long clear;
	std::vector<unsigned long> floating;
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
			const unsigned long base_address = std::stoul(
				line.substr(4, endbracket_index - 4));

			const size_t value_index = line.find('=') + 2;
			const unsigned long value = std::stoul(
				line.substr(value_index, line.size() - value_index));

			const std::vector<unsigned long> masked_addresses =
				mask.GetAddresses(base_address);
			for (const unsigned long a : masked_addresses) {
				memory[a] = value;
			}
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
