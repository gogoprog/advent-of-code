#include "../../common.h"

inline int getMultiplier(const int i, const int p) {
    static const int patternLen = 4;
    static const Array<int, patternLen> basePattern = {0, 1, 0, -1};

    auto pos = ((p + 1) / (i + 1));

    return basePattern[pos % patternLen];
}

struct FFT {
    String temp;

    String run(const String line) {
        auto input = line;

        for (int phase = 0; phase < 100; ++phase) {
            temp.resize(0);

            for (int i = 0; i < input.length(); ++i) {
                int sum = 0;

                for (int j = 0; j < input.length(); ++j) {
                    char c = input[j];
                    int v = c - '0';
                    int m = getMultiplier(i, j);

                    sum += v * m;
                }

                sum = std::abs(sum) % 10;
                temp.push_back('0' + sum);
            }

            input = temp;
        }

        return input;
    }

    void part2(const String input, const int offset) {
        const auto len = input.length();
        const auto max = len - offset;
        Vector<int> buffer;
        buffer.resize(max);
        String result;

        for (int n = max - 8; n < max; ++n) {
            for (int n = 0; n < max; ++n) {
                buffer[n] = input[len - n - 1] - '0';
            }
            for (int phase = 0; phase < 100; ++phase) {
                int sum{0};
                for (int m = 0; m < n + 1; ++m) {
                    sum += buffer[m];
                    buffer[m] = std::abs(sum) % 10;
                }
            }
            result.insert(result.begin(), '0' + buffer[n]);
        }

        log << "part2: " << result << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    for (auto &line : lines) {
        {
            FFT fft;
            auto r = fft.run(line);
            log << "part1: " << r.substr(0, 8) << endl;
        }
        {
            FFT fft;
            auto offsetstr = line.substr(0, 7);
            auto offset = std::stoi(offsetstr);

            if (offset < line.length() * 10000) {
                String input;
                for (int i = 0; i < 10000; ++i) {
                    input += line;
                }
                fft.part2(input, offset);
            }
        }
    }
}

int main() {
    process("sample.txt");
    process("sample2.txt");
    process("sample3.txt");
    process("sample4.txt");
    process("sample5.txt");
    process("sample6.txt");
    process("input.txt");
    return 0;
}
