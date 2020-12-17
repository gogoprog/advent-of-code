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

    void iterate(const Coord lmin, const Coord lmax, const int d, Coord coord,
                 const std::function<void(const Coord &)> &func) const {
        for (int i{lmin[d] - 1}; i <= lmax[d] + 1; ++i) {
            coord[d] = i;
            if (d == D - 1) {
                func(coord);
            } else {
                iterate(lmin, lmax, d + 1, coord, func);
            }
        }
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

        iterate(lmin, lmax, 0, {}, [&](auto coord) {
            auto active = state[coord];
            int activeCount = countActiveNeighbors(coord);

            if (active) {
                write(coord, activeCount == 2 || activeCount == 3);
            } else {
                write(coord, activeCount == 3);
            }
        });

        state = nextState;
    }

    int countActiveNeighbors(const Coord &pos) const {
        int r{0};

        iterate(pos, pos, 0, {}, [&](auto coord) {
            if (pos != coord) {
                auto it = state.find(coord);
                if (it != state.end()) {
                    r += (it->second == true) ? 1 : 0;
                }
            }
        });

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

template <int D> void process(const Vector<String> &lines) {
    Cube<D> cube{};
    cube.init(lines);

    for (int i{0}; i < 6; ++i) {
        cube.process();
    }

    log << "Part" << D - 2 << " : " << cube.count() << endl;
}

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    process<3>(lines);
    process<4>(lines);
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
