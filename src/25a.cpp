#include <iostream>

constexpr int kSubjectNumber = 7;
constexpr int kModulo = 20201227;

constexpr int Advance(const int subject, const int n) {
	return static_cast<long>(n) * subject % kModulo;
}

int FindPow(const int n) {
	int mod = 1;
	int pow = 0;

	while (true) {
		mod = Advance(kSubjectNumber, mod);
		++pow;

		if (mod == n) {
			return pow;
		}
	}
}

int main() {
	int card_pubkey;
	int door_pubkey;
	std::cin >> card_pubkey >> door_pubkey;

	const int pow = FindPow(door_pubkey);
	int mod = 1;

	for (int i = 0; i < pow; ++i) {
		mod = Advance(card_pubkey, mod);
	}

	std::cout << mod << std::endl;
	return 0;
}
