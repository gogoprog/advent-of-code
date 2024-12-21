#include "../../common_fast.h"

struct Context {
    StringViews codes;

    struct State {
        Array<Coord, 3> positions;
    };

    // +---+---+---+
    // | 7 | 8 | 9 |
    // +---+---+---+
    // | 4 | 5 | 6 |
    // +---+---+---+
    // | 1 | 2 | 3 |
    // +---+---+---+
    //     | 0 | A |
    //     +---+---+

    inline static Map<char, Coord> numPad{
        // list
        {'7', {0, 0}}, {'8', {1, 0}}, {'9', {2, 0}}, //
        {'4', {0, 1}}, {'5', {1, 1}}, {'6', {2, 1}}, //
        {'1', {0, 2}}, {'2', {1, 2}}, {'3', {2, 2}}, //
        {'0', {1, 3}}, {'A', {2, 3}}                 //

    };

    //     +---+---+
    //     | ^ | A |
    // +---+---+---+
    // | < | v | > |
    // +---+---+---+

    inline static Map<char, Coord> dirPad{// list
                                          {'^', {1, 0}},
                                          {'<', {0, 1}},
                                          {'v', {1, 1}},
                                          {'>', {2, 1}},
                                          {'A', {2, 0}}};

    inline static Map<Coord, int> dirCharMap{// list
                                             {{0, -1}, '^'},
                                             {{-1, 0}, '<'},
                                             {{0, 1}, 'v'},
                                             {{1, 0}, '>'}};

    inline static Map<int, Coord> charDirMap{// list
                                             {'^', {0, -1}},
                                             {'<', {-1, 0}},
                                             {'v', {0, 1}},
                                             {'>', {1, 0}}};

    struct Device {
        Map<char, Coord> pad;
        Map<Coord, char> ipad;
        Coord current;
        Coord forbidden;
        Device *next{nullptr};
        Device *previous{nullptr};

        void init(auto _pad, auto _next, Coord forbid) {
            pad = _pad;
            next = _next;
            forbidden = forbid;

            if (next)
                next->previous = this;

            for (auto kv : pad) {
                ipad[kv.second] = kv.first;
            }

            assert(ipad.size() == pad.size());

            reset();
        }

        void reset() {
            current = pad['A'];
        }

        void validate() {
            if (next) {
                next->process(ipad[current]);
            } else {
                log << ipad[current];
            }
        }

        void process(StringView commands) {
            for (auto c : commands) {
                process(c);
            }

            log << endl;
        }

        void process(char value) {
            switch (value) {
                case '^':
                case '<':
                case '>':
                case 'v':
                    current += charDirMap[value];
                    break;
                case 'A':
                    validate();
                    break;
            }
        }

        int reverse(StringView values, String &output) {
            int result = 0;
            for (auto c : values) {
                reverse(c, result, output);
            }

            return result;
        }

        void reverse(char value, int &result, String &output) {

            auto doChar = [&](char c) {
                if (previous) {
                    previous->reverse(c, result, output);
                } else {
                    result++;
                    output += c;
                }
            };

            auto delta = pad[value] - current;

            auto test_x_first = [&]() {
                if (delta.x) {
                    int dirx = delta.x / abs(delta.x);

                    for (int i = 0; i < abs(delta.x); ++i) {
                        auto dir = Coord{dirx, 0};

                        if (current + dir * i == forbidden) {
                            return false;
                        }
                    }
                }

                if (delta.y) {
                    int diry = delta.y / abs(delta.y);

                    for (int i = 0; i < abs(delta.y); ++i) {
                        auto dir = Coord{0, diry};
                        current += dir;
                        if (current + dir * i == forbidden) {
                            return false;
                        }
                    }
                }

                return true;
            };

            auto process_delta_x = [&]() {
                if (delta.x) {
                    int dirx = delta.x / abs(delta.x);

                    for (int i = 0; i < abs(delta.x); ++i) {
                        auto dir = Coord{dirx, 0};
                        doChar(dirCharMap[dir]);
                    }
                }
            };

            auto process_delta_y = [&]() {
                if (delta.y) {
                    int diry = delta.y / abs(delta.y);

                    for (int i = 0; i < abs(delta.y); ++i) {
                        auto dir = Coord{0, diry};
                        doChar(dirCharMap[dir]);
                    }
                }
            };

            if (test_x_first()) {
                process_delta_x();
                process_delta_y();
            } else {
                process_delta_y();
                process_delta_x();
            }

            current = pad[value];

            doChar('A');
        }
    };

    Vector<Device> devices;

    void parse(auto lines) {
        rs::copy(lines, std::back_inserter(codes));

        devices.resize(3);

        devices[0].init(dirPad, &devices[1], {0, 0});
        devices[1].init(dirPad, &devices[2], {0, 0});
        devices[2].init(numPad, nullptr, {0, 3});
    }

    void reset() {

        for (auto &device : devices) {
            device.reset();
        }
    }

    int compute(auto code) {
        int result = 0;

        return result;
    }

    void part1() {
        auto result{0_int64};

        /* devices[2].process("<A^A>^^AvvvA"); */
        /* devices[1].process("v<<A>>^A<A>AvA<^AA>A<vAAA>^A"); */
        /* devices[0].process("<vA<AA>>^AvAA<^A>A<v<A>>^AvA^A<vA>^A<v<A>^A>AAvA^A<v<A>A>^AAAvA<^A>A"); */

        String out;
        log << devices[2].reverse("179A", out) << endl;

        reset();
        devices[0].process(out);

        auto get_num = [](auto code) { return parseInt(code); };

        /* for (auto code : codes) { */

        /*     reset(); */
        /*     auto a = devices[2].reverse(code); */
        /*     auto b = get_num(code); */

        /*     log << "a: " << a << ", b: " << b << endl; */
        /*     result += a * b; */
        /* } */

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0_int64};

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context context;
        context.parse(lines);
        context.part1();
        context.part2();
    }
}

int main() {
    process("sample.txt");
    /* process("input.txt"); */
    return 0;
}
