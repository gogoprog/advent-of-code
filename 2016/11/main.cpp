#include "../../common.h"

template <typename T> void fillAllSubsets(Vector<Set<T>> & result, Set<T> set) {
    static Set<T> empty;
    result.push_back(empty);

    for (auto &seti : set) {
        auto subsetTemp = result;

        for (auto &subsetTempj : subsetTemp)
            subsetTempj.insert(seti);

        for (const auto &subsetTempj : subsetTemp)
            result.push_back(subsetTempj);
    }
}

enum Type { MICROCHIP, GENERATOR };

struct Item {
    Type type;
    char atom;

    bool operator==(const Item &other) const { return type == other.type && atom == other.atom; };
    bool operator<(const Item &other) const {
        if (type == other.type) {
            return atom < other.atom;
        }

        return type < other.type;
    }
};

struct State {
    Array<Set<Item>, 4> floors;
    int elevatorFloor{0};

    auto &currentFloorItems() { return floors[elevatorFloor]; }
    const auto &currentFloorItems() const { return floors[elevatorFloor]; }

    bool isComplete() const {
        return elevatorFloor == 3 && std::all_of(floors.begin(), floors.end() - 1, [](auto &f) { return f.empty(); });
    }

    bool operator==(const State &other) const { return elevatorFloor == other.elevatorFloor && floors == other.floors; }
    bool operator<(const State &other) const {
        if (elevatorFloor != other.elevatorFloor) {
            return elevatorFloor < other.elevatorFloor;
        }

        for (int f{0}; f < 4; ++f) {
            auto &floor = floors[f];
            auto &ofloor = other.floors[f];

            if (floor.size() != ofloor.size()) {
                return floor.size() < ofloor.size();
            } else {

                auto oit = ofloor.begin();
                for (auto &item : floor) {
                    auto &oitem = *oit;

                    if (!(item == oitem)) {
                        return item < oitem;
                    }
                    ++oit;
                }
            }
        }

        return false;
    }
};

using StateStepMap = Map<State, int>;

struct Context {
    State state;

    void debug() {
        for (int i{3}; i >= 0; --i) {
            auto &floor = state.floors[i];

            log << i << ": ";

            if (i == state.elevatorFloor) {
                log << "E ";
            } else {
                log << "  ";
            }

            for (auto &item : floor) {
                log << item.atom << (item.type == MICROCHIP ? 'M' : 'G') << ' ';
            }

            log << endl;
        }

        log << endl;
    }

    static bool isValid(Set<Item> &items) {
        if (items.size() == 1) {
            return true;
        }

        if (std::all_of(items.begin(), items.end(), [&](auto &a) {
                if (a.type == GENERATOR) {
                    return true;
                }

                return std::any_of(items.begin(), items.end(),
                                   [&](auto &b) { return a.atom == b.atom && b.type == GENERATOR; });
            })) {
            return true;
        }

        return false;
    }

    static void fillAdjacents(Vector<State> &result, const State &state) {
        static Vector<Set<Item>> combos;
        combos.resize(0);
        fillAllSubsets(combos, state.currentFloorItems());

        result.resize(0);

        for (auto &combo : combos) {
            if (combo.size() > 0 && combo.size() <= 2) {

                State tempState{state};
                auto &currentFloor = tempState.currentFloorItems();

                for (auto &item : combo) {
                    currentFloor.erase(item);
                }

                if (isValid(currentFloor)) {

                    for (int i{1}; i >= -1; i -= 2) {
                        int f = state.elevatorFloor;
                        f += i;

                        if (f >= 0 && f < 4) {
                            State nextState{tempState};
                            nextState.elevatorFloor = f;
                            auto &newFloor = nextState.currentFloorItems();

                            for (auto &item : combo) {
                                newFloor.insert(item);
                            }

                            if (isValid(newFloor)) {
                                result.push_back(nextState);
                            }
                        }
                    }
                }
            }
        }
    }

    struct Node {
        State state;
        int steps;
        bool operator<(const Node &other) const { return steps > other.steps; }
    };

    void compute() {
        int minSteps{128};
        StateStepMap visited;
        std::priority_queue<Node> q;

        Vector<State> adjacents;

        q.push({state, 0});

        while (!q.empty()) {
            auto node = q.top();
            q.pop();

            if (node.steps < minSteps) {
                int &s = visited[node.state];

                if (s != 0 && s <= node.steps) {
                    continue;
                } else {
                    s = node.steps;
                }

                if (node.state.isComplete()) {
                    minSteps = node.steps;
                    log << "found: " << minSteps << endl;
                }

                fillAdjacents(adjacents, node.state);

                for (auto &otherState : adjacents) {
                    q.push({otherState, node.steps + 1});
                }
            }
        }

        log << "best min steps is " << minSteps << endl;
    }
};

int main() {
    {
        // sample
        Context context;

        context.state.floors[0] = {{MICROCHIP, 'H'}, {MICROCHIP, 'L'}};
        context.state.floors[1] = {{GENERATOR, 'H'}};
        context.state.floors[2] = {{GENERATOR, 'L'}};

        context.compute();
    }
    {
        // input
        /* The first floor contains a thulium generator, a thulium-compatible microchip, a plutonium generator, and
         * a strontium generator. */
        /* The second floor contains a plutonium-compatible microchip and a strontium-compatible microchip. */
        /* The third floor contains a promethium generator, a promethium-compatible microchip, a ruthenium
         * generator, and a ruthenium-compatible microchip. */
        /* The fourth floor contains nothing relevant. */

        Context context;
        context.state.floors[0] = {{GENERATOR, 'T'}, {MICROCHIP, 'T'}, {GENERATOR, 'P'}, {GENERATOR, 'S'}};
        context.state.floors[1] = {{MICROCHIP, 'P'}, {MICROCHIP, 'S'}};
        context.state.floors[2] = {{GENERATOR, 'O'}, {MICROCHIP, 'O'}, {GENERATOR, 'R'}, {MICROCHIP, 'R'}};

        Context part2Context{context};

        part2Context.state.floors[0].insert({GENERATOR, 'E'});
        part2Context.state.floors[0].insert({MICROCHIP, 'E'});
        part2Context.state.floors[0].insert({GENERATOR, 'D'});
        part2Context.state.floors[0].insert({MICROCHIP, 'D'});

        context.compute();
        part2Context.compute();
    }
    return 0;
}
