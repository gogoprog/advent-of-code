#include "../../common.h"

void process(const String filename, const int preambleSize) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    Vector<ull> values;

    for (auto &line : lines) {
        values.push_back(std::stoll(line));
    }

    auto isValid = [&](const ull value, const int index) {
        for (int i = index - preambleSize; i < index; ++i) {
            if (i < 0) {
                continue;
            }

            auto p = values[i];

            for (int j = index - preambleSize; j < index; ++j) {
                if (j < 0) {
                    continue;
                }

                auto q = values[j];

                if (p + q == value) {
                    return true;
                }
            }
        }

        return false;
    };

    ull part1{0};

    for (int i = preambleSize; i < values.size(); ++i) {
        auto value = values[i];

        if (!isValid(value, i)) {
            part1 = value;
            log << "Part1 : " << value << endl;
            break;
        }
    }

    for (int i = 0; i < values.size(); ++i) {
        ull sum{0};

        for (int j = i; j < values.size(); ++j) {
            sum += values[j];

            if (sum == part1) {

                auto minmax = std::minmax_element(values.begin() + i, values.begin() + j);
                log << "Part2: " << *(minmax.first) + *(minmax.second) << endl;
                return;
            }
        }
    }
}

int main() {
    process("sample.txt", 5);
    process("input.txt", 25);

    return 0;
}
