#include "../../common.h"

struct Fish {
    int timer;
    ull count;
};

struct Context {
    Vector<Fish> fishes;

    void update() {
        auto len = fishes.size();
        ull new_count = 0;

        for (int i = 0; i < len; i++) {
            auto &fish = fishes[i];

            fish.timer--;

            if (fish.timer < 0) {
                fish.timer = 6;
                new_count += fish.count;
            }
        }

        if (new_count) {
            add(8, new_count);
        }
    }

    void add(const int value, const ull count = 1) {
        for (auto &fish : fishes) {
            if (fish.timer == value) {
                fish.count += count;
                return;
            }
        }

        fishes.push_back({value, count});
    }

    ull total() const {
        ull r = 0;
        for (auto &fish : fishes) {
            r += fish.count;
        }

        return r;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto values = splitString<int>(lines[0], ',');
    Context context;

    for (auto &value : values) {
        context.add(value);
    }

    {
        for (int i = 0; i < 80; ++i) {
            context.update();
        }

        log << "Part1: " << context.total() << endl;
    }

    {
        for (int i = 0; i < 256 - 80; ++i) {
            context.update();
        }

        log << "Part2: " << context.total() << endl;
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
