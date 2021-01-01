#ifndef UTIL_H
#define UTIL_H

#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace util {

const std::vector<std::string> ReadInput();
const std::vector<std::vector<std::string>> ReadInputGroups();
void Tokenize(const std::string&, const char,
	const std::function<void(const std::string&, int)>&);
std::vector<int> ParseIntList(const std::string&);

// Based on https://stackoverflow.com/a/27216842, which seems to be based on
// Boost libraries.
template<typename T>
size_t HashCombine(const size_t seed, const T value) {
	return seed ^ (value + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

template<typename T1, typename T2>
struct PairHash {
	size_t operator()(const std::pair<T1, T2>& v) const {
		return HashCombine(std::hash<T1>{}(v.first), std::hash<T2>{}(v.second));
	}
};

}

#endif /* UTIL_H */
