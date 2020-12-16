#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <unordered_set>
#include <vector>

const int kPreambleSize = 25;

long FindInvalidNumber(const std::vector<long>& data) {
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
			return tail_num;
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

	return -1;
}

int main() {
	std::istream_iterator<long> input_begin(std::cin), input_end;
	const std::vector<long> data(input_begin, input_end);

	const long target = FindInvalidNumber(data);
	if (target == -1) {
		std::cerr << "All values were valid" << std::endl;
		return 1;
	}

	const auto tail_end = data.end();
	const auto head_end = tail_end - 2;
	auto head_iter = data.begin();
	auto tail_iter = data.begin() + 2;
	long sum = *head_iter + *(head_iter + 1);

	for (; head_iter != head_end; ++head_iter) {
		// Grow sum towards target by moving tail forward.
		for (; sum < target && tail_iter != tail_end; ++tail_iter) {
			sum += *tail_iter;
		}

		if (sum == target) {
			break;
		}

		sum -= *head_iter;

		// Shrink sum towards target by moving tail backward.
		while (sum > target && tail_iter != head_iter + 3) {
			sum -= *(--tail_iter);
		}
	}

	auto minmax = std::minmax_element(head_iter, tail_iter);
	const long result = *minmax.first + *minmax.second;
	std::cout << result << std::endl;
	return 0;
}
