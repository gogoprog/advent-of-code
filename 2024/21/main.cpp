#include "../../common_fast.h"

struct Context {
    StringViews codes;

    using State = Array<Coord, 27>;

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

        Uint64 reverse(StringView values) {
            Uint64 result = 0;
            for (auto c : values) {
                reverse(c, result);
            }

            return result;
        }

        Map<Pair<Coord, char>, Uint64> cache;

        void reverse(char value, Uint64 &result) {
            auto key = Pair<Coord, char>{current, value};
            auto it = cache.find(key);

            /* if (next == nullptr) { */
            if (it != cache.end()) {
                /* log << current << " " << value << " "; */
                /* log << it->second << " vs " ; */
                current = pad[value];
                result += it->second;
                return;
            }
            /* } */

            Uint64 this_result = 0;

            auto doChar = [&](char c) {
                if (previous) {
                    previous->reverse(c, this_result);
                } else {
                    this_result++;
                }
            };

            auto delta = pad[value] - current;

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

            auto can_x = canMoveXFirst(current, delta);
            auto can_y = canMoveYFirst(current, delta);
            Uint64 a = std::numeric_limits<Uint64>::max();
            Uint64 b = std::numeric_limits<Uint64>::max();

            current = pad[value];

            if (can_y) {
                this_result = 0;
                process_delta_y();
                process_delta_x();
                doChar('A');
                a = this_result;
            }

            if (can_x) {
                this_result = 0;
                process_delta_x();
                process_delta_y();
                doChar('A');
                b = this_result;
            }

            this_result = std::min<Uint64>(a, b);

            result += this_result;

            if (next == nullptr) {
                if (it != cache.end() && it->second != this_result) {
                    log << "Whoops, this result is not equal: " << it->second << " vs " << this_result << endl;
                }
            }

            cache[key] = this_result;
        }

        bool canMoveXFirst(const Coord current, const Coord delta) const {
            auto tmp = current;
            if (delta.x) {
                int dirx = delta.x / abs(delta.x);
                auto dir = Coord{dirx, 0};

                for (int i = 0; i < abs(delta.x); ++i) {

                    tmp += dir;

                    if (tmp == forbidden) {
                        return false;
                    }
                }
            }

            if (delta.y) {
                int diry = delta.y / abs(delta.y);
                auto dir = Coord{0, diry};

                for (int i = 0; i < abs(delta.y); ++i) {

                    tmp += dir;

                    if (tmp == forbidden) {
                        return false;
                    }
                }
            }

            return true;
        }

        bool canMoveYFirst(const Coord current, const Coord delta) const {
            auto tmp = current;
            if (delta.y) {
                int diry = delta.y / abs(delta.y);
                auto dir = Coord{0, diry};

                for (int i = 0; i < abs(delta.y); ++i) {

                    tmp += dir;

                    if (tmp == forbidden) {
                        return false;
                    }
                }
            }

            if (delta.x) {
                int dirx = delta.x / abs(delta.x);
                auto dir = Coord{dirx, 0};

                for (int i = 0; i < abs(delta.x); ++i) {

                    tmp += dir;

                    if (tmp == forbidden) {
                        return false;
                    }
                }
            }

            return true;
        }
    };

    Vector<Device> devices;

    void parse(auto lines) {
        rs::copy(lines, std::back_inserter(codes));
    }

    void reset() {
        for (auto &device : devices) {
            device.reset();
        }
    }

    void part1() {
        auto result{0_int64};

        devices.resize(3);

        devices[0].init(dirPad, &devices[1], {0, 0});
        devices[1].init(dirPad, &devices[2], {0, 0});
        devices[2].init(numPad, nullptr, {0, 3});

        auto get_num = [](auto code) { return parseInt(code); };

        for (auto code : codes) {
            reset();
            auto b = get_num(code);
            auto a = devices.back().reverse(code);
            log << "a: " << a << ", b: " << b << endl;
            result += a * b;
        }

        log << "Part1: " << result << endl;
    }

    void part2() {

        for (int robots = 20; robots <= 25; ++robots) {
            log << "Robots = " << robots << endl;

            Uint64 result{0};

            devices.clear();
            devices.resize(robots + 1);

            for (int i = 0; i < robots; ++i) {
                devices[i].init(dirPad, &devices[i + 1], {0, 0});
            }

            devices[robots].init(numPad, nullptr, {0, 3});

            auto get_num = [](auto code) { return parseUint64(code); };

            /* result = devices.back().reverse("0"); */
            for (auto code : codes) {
                reset();
                auto b = get_num(code);
                auto a = devices.back().reverse(code);
                log << "a: " << a << ", b: " << b << endl;
                result += a * b;
            }
            log << "Part2: " << result << endl;
        }
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
    process("input.txt");
    return 0;
}
