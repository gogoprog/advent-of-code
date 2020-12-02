#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Vector<int> numbers;

    for (const auto &line : lines) {
        numbers.push_back(std::stoi(line));
    }

    for (const auto &a : numbers) {
        for (const auto &b : numbers) {
            if (&a != &b) {
                if (a + b == 2020) {
                    log << a * b << endl;
                    break;
                }
            }
        }
    }
    for (const auto &a : numbers) {
        for (const auto &b : numbers) {
            for (const auto &c : numbers) {
                if (&a != &b and &a != &c) {
                    if (a + b + c == 2020) {
                        log << a * b * c << endl;
                        break;
                    }
                }
            }
        }
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
