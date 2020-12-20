#include "../../common.h"

enum Direction { N, E, S, W };

struct State {
    char orientation{N};
    Pair<bool, bool> flips{false, false};
};

struct Tile {
    int id;
    Map<Point, bool> points;
    int size{0};
    Map<int, bool> borders;

    void set(const String line, const int y, const int size) {
        for (int x{0}; x < size; ++x) {
            if (line[x] == '#') {
                points[{x, y}] = true;
            }
        }
        this->size = size;
    }

    bool get(Point point, const State state) const {
        for (int i{0}; i < state.orientation; ++i) {
            auto tmp = point;
            point = {size - tmp.y - 1, tmp.x};
        }

        if (state.flips.first) {
            point = {size - point.x - 1, point.y};
        }

        if (state.flips.second) {
            point = {point.x, size - point.y - 1};
        }

        auto it = points.find(point);
        if (it != points.end()) {
            return it->second;
        }

        return false;
    }

    bool match(const Tile &other, const int dir, const State state, const State otherState) const {
        switch (dir) {
            case N: {
                for (int x{0}; x < size; ++x) {
                    if (get({x, 0}, state) != other.get({x, size - 1}, otherState)) {
                        return false;
                    }
                }
            } break;
            case S: {
                for (int x{0}; x < size; ++x) {
                    if (get({x, size - 1}, state) != other.get({x, 0}, otherState)) {
                        return false;
                    }
                }
            } break;
            case E: {
                for (int y{0}; y < size; ++y) {
                    if (get({size - 1, y}, state) != other.get({0, y}, otherState)) {
                        return false;
                    }
                }
            } break;
            case W: {
                for (int y{0}; y < size; ++y) {
                    if (get({0, y}, state) != other.get({size - 1, y}, otherState)) {
                        return false;
                    }
                }
            } break;
        }

        return true;
    }

    bool isBorder(const int worldDir, const State state) {
        int d = worldDir + state.orientation + 4;
        d %= 4;

        switch (d) {
            case N:
            case S: {
                if (state.flips.second)
                    d += 2;
            } break;

            case W:
            case E: {
                if (state.flips.first)
                    d += 2;
            } break;
        }

        return borders[d % 4];
    }

    void merge(const Tile &other, const int ox, const int oy, const State otherState, const int size) {
        for (int x{1}; x < size - 1; ++x) {
            for (int y{1}; y < size - 1; ++y) {

                auto value = other.get({x, y}, otherState);

                if (value) {
                    points[{ox + x - 1, oy + y - 1}] = value;
                }
            }
        }

        this->size = std::max(this->size, ox + (size - 2));
    }

    void draw(const State state = {}) const {
        for (int y{0}; y < size; ++y) {
            for (int x{0}; x < size; ++x) {
                if (get({x, y}, state)) {
                    log << "#";
                } else {
                    log << ".";
                }
            }

            log << endl;
        }
    }

    int compute(const State state) const {
        static Vector<String> monster{"                  # ", "#    ##    ##    ###", " #  #  #  #  #  #   "};
        static int mlen = monster[0].length();

        int result{0};

        auto matchMonster = [&](const int ox, const int oy) {
            for (int y{oy}; y < oy + 3; ++y) {
                for (int x{ox}; x < ox + mlen; ++x) {
                    if (x >= size || y >= size) {
                        return false;
                    }
                    if (monster[y - oy][x - ox] == '#') {
                        if (!get({x, y}, state)) {
                            return false;
                        }
                    }
                }
            }

            return true;
        };

        for (int y{0}; y < size - 3; ++y) {
            for (int x{0}; x < size - mlen; ++x) {

                if (matchMonster(x, y)) {
                    result++;
                }
            }
        }

        if (!result)
            return 0;

        return points.size() - result * 15;
    }
};

struct PlacedTile {
    int index{0};
    State state;
};

struct System {
    Vector<Tile> tiles;
    int size;

