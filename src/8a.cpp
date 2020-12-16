#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <util.h>

// Lazily using first letter of opcode to identify operation since each letter
// is distinct (Acc, Jmp, Nop).
struct Instruction {
	char op;
	int arg;
};

int main() {
	const std::vector<std::string> input = util::ReadInput();

	std::vector<Instruction> program;
	std::transform(input.begin(), input.end(), std::back_inserter(program),
		[](const std::string& line) -> Instruction {
			const char op = line[0];
			const int arg = std::stoi(line.substr(4, line.size() - 4));

			return {op, arg};
		});

	std::vector<bool> visited(input.size(), false);
	int pc = 0;
	int accumulator = 0;

	while (!visited[pc]) {
		const Instruction& instruction = program[pc];
		visited[pc] = true;

		const char op = instruction.op;
		switch (op) {
			case 'a':
				accumulator += instruction.arg;
				++pc;
				break;
			case 'j':
				pc += instruction.arg;
				break;
			case 'n':
				++pc;
				break;
			default:
				std::cerr << "Invalid instruction " << op << std::endl;
				return 1;
				break;
		}
	}

	std::cout << accumulator << std::endl;
	return 0;
}
