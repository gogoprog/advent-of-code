#include "../../common.h"

struct Bus {
    ull id;
    int index;

    int compute(const int target) const {
        int r{0};

        while (r < target) {
            r += id;
        }

        return r;
    }

    bool isValid(const ull t) const {
        return ((t + index) % id) == 0;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    int timestamp = std::stoi(lines[0]);
    Vector<String> ids = splitString(lines[1], ',');
    Vector<Bus> busses;

    int i{0};
    for (auto &id : ids) {
        if (id != "x") {
            busses.push_back({std::stoull(id), i});
        }
        ++i;
    }

    auto best = std::min_element(busses.begin(), busses.end(),
                                 [&](auto &a, auto &b) { return a.compute(timestamp) < b.compute(timestamp); });

    log << "Part1: " << best->id * (best->compute(timestamp) - timestamp) << endl;

    {
        auto total = busses.size();

        ull i{0};
        ull lastValid{1};
        ull jump{busses[0].id};
        Map<int, ull> repeats;

        while (true) {
            int valid{0};

            for (auto &bus : busses) {
                if (bus.isValid(i)) {
                    ++valid;
                } else {
                    break;
                }
            }

            if (valid == total) {
                log << "Part2: " << i << endl;
                break;
            }

            if (valid > lastValid) {
                if (repeats[valid] == 0) {
                    repeats[valid] = i;
                } else {
                    if (valid > lastValid) {
                        jump = i - repeats[valid];
                        lastValid = valid;
                    }
                }
            }

            i += jump;
        }
    }
}

int main() {
    process("sample.txt");
    process("sample6.txt");
    process("input.txt");
    return 0;
}
