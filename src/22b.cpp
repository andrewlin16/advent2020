#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_set>
#include <vector>

#include <util.h>

using Deck = std::string;
using HistoryEntry = std::pair<std::string, std::string>;
using History = std::unordered_set<
	HistoryEntry, util::PairHash<std::string, std::string>>;

char stoi(const std::string& a) {
	return std::stoi(a);
}

bool PlayGame(Deck&, Deck&);

bool P1Won(const Deck& deck1, const Deck& deck2) {
	const size_t top_deck1 = deck1.front();
	const size_t top_deck2 = deck2.front();

	if (top_deck1 > deck1.size() - 1 || top_deck2 > deck2.size() - 1) {
		return top_deck1 > top_deck2;
	}

	Deck new_deck1(deck1.begin() + 1, deck1.begin() + 1 + top_deck1);
	Deck new_deck2(deck2.begin() + 1, deck2.begin() + 1 + top_deck2);
	return PlayGame(new_deck1, new_deck2);
}

bool PlayGame(Deck& deck1, Deck& deck2) {
	History history;

	while (!deck1.empty() && !deck2.empty()) {
		HistoryEntry entry = {deck1, deck2};
		if (history.contains(entry)) {
			return true;
		}

		const char top_deck1 = deck1.front();
		const char top_deck2 = deck2.front();
		const bool p1_winner = P1Won(deck1, deck2);

		history.insert(std::move(entry));
		deck1.erase(deck1.begin());
		deck2.erase(deck2.begin());

		if (p1_winner) {
			deck1.insert(deck1.end(), {top_deck1, top_deck2});
		} else {
			deck2.insert(deck2.end(), {top_deck2, top_deck1});
		}
	}

	return deck2.empty();
}

int main() {
	const std::vector<std::vector<std::string>> input = util::ReadInputGroups();

	Deck deck1;
	Deck deck2;

	std::transform(input[0].begin() + 1, input[0].end(),
		std::back_inserter(deck1), stoi);
	std::transform(input[1].begin() + 1, input[1].end(),
		std::back_inserter(deck2), stoi);

	const bool p1_winner = PlayGame(deck1, deck2);
	const Deck& winning_deck = p1_winner ? deck1 : deck2;
	const size_t size = winning_deck.size();

	int result = 0;
	for (size_t i = 0; i < size; ++i) {
		result += (size - i) * winning_deck[i];
	}

	std::cout << result << std::endl;
	return 0;
}
