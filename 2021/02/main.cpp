#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    {
        auto r = 0;

        auto depth = 0;
        auto horizontal = 0;

        for (auto &line : lines) {
            String word;
            int value;
            InputStringStream iss{line};
            iss >> word;
            iss >> value;

            if (word == "forward") {
                horizontal += value;
            } else if (word == "down") {
                depth += value;
            } else if (word == "up") {
                depth -= value;
            }
        }

        r = depth * horizontal;
        log << "Part1: " << r << endl;
    }

    {
        auto r = 0;

        auto depth = 0;
        auto horizontal = 0;
        auto aim = 0;

        for (auto &line : lines) {
            String word;
            int value;
            InputStringStream iss{line};
            iss >> word;
            iss >> value;

            if (word == "forward") {
                horizontal += value;
                depth += aim * value;
            } else if (word == "down") {
                aim += value;
            } else if (word == "up") {
                aim -= value;
            }
        }

        r = depth * horizontal;
        log << "Part2: " << r << endl;
    }
}

int main() {
    process("input.txt");
    return 0;
}
