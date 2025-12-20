#include "../../common_fast.h"

struct Context {

    using Range = Pair<Uint64, Uint64>;
    Vector<Range> ranges;
    Vector<Uint64> ingredients;

    bool isFresh(Uint64 id) {
        for (auto &range : ranges) {
            if (id >= range.first && id <= range.second) {
                return true;
            }
        }

        return false;
    }

    void parse(auto content) {
        auto ranges_v = content | rv::split_sv("\n\n") | rv::get0 | rv::split_sv('\n');
        auto list_v = content | rv::split_sv("\n\n") | rv::get1 | rv::split_sv('\n') | rv::to_ints;

        for (auto range_v : ranges_v) {
            Range r;
            r.first = range_v | rv::split_sv('-') | rv::get0 | rv::to_int;
            r.second = range_v | rv::split_sv('-') | rv::get1 | rv::to_int;
            ranges.push_back(r);
        }

        rs::copy(list_v, std::back_inserter(ingredients));
    }

    void part1() {
        auto result{0_int64};

        for (auto ingredient : ingredients) {
            if (isFresh(ingredient)) {
                result++;
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        Uint64 result{0};

        Vector<Range> new_ranges = ranges;

        bool altered = true;

        while (altered) {
            altered = false;

            for (auto &range : new_ranges) {

                for (auto &other_range : new_ranges) {

                    if (&range != &other_range) {
                        if (range.first >= other_range.first && range.first <= other_range.second) {
                            range.first = other_range.second + 1;
                            altered = true;
                            break;
                        } else {
                            if (range.first >= other_range.first && range.second <= other_range.second) {
                                new_ranges.erase(std::find(new_ranges.begin(), new_ranges.end(), range));
                                altered = true;
                                break;
                            }
                        }
                    }
                }

                if (altered)
                    break;
            }
        }

        for (auto &range : new_ranges) {
            result += range.second - range.first + 1;
        }

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    {
        Context context;
        context.parse(content);
        context.part1();
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
