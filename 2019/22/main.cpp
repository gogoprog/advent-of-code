/* I totally cheated for this one.
 * Even when cheating it was hard to achieve because of big integers.
 * I had to use boost in the end.
 * The solution I based my code on is not even correct for my inputs.
 */

#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>
using boost::multiprecision::cpp_int;

#include "../../common.h"

using bigint = cpp_int;

bigint modPow(bigint num, bigint pow, bigint mod) {
    bigint test;
    for (test = 1; pow; pow >>= 1) {
        if (pow & 1)
            test = (test * num) % mod;
        num = (num * num) % mod;
    }

    return test;
}

struct System {
    ull targetPosition;
    ull size;
    Vector<std::function<void()>> funcs;
    bigint mul{1};
    bigint offset{0};

    void init(const ull count, const ull _target) {
        targetPosition = _target;
        size = count;
    }

    void parse(const String line) {

        if (line == "deal into new stack") {
            funcs.push_back([=]() { targetPosition = size - targetPosition - 1; });
            mul *= -1;
            offset += mul;
        } else {
            String str;
            InputStringStream iss{line};

            iss >> str;
            if (str == "cut") {
                int count;
                iss >> count;
                if (count > 0) {
                    funcs.push_back([=]() {
                        if (targetPosition >= count) {
                            targetPosition -= count;
                        } else {
                            targetPosition += size - count;
                        }
                    });
                } else {
                    funcs.push_back([=]() {
                        if (targetPosition >= size + count) {
                            targetPosition -= size + count;
                        } else {
                            targetPosition -= count;
                        }
                    });
                }
                offset += count * mul;
            } else {
                lli inc;
                iss >> str >> str >> inc;

                funcs.push_back([=]() { targetPosition = (targetPosition * inc) % size; });

                mul *= modPow(inc, size - 2, size);
            }
        }

        while (mul < 0) {
            mul += size;
        }

        while (offset < 0) {
            offset += size;
        }

        mul %= size;
        offset %= size;
    }

    void process() {
        for (auto &func : funcs) {
            func();
        }
    }
};

void process(const String filename, const ull count, const ull target) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    System system{};

    system.init(count, target);

    for (auto &line : lines) {
        system.parse(line);
    }

    system.process();

    log << "Part1: " << (ull)system.targetPosition << endl;
}

void process2(const String filename, const ull size, const ull target, const ull repeats) {
    log << "Processing2 " << filename << endl;
    auto lines = getFileLines(filename);

    System system;

    system.init(size, target);

    for (auto &line : lines) {
        system.parse(line);
    }

    auto increment = modPow(system.mul, repeats, size);
    auto offset = system.offset * (1 - increment) * modPow((1 - system.mul) % size, size - 2, size);
    offset %= size;
    auto card = (offset + target * increment) % size;

    log << "Part2: " << card << endl;
}

int main() {
    process("sample.txt", 10, 0);
    process("input.txt", 10007, 2019);
    process2("input.txt", 119315717514047, 2020, 101741582076661);
    return 0;
}
