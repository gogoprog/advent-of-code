#include "../../common.h"

struct Range {
    uint begin;
    uint end;
};

struct Context {
    uint max;
    Vector<Range> blockedRanges;

    void parse(String line) {
        InputStringStream iss{line};
        uint begin, end;
        char c;
        iss >> begin >> c >> end;

        blockedRanges.push_back({begin, end});
    }

    void run() {
        std::sort(blockedRanges.begin(), blockedRanges.end(), [](auto &a, auto &b) { return a.begin < b.begin; });
        bool changed{true};

        while (changed) {
            changed = false;
            for (int b{0}; b < blockedRanges.size() - 1; ++b) {
                auto &current = blockedRanges[b];
                auto &next = blockedRanges[b + 1];

                if (current.end > next.begin) {
                    next.begin = current.end;
                    changed = true;
                }
            }

            blockedRanges.erase(
                std::remove_if(blockedRanges.begin(), blockedRanges.end(), [](auto br) { return br.begin >= br.end; }),
                blockedRanges.end());
        }
        for (auto br : blockedRanges) {
            /* log << br.begin << " - " << br.end << endl; */
        }

        bool part1done{false};
        uint count{0};

        for (int i{0}; i <= max; ++i) {
            bool valid{true};
            for (auto &br : blockedRanges) {
                if (i >= br.begin && i <= br.end) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                if (!part1done) {
                    log << "part1: " << i << endl;
                    part1done = true;
                    break;
                }
            }
        }

        int i{0};

        for (auto &br : blockedRanges) {
            if (i < br.begin) {
                count += br.begin - i;
            }
            i = br.end + 1;
        }

        if (max > blockedRanges.back().end) {
            count += max - blockedRanges.back().end;
        }

        log << "part2: " << count << endl;
    }
};

void process(const String filename, const uint max) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx{max};

    for (auto &line : lines) {
        ctx.parse(line);
    }

    ctx.run();
}

int main() {
    process("sample.txt", 9);
    process("sample2.txt", 12);
    process("input.txt", 4294967295);
    return 0;
}
