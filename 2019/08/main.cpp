#include "../../common.h"

void decode(const String line, const int width, const int height) {
    auto layerCount = line.length() / (width * height);
    auto size = width * height;

    Map<int, int> zeroCounts;

    for (int l = 0; l < layerCount; ++l) {
        int count{0};

        for (int i = 0; i < size; ++i) {
            char c = line[l * size + i];

            if (c == '0') {
                ++count;
            }
        }

        zeroCounts[l] = count;
    }

    auto it =
        std::min_element(zeroCounts.begin(), zeroCounts.end(), [](auto &a, auto &b) { return a.second < b.second; });
    Map<char, int> charCounts;
    int l = it->first;

    for (int i = 0; i < size; ++i) {
        char c = line[l * size + i];

        if (c == '1' || c == '2') {
            charCounts[c]++;
        }
    }

    int r = charCounts['1'] * charCounts['2'];

    log << "part1: " << r << endl;

    {
        Map<int, int> pixels;
        for (int l = 0; l < layerCount; ++l) {
            for (int i = 0; i < size; ++i) {
                char c = line[l * size + i];

                if (l == 0 || pixels[i] == 2) {
                    int v = c - '0';
                    pixels[i] = v;
                }
            }
        }

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int p = y * width + x;

                int v = pixels[p];
                if (v == 1) {
                    log << "#";
                } else {
                    log << " ";
                }
            }
            log << endl;
        }
    }
}

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    decode(lines[0], 25, 6);
}

int main() {
    process("input.txt");
    return 0;
}
