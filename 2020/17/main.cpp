#include "../../common.h"

template <int D> struct Cube {
    using Coord = Array<int, D>;
    using State = Map<Coord, bool>;
    State state;

    Coord min, max;

    void init(const Vector<String> &lines) {
        for (int y{0}; y < lines.size(); ++y) {
            auto &line = lines[y];
            for (int x{0}; x < line.length(); ++x) {
                char c = line[x];
                state[{x, y, 0}] = (c == '#');
            }
        }

        min = {0, 0, 0};
        max = {int(lines[0].length()), int(lines.size()), 0};
    }

    void process() {
        static Coord temp;
        State nextState;

        auto lmin = min;
        auto lmax = max;

        auto write = [&](const auto &pos, auto value) {
            nextState[pos] = value;

            for (int i{0}; i < D; ++i) {
                min[i] = std::min(min[i], pos[i]);
                max[i] = std::max(max[i], pos[i]);
            }
        };

        if constexpr (D == 3) {
            for (int x{lmin[0] - 1}; x <= lmax[0] + 1; ++x) {
                for (int y{lmin[1] - 1}; y <= lmax[1] + 1; ++y) {
                    for (int z{lmin[2] - 1}; z <= lmax[2] + 1; ++z) {
                        temp = {x, y, z};
                        auto active = state[temp];
                        int activeCount = countActiveNeighbors(temp);

                        if (active) {
                            write(temp, activeCount == 2 || activeCount == 3);
                        } else {
                            write(temp, activeCount == 3);
                        }
                    }
                }
            }
        } else {
            for (int x{lmin[0] - 1}; x <= lmax[0] + 1; ++x) {
                for (int y{lmin[1] - 1}; y <= lmax[1] + 1; ++y) {
                    for (int z{lmin[2] - 1}; z <= lmax[2] + 1; ++z) {
                        for (int w{lmin[3] - 1}; w <= lmax[3] + 1; ++w) {
                            temp = {x, y, z, w};
                            auto active = state[temp];
                            int activeCount = countActiveNeighbors(temp);

                            if (active) {
                                write(temp, activeCount == 2 || activeCount == 3);
                            } else {
                                write(temp, activeCount == 3);
                            }
                        }
                    }
                }
            }
        }

        state = nextState;
    }

    int countActiveNeighbors(const Coord &pos) const {
        static Coord temp;
        int r{0};

        if constexpr (D == 3) {
            for (int x{-1}; x <= 1; ++x) {
                temp[0] = pos[0] + x;
                for (int y{-1}; y <= 1; ++y) {
                    temp[1] = pos[1] + y;
                    for (int z{-1}; z <= 1; ++z) {
                        temp[2] = pos[2] + z;

                        if (pos != temp) {
                            auto it = state.find(temp);
                            if (it != state.end()) {
                                r += (it->second == true) ? 1 : 0;
                            }
                        }
                    }
                }
            }
        } else {
            for (int x{-1}; x <= 1; ++x) {
                temp[0] = pos[0] + x;
                for (int y{-1}; y <= 1; ++y) {
                    temp[1] = pos[1] + y;
                    for (int z{-1}; z <= 1; ++z) {
                        temp[2] = pos[2] + z;
                        for (int w{-1}; w <= 1; ++w) {
                            temp[3] = pos[3] + w;

                            if (pos != temp) {
                                auto it = state.find(temp);
                                if (it != state.end()) {
                                    r += (it->second == true) ? 1 : 0;
                                }
                            }
                        }
                    }
                }
            }
        }

        return r;
    }

    int count() const {
        int r{0};
        for (auto &kv : state) {
            if (kv.second) {
                ++r;
            }
        }

        return r;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    {
        Cube<3> cube{};
        cube.init(lines);

        for (int i{0}; i < 6; ++i) {
            cube.process();
        }

        log << "Part1: " << cube.count() << endl;
    }
    {
        Cube<4> cube{};
        cube.init(lines);

        for (int i{0}; i < 6; ++i) {
            cube.process();
        }

        log << "Part2: " << cube.count() << endl;
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
