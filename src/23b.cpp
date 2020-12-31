#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

const int kMaxCup = 1000000;

struct Cup {
	const int label;
	const int index;
	int next;
};

Cup& NextCup(std::vector<Cup>& cups, const Cup& cup) {
	return cups[cup.next];
}

Cup& FindCup(std::vector<Cup>& cups, const int label, const int input_size) {
	if (label > input_size) {
		return cups[label - 1];
	}

	for (Cup& cup : cups) {
		if (cup.label == label) {
			return cup;
		}
	}

	throw std::runtime_error("couldn't find cup");
}

constexpr int DecrementLabel(const int label) {
	return (label + kMaxCup - 2) % kMaxCup + 1;
}

int MoveCups(std::vector<Cup>& cups, const int current_index,
		const int input_size) {
	Cup& current_cup = cups[current_index];

	Cup& next1 = NextCup(cups, current_cup);
	Cup& next2 = NextCup(cups, next1);
	Cup& next3 = NextCup(cups, next2);

	current_cup.next = next3.next;

	int destination_label = current_cup.label;
	do {
		destination_label = DecrementLabel(destination_label);
	} while (
		destination_label == next1.label || destination_label == next2.label ||
		destination_label == next3.label);

	Cup& destination_cup = FindCup(cups, destination_label, input_size);
	next3.next = destination_cup.next;
	destination_cup.next = next1.index;

	return current_cup.next;
}

int main() {
	std::string line;
	std::getline(std::cin, line);
	const int input_size = line.size();

	std::vector<Cup> cups;

	for (int i = 0; i < input_size; ++i) {
		const int label = line[i] - '0';
		const int next = i + 1;
		cups.emplace_back(label, i, next);
	}

	for (int i = input_size; i < kMaxCup; ++i) {
		const int label = i + 1;
		const int next = label % kMaxCup;
		cups.emplace_back(label, i, next);
	}

	int current_index = 0;
	for (int i = 0; i < 10000000; ++i) {
		current_index = MoveCups(cups, current_index, input_size);
	}

	Cup& cup = FindCup(cups, 1, input_size);
	Cup& next1 = NextCup(cups, cup);
	Cup& next2 = NextCup(cups, next1);

	const long result = static_cast<long>(next1.label) * next2.label;
	std::cout << result << std::endl;
	return 0;
}
