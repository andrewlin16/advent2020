#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

struct Cup {
	const char label;
	const char index;
	char next;

	char GetPrintLabel() const {
		return label + '0';
	}
};

Cup& NextCup(std::vector<Cup>& cups, const Cup& cup) {
	return cups[cup.next];
}

Cup& FindCup(std::vector<Cup>& cups, const char label) {
	for (Cup& cup : cups) {
		if (cup.label == label) {
			return cup;
		}
	}
	throw std::runtime_error("couldn't find cup");
}

constexpr char DecrementLabel(const char max_cup, const char label) {
	return (label + max_cup - 2) % max_cup + 1;
}

char MoveCups(std::vector<Cup>& cups, const char current_index) {
	const char max_cup = cups.size();
	Cup& current_cup = cups[current_index];

	Cup& next1 = NextCup(cups, current_cup);
	Cup& next2 = NextCup(cups, next1);
	Cup& next3 = NextCup(cups, next2);

	current_cup.next = next3.next;

	char destination_label = current_cup.label;
	do {
		destination_label = DecrementLabel(max_cup, destination_label);
	} while (
		destination_label == next1.label || destination_label == next2.label ||
		destination_label == next3.label);

	Cup& destination_cup = FindCup(cups, destination_label);
	next3.next = destination_cup.next;
	destination_cup.next = next1.index;

	return current_cup.next;
}

int main() {
	std::string line;
	std::getline(std::cin, line);

	std::vector<Cup> cups;
	const char max_cup = line.size();

	for (int i = 0; i < max_cup; ++i) {
		const char label = line[i] - '0';
		const char next = (i + 1) % max_cup;
		cups.emplace_back(label, i, next);
	}

	char current_index = 0;
	for (int i = 0; i < 100; ++i) {
		current_index = MoveCups(cups, current_index);
	}

	Cup& cup1 = FindCup(cups, 1);
	const char stop_index = cup1.index;
	char index = cup1.next;
	while (index != stop_index) {
		Cup& cup = cups[index];
		std::cout << cup.GetPrintLabel();
		index = cup.next;
	}
	std::cout << std::endl;
	return 0;
}
