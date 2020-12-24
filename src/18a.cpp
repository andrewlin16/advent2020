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

void PushValueAndMaybeFlatten(std::vector<Node>& stack, long value) {
	if (stack.empty()) {
		// Nothing on stack to flatten, just push value.
		stack.emplace_back(value);
		return;
	}

	const Node& top = stack.back();
	if (top.type == kParenType) {
		// Cannot flatten past open paren, just push value.
		stack.emplace_back(value);
		return;
	}

	// Top node should be operator by this point. Remove operator node as part
	// of flattening.
	stack.pop_back();
	// Pop operand off of stack to flatten stack using operator.
	const Node& operand = stack.back();
	stack.pop_back();

	if (top.type == kPlusType) {
		stack.emplace_back(value + operand.value);
	} else if (top.type == kTimesType) {
		stack.emplace_back(value * operand.value);
	} else {
		// Type is number, which from looking at the input shouldn't happen but
		// let's support multi-digit numbers anyway...
		stack.emplace_back(top.value * 10 + value);
	}
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
				PushValueAndMaybeFlatten(stack, value);
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
				const long value = stack.back().value;
				stack.pop_back();	// Should be number.
				stack.pop_back();	// Should be open paren.
				PushValueAndMaybeFlatten(stack, value);
				break;
			}
			default: {
				break;
			}
		}
	}

	return stack.back().value;
}

int main() {
	const std::vector<std::string> input = util::ReadInput();
	const long result = std::transform_reduce(input.begin(), input.end(), 0L,
		std::plus<long>(), Evaluate);

	std::cout << result << std::endl;
	return 0;
}
