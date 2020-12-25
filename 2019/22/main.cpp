#include "../../common.h"

struct System {
    ull targetPosition;
    ull target;
    ull size;

    void init(const ull count, const ull _target) {
        target = _target;
        targetPosition = target;
        size = count;
    }

    void process(const String line) {

        if (line == "deal into new stack") {
            targetPosition = size - targetPosition - 1;
        } else {
            String str;
            InputStringStream iss{line};

            iss >> str;
            if (str == "cut") {
                int count;
                iss >> count;
                if (count > 0) {
                    if (targetPosition > count) {
                        targetPosition -= count;
                    } else {
                        targetPosition += size - count;
                    }
                } else {
                    count = -count;

                    if (targetPosition > size - count) {
                        targetPosition -= size - count;
                    } else {
                        targetPosition += count;
                    }
                }
            } else {
                int inc;
                iss >> str >> str >> inc;

                targetPosition = (targetPosition * inc) % size;
            }
        }
    }

    void show() {
        log << "Pos: " << targetPosition << endl;
    }
};

void process(const String filename, const ull count, const ull target, const ull repeat = 1) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    System system;

    system.init(count, target);


    Map<ull, ull> history;

    for (ull i{0}; i < repeat; ++i) {

        for (auto &line : lines) {
            system.process(line);
        }

        log << system.targetPosition << endl;

    }

    system.show();
}

int main() {
    process("sample.txt", 10, 0);
    process("input.txt", 10007, 2019);
    process("input.txt", 119315717514047, 2020, 101741582076661);
    return 0;
}
