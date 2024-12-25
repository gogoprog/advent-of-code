#include "../../common_fast.h"

struct Context {

    using Pins = Vector<int>;

    Vector<Pins> keys;
    Vector<Pins> locks;
    int width, height;

    void parse(auto content) {
        auto blocks = content | rv::split_sv("\n\n");

        for (auto block : blocks) {
            Grid grid;
            grid.parse(block | rv::split_sv('\n'));

            width = grid.width;
            height = grid.height;

            if (grid.get({0, 0}) == '#') {
                locks.push_back({});
                auto &lock = locks.back();

                for (int x = 0; x < grid.width; ++x) {
                    int i = 0;

                    while (grid.get({x, i}) == '#') {
                        i++;
                    }

                    lock.push_back(i - 1);
                }
            } else {
                keys.push_back({});
                auto &lock = keys.back();
                for (int x = 0; x < grid.width; ++x) {
                    int i = 0;

                    while (grid.get({x, grid.height - 1 - i}) == '#') {
                        i++;
                    }

                    lock.push_back(i - 1);
                }
            }
        }
    }

    void part1() {
        auto result{0_int64};

        Set<Pair<int, int>> matching;

        for (int l = 0; l < locks.size(); ++l) {
            const auto &lock = locks[l];

            for (int k = 0; k < keys.size(); ++k) {
                const auto &key = keys[k];

                bool good = true;

                for (int x = 0; x < width; x++) {
                    if ((lock[x] + key[x] >= height - 1)) {
                        good = false;
                        break;
                    }
                }

                if (good) {
                    matching.insert({k, l});
                }
            }
        }

        result = matching.size();

        log << "Part1: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    {
        Context context;
        context.parse(content);
        context.part1();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
