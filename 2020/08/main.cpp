#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto original_lines = getFileLines(filename);

    auto part1done = false;
    auto part2done = false;
    auto r = 0;

    auto cursor = 0;
    auto accumulator = 0;

    Map<int, int> lineExecutedMap;

    auto readValue = [](InputStringStream &iss) -> int {
        char op;
        int value;
        iss >> op >> value;

        if (op == '+') {
            return value;
        } else {
            return -value;
        }
    };

    while (cursor < original_lines.size()) {
        auto &line = original_lines[cursor];
        InputStringStream iss{line};
        String instr;

        iss >> instr;

        if (lineExecutedMap[cursor]) {
            log << "Part1: " << accumulator << endl;
            break;
        }

        lineExecutedMap[cursor]++;

        if (instr == "acc") {
            accumulator += readValue(iss);
            cursor++;
        } else if (instr == "jmp") {
            cursor += readValue(iss);
        } else if (instr == "nop") {
            cursor++;
        }
    }

    for (int l = 0; l < original_lines.size(); ++l) {

        auto mline = original_lines[l];

        if (mline.find("nop") != std::string::npos || mline.find("jmp") != std::string::npos) {
            auto lines = original_lines;

            if (mline.find("nop") != std::string::npos) {
                lines[l] = "jmp" + lines[l].substr(3);
            } else {
                lines[l] = "nop" + lines[l].substr(3);
            }

            auto cursor = 0;
            auto accumulator = 0;

            Map<int, int> lineExecutedMap;

            while (cursor < lines.size()) {
                auto &line = lines[cursor];
                InputStringStream iss{line};
                String instr;

                iss >> instr;

                lineExecutedMap[cursor]++;

                if (lineExecutedMap[cursor] > 1) {
                    break;
                }

                if (instr == "acc") {
                    accumulator += readValue(iss);
                    cursor++;
                } else if (instr == "jmp") {
                    cursor += readValue(iss);
                } else if (instr == "nop") {
                    cursor++;
                }
            }

            if (cursor >= lines.size()) {
                log << "Part2: " << accumulator << endl;
                break;
            }
        }
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
