#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <util.h>

using Deck = std::deque<int>;

int stoi(const std::string& a) {
	return std::stoi(a);
}

void PlayRound(Deck& deck1, Deck& deck2) {
	const int top_deck1 = deck1.front();
	const int top_deck2 = deck2.front();

	deck1.erase(deck1.begin());
	deck2.erase(deck2.begin());

	if (top_deck1 > top_deck2) {
		deck1.insert(deck1.end(), {top_deck1, top_deck2});
	} else {
		deck2.insert(deck2.end(), {top_deck2, top_deck1});
	}
}

int main() {
	const std::vector<std::vector<std::string>> input = util::ReadInputGroups();

	Deck deck1;
	Deck deck2;

	std::transform(input[0].begin() + 1, input[0].end(),
		std::back_inserter(deck1), stoi);
	std::transform(input[1].begin() + 1, input[1].end(),
		std::back_inserter(deck2), stoi);

	while (!deck1.empty() && !deck2.empty()) {
		PlayRound(deck1, deck2);
	}

	const Deck& winning_deck = deck1.empty() ? deck2 : deck1;
	const size_t size = winning_deck.size();

	int result = 0;
	for (size_t i = 0; i < size; ++i) {
		result += (size - i) * winning_deck[i];
	}

	std::cout << result << std::endl;
	return 0;
}
