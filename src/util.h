#ifndef UTIL_H
#define UTIL_H

#include <functional>
#include <string>
#include <vector>

namespace util {

const std::vector<std::string> ReadInput();
const std::vector<std::vector<std::string>> ReadInputGroups();
void Tokenize(const std::string&, const char,
	const std::function<void(const std::string&, int)>&);
std::vector<int> ParseIntList(const std::string&);

}

#endif /* UTIL_H */
