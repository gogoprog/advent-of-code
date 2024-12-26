#include "../../common_fast.h"

auto find_symmetry = [](auto lines) {
    auto width = lines[0].size();
    auto height = lines.size();

    for (int x = 1; x < width; ++x) {
        int i = 0;
        bool good = true;

        while (good && x - i - 1 >= 0 && x + i < width) {

            for (int y = 0; y < height; ++y) {
                auto a = lines[y][x - i - 1];
                auto b = lines[y][x + i];

                if (a != b) {
                    good = false;
                    break;
                }
            }

            ++i;
        }

        if (good) {
            return x;
        }
    }

    for (int y = 1; y < height; ++y) {
        int i = 0;
        bool good = true;

        while (good && y - i - 1 >= 0 && y + i < height) {

            for (int x = 0; x < width; ++x) {
                auto a = lines[y - i - 1][x];
                auto b = lines[y + i][x];

                if (a != b) {
                    good = false;
                    break;
                }
            }

            ++i;
        }

        if (good) {
            return 100 * y;
        }
    }

    /* throw "oops"; */
    assert(false);

    return 0;
};

auto find_symmetry2 = [](auto lines) {
    auto width = lines[0].size();
    auto height = lines.size();

    for (int x = 1; x < width; ++x) {
        int i = 0;

        int diffs = 0;

        while (x - i - 1 >= 0 && x + i < width) {

            for (int y = 0; y < height; ++y) {
                auto a = lines[y][x - i - 1];
                auto b = lines[y][x + i];

                if (a != b) {
                    diffs++;
                }
            }

            ++i;
        }

        if (diffs == 1) {
            return x;
        }
    }

    for (int y = 1; y < height; ++y) {
        int i = 0;
        int diffs = 0;

        while (y - i - 1 >= 0 && y + i < height) {

            for (int x = 0; x < width; ++x) {
                auto a = lines[y - i - 1][x];
                auto b = lines[y + i][x];

                if (a != b) {
                    diffs++;
                }
            }

            ++i;
        }

        if (diffs == 1) {
            return y * 100;
        }
    }

    /* throw "oops"; */
    assert(false);

    return 0;
};

struct Context {

    void part1(auto content) {
        auto result{0};

        auto parts = content | rv::split_sv("\n\n");

        for (auto part : parts) {

            auto lines_view = part | rv::split_sv('\n');

            StringViews lines;
            rs::copy(lines_view, std::back_inserter(lines));

            auto r = find_symmetry(lines);
            result += r;
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto content) {
        auto result{0};

        auto parts = content | rv::split_sv("\n\n");

        for (auto part : parts) {

            auto lines_view = part | rv::split_sv('\n');

            StringViews lines;
            rs::copy(lines_view, std::back_inserter(lines));

            auto r = find_symmetry2(lines);
            result += r;
        }

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileContent(filename);
    {
        Context context;
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
