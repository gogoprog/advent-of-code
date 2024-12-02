#include "../../common_fast.h"

struct Context {

    void part1(auto lines) {
        auto result{0};

        for (auto line : lines) {

            auto numbers = line | rv::split_string_view(' ') | rv::filter_empty | rv::to_ints;

            Vector<int> vec;
            rs::copy(numbers, std::back_inserter(vec));

            char prevsign = 0;

            bool safe = true;

            for (int i = 0; i < vec.size() - 1; ++i) {

                auto a = vec[i];
                auto b = vec[i + 1];
                auto sign = 0;
                if (a < b) {
                    sign = '<';
                } else if (a > b) {
                    sign = '>';
                } else {
                    safe = false;
                    break;
                }

                if (!prevsign) {
                    prevsign = sign;

                } else {
                    if (sign != prevsign) {
                        safe = false;
                        break;
                    }
                }

                auto delta = std::abs(a - b);

                if (delta < 1 || delta > 3) {
                    safe = false;
                    break;
                }
            }

            if (safe) {
                result++;
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0};
        for (auto line : lines) {

            auto numbers = line | rv::split_string_view(' ') | rv::filter_empty | rv::to_ints;

            Vector<int> vec;
            rs::copy(numbers, std::back_inserter(vec));
            auto ovec = vec;


            int j = 0;

            bool safe = true;

            do {
                char prevsign = 0;
                safe = true;

                for (int i = 0; i < vec.size() - 1; ++i) {

                    auto a = vec[i];
                    auto b = vec[i + 1];
                    auto sign = 0;
                    if (a < b) {
                        sign = '<';
                    } else if (a > b) {
                        sign = '>';
                    } else {
                        safe = false;
                        break;
                    }

                    if (!prevsign) {
                        prevsign = sign;

                    } else {
                        if (sign != prevsign) {
                            safe = false;
                            break;
                        }
                    }

                    auto delta = std::abs(a - b);

                    if (delta < 1 || delta > 3) {
                        safe = false;
                        break;
                    }
                }

                if (safe) {
                    result++;
                    break;
                } else {
                    vec = ovec;
                    vec.erase(vec.begin() + j);
                    j++;
                }
            } while (!safe && j <= ovec.size());
        }

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = rs::split_string_view(getFileContent(filename), '\n');
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