    void process() {
        size = int(std::sqrt(tiles.size()));

        State regularState{};

        log << "Finding borders" << endl;

        ull res{1};

        for (auto &tile : tiles) {

            for (int o{0}; o < 4; ++o) {

                auto hasMatches{false};

                for (auto &otherTile : tiles) {
                    if (tile.id != otherTile.id) {
                        State state;
                        for (int d{0}; d < 4; ++d) {
                            state.orientation = d;
                            for (auto fh : {false, true}) {
                                state.flips.first = fh;
                                for (auto fv : {false, true}) {
                                    state.flips.second = fv;

                                    bool match = tile.match(otherTile, o, regularState, state);
                                    if (match) {
                                        hasMatches = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                if (!hasMatches) {
                    tile.borders[o] = true;
                }
            }

            if (tile.borders.size() == 2) {
                res *= tile.id;
            }
        }

        log << "Part1: " << res << endl;

        struct Node {
            Vector<PlacedTile> tiles;
            Set<int> used;

            bool operator<(const Node &other) const {
                return tiles.size() < other.tiles.size();
            }
        };

        auto getCoord = [&](const int index) { return Point{index % size, int(index / size)}; };
        auto getIndex = [&](const int x, const int y) { return y * size + x; };

        PriorityQueue<Node> q{};

        q.push({});

        Node solution;

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();

            if (node.tiles.size() == size * size) {
                log << "found" << endl;
                auto &otherPlacedTile = node.tiles[0];
                auto &otherTile = tiles[otherPlacedTile.index];

                log << node.tiles.size() << endl;

                int i{0};
                for (auto ptile : node.tiles) {
                    log << tiles[ptile.index].id << " ";

                    if (!(++i % size))
                        log << endl;
                }

                log << endl;

                solution = node;
                break;
            }

            for (int t{0}; t < tiles.size(); ++t) {
                if (node.used.find(t) == node.used.end()) {
                    auto &tile = tiles[t];
                    State state;
                    for (int d{0}; d < 4; ++d) {
                        state.orientation = d;
                        for (auto fh : {false, true}) {
                            state.flips.first = fh;
                            for (auto fv : {false, true}) {
                                state.flips.second = fv;
                                auto copy = node;
                                auto newIndex = copy.tiles.size();

                                auto newPoint = getCoord(newIndex);

                                if (newPoint.x == 0) {
                                    if (!tile.isBorder(W, state)) {
                                        break;
                                    }
                                }

                                if (newPoint.x == size - 1) {
                                    if (!tile.isBorder(E, state)) {
                                        break;
                                    }
                                }

                                if (newPoint.y == 0) {
                                    if (!tile.isBorder(N, state)) {
                                        break;
                                    }
                                }

                                if (newPoint.y == size - 1) {
                                    if (!tile.isBorder(S, state)) {
                                        break;
                                    }
                                }

                                if (newPoint.x - 1 >= 0) {
                                    auto otherIndex = getIndex(newPoint.x - 1, newPoint.y);
                                    auto &otherPlacedTile = copy.tiles[otherIndex];
                                    auto &otherTile = tiles[otherPlacedTile.index];

                                    if (!tile.match(otherTile, W, state, otherPlacedTile.state)) {
                                        break;
                                    }
                                }

                                if (newPoint.y - 1 >= 0) {
                                    auto otherIndex = getIndex(newPoint.x, newPoint.y - 1);
                                    auto &otherPlacedTile = copy.tiles[otherIndex];
                                    auto &otherTile = tiles[otherPlacedTile.index];

                                    if (!tile.match(otherTile, N, state, otherPlacedTile.state)) {
                                        break;
                                    }
                                }

                                copy.tiles.push_back({t, state});
                                copy.used.insert(t);
                                q.push(copy);
                            }
                        }
                    }
                }
            }
        }

        {
            Tile picture{};

            int i{0};
            for (auto &ptile : solution.tiles) {
                auto coord = getCoord(i);
                auto &tile = tiles[ptile.index];
                auto newSize = tile.size - 2;

                picture.merge(tile, newSize * coord.x, newSize * coord.y, ptile.state, tile.size);
                ++i;
            }

            /* picture.draw(); */

            {
                State state;
                for (int d{0}; d < 4; ++d) {
                    state.orientation = d;
                    for (auto fh : {false, true}) {
                        state.flips.first = fh;
                        for (auto fv : {false, true}) {
                            state.flips.second = fv;
                            int count = picture.compute(state);
                            if (count > 0) {
                                log << "Part2: " << count << endl;
                                /* picture.draw(state); */
                            }
                        }
                    }
                }
            }
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    System system{};

    for (int l{0}; l < lines.size(); l++) {
        auto &line = lines[l];
        InputStringStream iss{line};
        system.tiles.push_back({});
        auto &tile = system.tiles.back();

        String tmp;
        iss >> tmp >> tile.id;

        l++;

        int size = lines[l].length();

        for (int k{0}; k < size; ++k) {
            tile.set(lines[l + k], k, size);
        }

        l += size;
    }

    system.process();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
