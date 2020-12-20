#include <algorithm>
#include <iostream>
#include <iterator>
#include <unordered_map>

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
	std::istream_iterator<int> input_iter(std::cin), input_end;
	std::unordered_map<int, History> history;

	int turn = 1;
	int last_number = 0;

	for (; input_iter != input_end; ++input_iter, ++turn) {
		last_number = *input_iter;
		UpdateHistory(history, last_number, turn);
	}

	for (; turn <= 30000000; ++turn) {
		last_number = UpdateHistory(history, last_number, turn);
	}

	std::cout << last_number << std::endl;
	return 0;
}
