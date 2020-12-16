#include <iostream>
#include <iterator>
#include <unordered_set>
#include <vector>

const int kPreambleSize = 25;

int main() {
	std::istream_iterator<long> input_begin(std::cin), input_end;
	const std::vector<long> data(input_begin, input_end);

	std::unordered_multiset<long> sums;
	for (int i = 0; i < kPreambleSize - 1; ++i) {
		const long a = data[i];
		for (int j = i + 1; j < kPreambleSize; ++j) {
			const long b = data[j];
			sums.insert(a + b);
		}
	}

	for (auto iter = data.begin() + kPreambleSize; iter != data.end(); ++iter) {
		const long tail_num = *iter;
		if (!sums.contains(tail_num)) {
			std::cout << tail_num << std::endl;
			return 0;
		}

		const long head_num = *(iter - kPreambleSize);
		for (int i = 1; i < kPreambleSize; ++i) {
			const long history_num = *(iter - kPreambleSize + i);
			const long head_sum = head_num + history_num;
			const long tail_sum = tail_num + history_num;

			sums.extract(head_sum);
			sums.insert(tail_sum);
		}
	}

	std::cerr << "All values were valid" << std::endl;
	return 1;
}
