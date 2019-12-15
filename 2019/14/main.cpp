#include "../../common.h"

struct Chemical {
    String name;
    int quantity;

    bool operator<(const Chemical &other) const {
        if (name == other.name) {
            return quantity < other.quantity;
        }

        return name < other.name;
    }
};

struct Reaction {
    Vector<Chemical> inputs;
    Chemical output;

    void parse(const String line) {
        auto [left, right] = splitNString<String, 2>(line, '=');

        auto chems = splitString(left, ',');

        for (auto str : chems) {
            InputStringStream iss{str};
            Chemical chem;
            iss >> chem.quantity;
            iss >> chem.name;
            inputs.push_back(chem);
        }

        InputStringStream iss{right};
        iss.ignore(1);

        iss >> output.quantity;
        iss >> output.name;
    }
};

using Reactions = Map<String, Reaction>;

struct State : Map<String, int> {
    bool operator<(const State &other) const {
        auto s = size();
        auto os = other.size();

        if (s == os) {
            auto it = begin();
            auto oit = other.begin();

            while (it != end()) {
                if (it->first == oit->first) {

                    if (it->second == oit->second) {
                        ++it;
                        ++oit;
                    } else {
                        return it->second < oit->second;
                    }
                } else {
                    return it->first < oit->first;
                }
            }
            return false;

        } else {
            return s < os;
        }
        /* return at("ORE") < other.at("ORE"); */
    }
};

struct Expander {

    Map<Chemical, State> cache;

    bool expand(const Reactions &reactions, const String &chem, const int qty, State &state) {

        /* volatile State &theState = state; */

        auto cache_key = Chemical{chem, qty};
        auto cache_it = cache.find(cache_key);

        if (cache_it != cache.end()) {
            state = cache_it->second;
            return true;
        }

        auto rIt = reactions.find(chem);
        if (rIt != reactions.end()) {

            auto reacQty = rIt->second.output.quantity;

            auto count = qty / reacQty;
            auto rest = qty % reacQty;

            if (count > 0) {

                if (rest > 0) {
                    state[chem] = rest;
                }

                for (auto &c : rIt->second.inputs) {
                    state[c.name] += c.quantity * count;
                }

                bool changed{true};

                while (changed) {
                    changed = false;

                    for (auto &kv : state) {
                        int q = kv.second;
                        state[kv.first] = 0;
                        State addition;
                        changed |= expand(reactions, kv.first, q, addition);

                        for (auto &addkv : addition) {
                            state[addkv.first] += addkv.second;
                        }

                        if (state[kv.first] == 0) {
                            state.erase(kv.first);
                            break;
                        }
                    }
                }

                cache[cache_key] = state;
                return true;

            } else {
                state[chem] = qty;
                return false;
            }
        } else {
            state[chem] = qty;
            return false;
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Reactions reactions;

    for (auto &line : lines) {
        Reaction reaction;
        reaction.parse(line);

        reactions[reaction.output.name] = reaction;
    }

    State state;
    Expander expander;
    expander.expand(reactions, "FUEL", 1, state);

    for (auto &kv : state) {
        log << kv << endl;
    }

    int result = 20000000;

    Map<State, bool> visited;
    auto cmp = [](const State &a, const State &b) { return a.size() < b.size(); };
    std::priority_queue<State, Vector<State>, decltype(cmp)> q(cmp);
    q.push({{{"FUEL", 1}, {"ORE", 0}}});

    while (!q.empty()) {
        const auto state = q.top();
        q.pop();
        auto ore_it = state.find("ORE");

        if (visited[state]) {
            continue;
        }

        visited[state] = true;

        if (ore_it->second < result) {
            State reacted_state;
            auto reacted = false;

            /* log << "reacting" << endl; */
            for (auto &kv : state) {
                /* log << kv << endl; */
                State addition;
                reacted |= expander.expand(reactions, kv.first, kv.second, addition);
                for (auto &addkv : addition) {
                    reacted_state[addkv.first] += addkv.second;
                }
            }

            if (reacted_state.size() == 1) {
                if (reacted_state["ORE"] < result) {
                    result = reacted_state["ORE"];
                    log << result << endl;
                }
            }

            /* if (reacted_state != state) { */
            q.push(reacted_state);

            for (auto &kv : reacted_state) {
                auto rIt = reactions.find(kv.first);
                if (rIt != reactions.end()) {
                    auto requiredQty = kv.second;
                    auto reacQty = rIt->second.output.quantity;

                    if (requiredQty < reacQty) {
                        State copy = reacted_state;
                        /* auto current = reacted_state[kv.first]; */
                        auto needed = reactions.at(kv.first).output.quantity;
                        copy[kv.first] = needed;

                        q.push(copy);
                    }
                }
            }
            /* } */
        }
    }

    log << "part1: " << result << endl;
}

int main() {
    process("sample.txt");
    /* process("sample2.txt"); */
    /* process("sample3.txt"); */
    process("sample4.txt");
    process("sample5.txt");
    /* process("input.txt"); */
    return 0;
}
