#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <string>
#include <vector>

#include <util.h>

// Lazily using first letter of opcode to identify operation since each letter
// is distinct (Acc, Jmp, Nop).
struct Instruction {
	char op;
	int arg;
};

struct State {
	size_t pc;
	int a;
};

State CalculateState(const Instruction& inst, const State state, bool flip) {
	const size_t pc = state.pc;
	const int a = state.a;

	switch (inst.op) {
		case 'a':
			return State(pc + 1, a + inst.arg);
			break;
		case 'j':
			return flip ? State(pc + 1, a) : State(pc + inst.arg, a);
			break;
		case 'n':
		default:
			return flip ? State(pc + inst.arg, a) : State(pc + 1, a);
			break;
	}
}

char str(bool a) {
	return a ? '1' : '0';
}

std::string str(std::vector<bool> a) {
	std::string result;
	for (const bool b : a) {
		result.push_back(str(b));
	}
	return result;
}

std::optional<int> ExecuteProgram(
		const std::vector<Instruction>& program, const State state,
		std::vector<bool> visited, bool flipped) {
	const size_t pc = state.pc;
	if (pc == program.size()) {
		return state.a;
	}
	if (visited[pc]) {
		return {};
	}

	const Instruction& inst = program[pc];
	visited[pc] = true;

	if (!flipped && inst.op != 'a') {
		const State new_state = CalculateState(program[pc], state, true);
		const std::optional<int> result = ExecuteProgram(
			program, new_state, visited, true);
		if (result) {
			return result;
		}
	}

	const State new_state = CalculateState(program[pc], state, false);
	return ExecuteProgram(program, new_state, visited, flipped);
}

int main() {
	const std::vector<std::string> input = util::ReadInput();

	std::vector<Instruction> program;
	std::transform(input.begin(), input.end(), std::back_inserter(program),
		[](const std::string& line) -> Instruction {
			const char op = line[0];
			const int arg = std::stoi(line.substr(4, line.size() - 4));

			return {op, arg};
		});

	std::optional<int> result = ExecuteProgram(
		program, {0, 0}, std::vector<bool>(input.size(), false), false);

	if (!result) {
		std::cerr << "Program did not properly terminate" << std::endl;
		return 1;
	} else {
		std::cout << *result << std::endl;
		return 0;
	}
}
