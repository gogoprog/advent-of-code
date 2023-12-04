/* #include "../../common.h" */

#include <string_view>
#include <ranges>
#include <algorithm>

namespace rs = std::ranges;
namespace rv = std::ranges::views;

using StringView = std::string_view;

inline StringView getStringView(auto range) {
    return StringView(&*range.begin(), rs::distance(range));
}

StringView getFileContent(const char *filename, const char delim = '\n') {

    static char buffer[32 * 1024];
    long length;
    FILE *f = fopen(filename, "rb");

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        fread(buffer, sizeof(char), length, f);
        fclose(f);
    }
    buffer[length - 1] = '\0';

    return StringView(buffer, length);
}

struct Context {
    int computeP1(const StringView lines) {
        auto computeLineValue = [&](auto range) {
            auto line = getStringView(range);
            auto linelen = line.length();

            int minval, maxval;

            for (int c = 0; c < linelen; ++c) {
                auto ch = *(line.begin() + c);
                switch (ch) {
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': {
                        minval = ch - '0';
                        c = linelen;
                    } break;
                }
            }

            for (int c = linelen - 1; c >= 0; --c) {
                auto ch = *(line.begin() + c);
                switch (ch) {
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': {
                        maxval = ch - '0';
                        c = -1;
                    } break;
                }
            }

            auto v = minval * 10 + maxval;

            return v;
        };

        return rs::fold_left(rs::split_view(lines, '\n') | rv::transform(computeLineValue), 0, std::plus<int>());
    }

    int computeP2(const StringView lines) {

        auto computeLineValue = [&](auto range) {
            auto line = getStringView(range);
            auto linelen = line.length();

            int minval = 0, maxval = 0;

            auto getChar = [&](auto index) { return *(line.begin() + index); };

            for (int c = 0; c < linelen; ++c) {
                auto ch = getChar(c);

                auto is = [&](int i, char value) { return getChar(c + i + 1) == value; };

                auto set = [&](int v) {
                    minval = v;
                    c = linelen;
                };

                auto accept = [&](auto n, char ch) {
                    if (!is(n - 1, ch)) {
                        return false;
                    }

                    c += n;

                    return true;
                };

                switch (ch) {
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': {
                        minval = ch - '0';
                        c = linelen;
                    } break;

                    case 'o':
                        if (is(0, 'n') && is(1, 'e')) {
                            set(1);
                        }
                        break;
                    case 't':
                        if (accept(1, 'w')) {
                            if (is(0, 'o'))
                                set(2);
                        } else if (accept(1, 'h') && accept(1, 'r') && is(0, 'e') && is(1, 'e')) {
                            set(3);
                        }
                        break;
                    case 'f':
                        if (is(0, 'o')) {
                            if (accept(2, 'u') && accept(1, 'r'))
                                set(4);
                        } else if (accept(1, 'i') && accept(1, 'v') && is(0, 'e')) {
                            set(5);
                        }
                        break;
                    case 's':
                        if (accept(1, 'i')) {
                            if (accept(1, 'x'))
                                set(6);
                        } else if (is(0, 'e') && accept(2, 'v') && is(0, 'e') && is(1, 'n')) {
                            set(7);
                        }
                        break;
                    case 'e':
                        if (accept(1, 'i') && accept(1, 'g') && accept(1, 'h') && is(0, 't')) {
                            set(8);
                        }
                        break;
                    case 'n':
                        if (accept(1, 'i') && is(0, 'n') && is(1, 'e')) {
                            set(9);
                        }
                        break;
                }
            }

            for (int c = linelen - 1; c >= 0; --c) {

                auto ch = getChar(c);

                auto is = [&](int i, char value) {
                    /* if (c - i - 1 <= 0) */
                    /*     return false; */

                    return getChar(c - i - 1) == value;
                };

                auto set = [&](int v) {
                    maxval = v;
                    c = -1;
                };

                auto accept = [&](auto n, char ch) {
                    if (!is(n - 1, ch)) {
                        return false;
                    }

                    c -= n;
                    return true;
                };

                switch (ch) {
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': {
                        maxval = ch - '0';
                        c = -1;
                    } break;

                    case 'e': {
                        if (is(0, 'n') && is(1, 'o')) {
                            set(1);
                        } else if (is(0, 'e') && is(1, 'r') && is(2, 'h') && is(3, 't')) {
                            set(3);
                        } else if (is(0, 'v') && is(1, 'i') && is(2, 'f')) {
                            set(5);
                        } else if (is(0, 'n') && is(1, 'i') && is(2, 'n')) {
                            set(9);
                        }
                    } break;

                    case 'o': {
                        if (accept(1, 'w') && is(0, 't')) {
                            set(2);
                        }
                    } break;

                    case 'r': {
                        if (accept(1, 'u') && is(0, 'o') && accept(2, 'f')) {
                            set(4);
                        }
                    } break;

                    case 'x': {
                        if (accept(1, 'i') && accept(1, 's')) {
                            set(6);
                        }
                    } break;

                    case 'n': {
                        if (is(0, 'e') && is(1, 'v') && is(2, 'e') && is(3, 's')) {
                            set(7);
                        }
                    } break;

                    case 't': {
                        if (is(0, 'h') && is(1, 'g') && is(2, 'i') && is(3, 'e')) {
                            set(8);
                        }
                    } break;
                }
            }

            auto v = minval * 10 + maxval;

            return v;
        };

        return rs::fold_left(rs::split_view(lines, '\n') | rv::transform(computeLineValue), 0, std::plus<int>());
    }

    void part1(StringView lines) {
        auto result = computeP1(lines);

        printf("Part1: %d\n", result);
    }

    void part2(StringView lines) {
        auto result = computeP2(lines);

        printf("Part2: %d\n", result);
    }
};

void process(const char *filename) {
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
