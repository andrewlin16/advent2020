#include <iostream>
#include <string>
#include <unordered_map>

#include <util.h>

class History {
public:
	History(): last_turn(-1), last_last_turn(-1) {}
	History(int t): last_turn(t), last_last_turn(-1) {}

	int GetResponse() const {
		if (last_last_turn == -1) {
			return 0;
		}
		return last_turn - last_last_turn;
	}

	void Update(int turn) {
		last_last_turn = last_turn;
		last_turn = turn;
	}

private:
	int last_turn;
	int last_last_turn;
};

int UpdateHistory(std::unordered_map<int, History>& history, const int num,
		const int turn) {
	const auto iter = history.find(num);
	if (iter == history.end()) {
		history.emplace(num, History(turn));
		return 0;
	} else {
		const int response = iter->second.GetResponse();
		history[response].Update(turn);
		return response;
	}
}

int main() {
	std::string input;
	std::getline(std::cin, input);
	const std::vector<int> numbers = util::ParseVectorString<int>(
		input, ',', [](const std::string& str) {
			return std::stoi(str);
		});

	std::unordered_map<int, History> history;
	for (size_t i = 0; i < numbers.size(); ++i) {
		UpdateHistory(history, numbers[i], i + 1);
	}

	int last_number = numbers.back();
	for (int turn = numbers.size() + 1; turn <= 30000000; ++turn) {
		last_number = UpdateHistory(history, last_number, turn);
	}

	std::cout << last_number << std::endl;
	return 0;
}
