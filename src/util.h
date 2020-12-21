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

template<typename T>
std::vector<T> ParseVectorString(
		const std::string& str, const char token,
		const std::function<T(const std::string&)>& parse_func) {
	std::vector<T> result;

	Tokenize(str, token, [&result, &parse_func](const std::string& str, int i) {
		result.push_back(parse_func(str));
	});

	return result;
}

}

#endif /* UTIL_H */
