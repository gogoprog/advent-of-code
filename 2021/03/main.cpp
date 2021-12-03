#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    {
        auto result1 = lines[0];
        auto result2 = lines[0];

        for (int c = 0; c < lines[0].size(); c++) {
            auto count = 0;

            for (auto &line : lines) {

                if (line[c] == '1') {
                    ++count;
                }
            }

            if (count > lines.size() / 2) {
                result1[c] = '1';
                result2[c] = '0';

            } else {
                result1[c] = '0';
                result2[c] = '1';
            }
        }

        auto gamma_rate = std::stoull(result1, 0, 2);
        log << gamma_rate << endl;
        auto epsilon_rate = std::stoull(result2, 0, 2);
        log << epsilon_rate << endl;

        log << "Part1: " << gamma_rate * epsilon_rate << endl;
    }

    {

        String result1;
        String result2;
        String criteria;
        String criteria2;

        for (int c = 0; c < lines[0].size(); c++) {
            auto count = 0;
            auto count2 = 0;
            auto total = 0.0f;
            auto total2 = 0.0f;
            String current;
            String current2;

            for (auto &line : lines) {
                if (line.rfind(criteria, 0) == 0) {
                    ++total;
                    current = line;
                    if (line[c] == '1') {
                        ++count;
                    }
                }
                if (line.rfind(criteria2, 0) == 0) {
                    ++total2;
                    current2 = line;
                    if (line[c] == '1') {
                        ++count2;
                    }
                }
            }

            if (total) {
                if (total == 1) {
                    result1 = current;
                } else if (count >= total / 2) {
                    criteria += '1';
                    result1 += '1';
                } else if (count < total / 2) {
                    result1 += '0';
                    criteria += '0';
                }
            }

            if (total2) {
                if (total2 == 1) {
                    result2 = current2;
                } else if (count2 >= total2 / 2) {
                    criteria2 += '0';
                    result2 += '0';
                } else if (count2 < total2 / 2) {
                    criteria2 += '1';
                    result2 += '1';
                }
            }
        }

        auto oxygen_rate = std::stoull(result1, 0, 2);
        log << oxygen_rate << endl;
        auto co_rate = std::stoull(result2, 0, 2);
        log << co_rate << endl;

        log << "Part2: " << oxygen_rate * co_rate << endl;
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
