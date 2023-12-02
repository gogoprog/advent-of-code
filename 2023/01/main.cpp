#include "../../common.h"

struct Context {
    int computeP1(StringView lines) {
        auto computeLineValue = [&](auto range) {
            auto line = getStringView(range);
            auto linelen = line.length();

            int minval, maxval;

            for (int c = 0; c < linelen; ++c) {
                auto ch = *(line.begin() + c);
                if (std::isdigit(ch)) {
                    minval = ch - '0';
                    break;
                }
            }

            for (int c = linelen - 1; c >= 0; --c) {
                auto ch = *(line.begin() + c);
                if (std::isdigit(ch)) {
                    maxval = ch - '0';
                    break;
                }
            }

            auto v = minval * 10 + maxval;

            return v;
        };

        return rs::fold_left(rs::split_view(lines, '\n') | rv::transform(computeLineValue), 0, std::plus<int>());
    }

    int computeP2(StringView lines, const Strings &spellings) {
        auto len = spellings.size();

        /*
        auto computeLineValue = [&](auto range) {
            auto line = StringView(&*range.begin(), rs::distance(range));

            if (line.length() > 1) {
                auto view1 = rv::zip(rv::iota(0), spellings) | rv::transform([&](const auto &pair) {
                                 return std::pair{pair.first, line.find(pair.second)};
                             }) |
                             rv::filter([&](auto pair) { return pair.second != String::npos; });

                auto view2 = rv::zip(rv::iota(0), spellings) | rv::transform([&](const auto &pair) {
                                 return std::pair{pair.first, line.rfind(pair.second)};
                             }) |
                             rv::filter([&](auto pair) { return pair.second != String::npos; });

                auto min = rs::min(view1, [](auto a, auto b) { return a.second < b.second; });

                auto max = rs::max(view2, [](auto a, auto b) { return a.second < b.second; });

                return ((min.first % 9) + 1) * 10 + (max.first % 9) + 1;
            }
            return 0;
        };
        */

        auto computeLineValue3 = [&](auto range) {
            auto line = getStringView(range);
            auto linelen = line.length();

            int minval = 0, maxval = 0;

            for (int c = 0; c < linelen; ++c) {
                auto ch = *(line.begin() + c);
                if (std::isdigit(ch)) {
                    minval = ch - '0';
                    break;
                } else {
                    for (int i = 0; i < len; ++i) {
                        auto &spelling = *(spellings.begin() + i);
                        if (line.substr(c, spelling.length()) == spelling) {
                            minval = i + 1;
                            break;
                        }
                    }

                    if (minval)
                        break;
                }
            }

            for (int c = linelen - 1; c >= 0; --c) {
                auto ch = *(line.begin() + c);
                if (std::isdigit(ch)) {
                    maxval = ch - '0';
                    break;
                } else {
                    for (int i = 0; i < len; ++i) {
                        auto &spelling = *(spellings.begin() + i);
                        auto slen = spelling.length();
                        if (c >= slen && line.substr(c - slen + 1, slen) == spelling) {
                            maxval = i + 1;
                            break;
                        }
                    }
                    if (maxval)
                        break;
                }
            }

            auto v = minval * 10 + maxval;

            /* log << line << " " << v << "\n"; */

            return v;
        };

        return rs::fold_left(rs::split_view(lines, '\n') | rv::transform(computeLineValue3), 0, std::plus<int>());
    }

    void part1(StringView lines) {
        auto result = computeP1(lines);

        log << "Part1: " << result << endl;
    }

    void part2(StringView lines) {
        auto result = computeP2(lines, {
                                           "one",
                                           "two",
                                           "three",
                                           "four",
                                           "five",
                                           "six",
                                           "seven",
                                           "eight",
                                           "nine",
                                       });
        log << "Part2: " << result << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    /* auto lines = getFileLines(filename); */
    auto content = getFileContent(filename);
    {
        Context context;
        context.part1(content);
        context.part2(content);
    }
}

int main() {
    /* process("sample.txt"); */
    process("input.txt");
    return 0;
}
