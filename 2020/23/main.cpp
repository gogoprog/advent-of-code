#include "../../common.h"

struct Cup {
    int value;
    Cup *next;
};

struct System {
    Cup *current{nullptr};
    Cup *lastAdded;
    Vector<Cup> cups;
    int max{0};

    System() {
        cups.resize(1000001);
    }

    void add(const int value) {

        auto &cup = cups[value];
        cup.value = value;

        if (current == nullptr) {
            current = &cup;
        } else {
            lastAdded->next = &cup;
            cup.next = current;
        }

        lastAdded = &cup;

        max = std::max(max, value);
    }

    Cup &get(const int value) {
        return cups[value];
    }

    void process() {
        Cup *pick;

        pick = current->next;

        current->next = pick->next->next->next;

        int dest = current->value;
        Cup *destCup;

        do {
            dest--;
            if (dest <= 0) {
                dest = max;
            }
            destCup = &cups[dest];
        } while (destCup == pick || destCup == pick->next || destCup == pick->next->next);

        auto tmp = destCup->next;
        destCup->next = pick;
        pick->next->next->next = tmp;

        current = current->next;
    }

    void print() {
        log << "Part1: ";
        auto cup = cups[1].next;

        do {
            log << cup->value;

            cup = cup->next;
        } while (cup->value != 1);

        log << endl;
    }

    void print2() {
        auto cup = & cups[1];

        ull a = cup->next->value;
        ull b = cup->next->next->value;

        log << "Part2: " << a * b << endl;
    }
};

void process(const String line, const int moves) {
    log << "Processing " << line << endl;

    System system{};

    for (auto c : line) {
        system.add(c - '0');
    }

    for (int i{0}; i < moves; i++) {
        system.process();
    }

    system.print();
}

void process2(const String line, const int moves) {
    log << "Processing " << line << endl;

    System system{};

    for (auto c : line) {
        system.add(c - '0');
    }

    for (ull i{10}; i <= 1000000; ++i) {
        system.add(i);
    }

    for (int i{0}; i < moves; i++) {
        system.process();
    }

    system.print2();
}

int main() {
    process("389125467", 10);
    process("389125467", 100);
    process("952316487", 100);
    process2("389125467", 10000000);
    process2("952316487", 10000000);
    return 0;
}
