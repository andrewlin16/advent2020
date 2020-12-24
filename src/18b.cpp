#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <util.h>

enum NodeType {
	kNumberType,
	kPlusType,
	kTimesType,
	kParenType,
};

struct Node {
	NodeType type;
	long value;

	Node(long v) : type(kNumberType), value(v) {}
	Node(NodeType t) : type(t) {}
};

void PushValueAndMaybeFlattenAdd(std::vector<Node>& stack, long value) {
	if (stack.empty()) {
		// Nothing on stack to flatten, just push value.
		stack.emplace_back(value);
		return;
	}

	const Node top = stack.back();
	if (top.type == kParenType || top.type == kTimesType) {
		// Cannot flatten past open paren or times, just push value.
		stack.emplace_back(value);
		return;
	}

	// Top node should be plus operator by this point. Remove operator node as
	// part of flattening.
	stack.pop_back();
	// Flatten new value into operand node on top of stack.
	Node& operand = stack.back();

	if (top.type == kPlusType) {
		operand.value += value;
	} else {
		std::cerr << "top node type was number for some reason" << std::endl;
	}
}

long FlattenMultiplies(std::vector<Node>& stack) {
	long result = 1;

	while (!stack.empty()) {
		const Node top = stack.back();
		stack.pop_back();

		if (top.type == kParenType) {
			break;
		} else if (top.type == kNumberType) {
			result *= top.value;
		} else if (top.type == kPlusType) {
			std::cerr << "unexpected plus type while flattening multiplies" <<
				std::endl;
		}
	}

	return result;
}

long Evaluate(const std::string& expr) {
	std::vector<Node> stack;

	for (const char c : expr) {
		switch (c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': {
				const long value = c - '0';
				PushValueAndMaybeFlattenAdd(stack, value);
				break;
			}
			case '+': {
				stack.emplace_back(kPlusType);
				break;
			}
			case '*': {
				stack.emplace_back(kTimesType);
				break;
			}
			case '(': {
				stack.emplace_back(kParenType);
				break;
			}
			case ')': {
				const long flatten_value = FlattenMultiplies(stack);
				PushValueAndMaybeFlattenAdd(stack, flatten_value);
				break;
			}
			default: {
				break;
			}
		}
	}

	return FlattenMultiplies(stack);
}

int main() {
	const std::vector<std::string> input = util::ReadInput();
	const long result = std::transform_reduce(input.begin(), input.end(), 0L,
		std::plus<long>(), Evaluate);

	std::cout << result << std::endl;
	return 0;
}
