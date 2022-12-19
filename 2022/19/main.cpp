#include "../../common.h"

enum { ORE, CLAY, OBSIDIAN, GEODE };

Map<String, int> types{
    {"ore", 0},
    {"clay", 1},
    {"obsidian", 2},
    {"geode", 3},
};

using Blueprint = Array<Array<int, 3>, 4>;

template <int max_time> struct Context {
    Vector<Blueprint> blueprints;

    void parse(const Strings &lines) {
        String str;
        for (auto &line : lines) {
            auto parts1 = splitString(line, ':');
            auto parts2 = splitString(parts1[1], '.');

            blueprints.push_back({});

            auto &blueprint = blueprints.back();

            for (auto i : rs::iota_view(0, 4)) {
                InputStringStream iss{parts2[i]};

                int n;
                iss >> str >> str >> str >> str >> n >> str;

                blueprint[i][types[str]] = n;

                if (!iss.eof()) {
                    iss >> str >> n >> str;
                    blueprint[i][types[str]] = n;
                }
            }
        }
    }

    struct Node {
        Array<int, 4> resources;
        Array<int8_t, 4> robots;
        int8_t time{0};
        int8_t new_robot = -1;
        int8_t next_robot = GEODE;

        Node() {
            std::fill(resources.begin(), resources.end(), 0);
            std::fill(robots.begin(), robots.end(), 0);
        }

        bool update(const Blueprint &bp) {
            time++;

            for (int i{0}; i < 4; ++i) {
                resources[i] += robots[i];
            }

            if (new_robot != -1) {
                robots[new_robot]++;
                new_robot = -1;
            }

            if (next_robot != -1) {
                if (canBuy(bp, next_robot)) {
                    buy(bp, next_robot);
                    next_robot = -1;
                    return true;
                }
            }

            return false;
        }

        bool canBuy(const Blueprint &bp, const int type) const {
            auto &costs = bp[type];

            for (int i{0}; i < 3; ++i) {
                if (resources[i] < costs[i]) {
                    return false;
                }
            }

            return true;
        }

        bool willBeAbleToBuy(const Blueprint &bp, const int type) const {
            auto &costs = bp[type];

            for (int i{0}; i < 3; ++i) {
                auto cost = costs[i];

                if (cost && !robots[i]) {
                    return false;
                }
            }
            return true;
        }

        void buy(const Blueprint &bp, const int type) {
            auto &costs = bp[type];

            for (int i{0}; i < 3; ++i) {
                resources[i] -= costs[i];
            }

            new_robot = type;
        }

        bool needResource(const Blueprint &bp, const int type, const int res) const {
            auto &costs = bp[type];

            return resources[res] < costs[res];
        }

        int countNeededResource(const Blueprint &bp, const int type, const int res) const {
            auto &costs = bp[type];

            auto total{0};

            for (int i{0}; i < 3; ++i) {
                if (resources[i] < costs[i]) {
                    auto delta = costs[i] - resources[i];
                    if (i == res) {
                        total += delta;
                    } else {
                        total += delta * countNeededResource(bp, i, res);
                    }
                }
            }

            return total;
        }

        void getNeededRobots(Vector<int> &result, const Blueprint &bp, const int type) const {
            auto &costs = bp[type];
            for (int i{0}; i < 3; ++i) {
                if (resources[i] < costs[i]) {
                    result.push_back(i);
                }
            }
        }

        int timeToReach(const Blueprint &bp, const int type, const int amount) const {
            if (resources[type] > amount) {
                return 0;
            }
            if (!robots[type]) {
                return -1;
            }

            auto delta = amount - resources[type];

            return int(std::ceil(float(delta) / robots[type]));
        }

        bool isNeeded(const Blueprint &bp, const int type) const {

            for (auto &costs : bp) {
                if (resources[type] < costs[type]) {
                    return true;
                }
            }

            return false;
        }

        bool operator<(const Node &other) const {

            /* for (int i = 3; i >= 0; ++i) { */
            /*     if (resources[i] != other.resources[i]) { */
            /*         return resources[i] < other.resources[i]; */
            /*     } */
            /* } */

            /* return score() < other.score(); */
            return time < other.time;
        }

        bool operator==(const Node &other) const = default;

        inline int score() const {
            return resources[GEODE];
        }

        inline int robotCount() const {
            return robots[0] + robots[1] + robots[2] + robots[3] + ((new_robot != -1) ? 1 : 0);
        }

        void print() const {
            log << "resources:" << resources << endl;
            log << "robots:" << robots << endl;
        }
    };

    Node simulate(const Blueprint &bp, const Vector<int8_t> &list, const int max) {
        Node node;
        int l{0};
        node.robots[ORE] = 1;

        if (l < list.size()) {
            node.next_robot = list[l++];
        }

        for (auto i : rs::iota_view(0, max)) {
            if (node.update(bp)) {
                if (l < list.size()) {
                    node.next_robot = list[l++];
                } else {
                    return node;
                }
            }
        }
        return node;
    }

    void recurSim(const Blueprint &bp, Vector<int8_t> list, Vector<int8_t> &best_list, int &best, const int robots,
                  const int max) {

        auto node = simulate(bp, list, max);

        if (node.score() > best) {
            best = node.score();
            best_list = list;
            /* node.print(); */
        }

        if (node.robotCount() > robots) {
            if (node.canBuy(bp, 3)) {
                auto copy = list;
                copy.push_back(3);
                recurSim(bp, copy, best_list, best, node.robotCount(), max);
            } else {
                for (int i = 0; i < 4; ++i) {
                    /* if (i == 3 || node.isNeeded(bp, i)) { */

                    /* if (node.willBeAbleToBuy(bp, i)) { */

                    if (i == 0 && list.size() > 5)
                        continue;
                    if (i == 1 && list.size() > 15)
                        continue;

                    auto copy = list;
                    copy.push_back(i);
                    recurSim(bp, copy, best_list, best, node.robotCount(), max);
                    /* } */
                    /* } */
                }
            }
        }
    }

    int compute(const int bp_index) {

        Vector<int8_t> best_list;
        auto bp = blueprints[bp_index];

        int best{0};

        /* recurSim(bp, best_list, best_list, best, 0, m); */
        recurSim(bp, {}, best_list, best, 0, max_time);

        return best;
    }

    void part1() {
        auto result{0};

        for (int i = 0; i < blueprints.size(); ++i) {
            /* log << "compute " << i << endl; */
            auto r = compute(i);
            result += (i + 1) * r;
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{1};

        for (int i = 0; i < std::min<int>(blueprints.size(), 3); ++i) {
            /* log << "compute " << i << endl; */
            auto r = compute(i);
            result *= r;
        }

        log << "Part2: " << result << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    {
        Context<24> context;
        context.parse(lines);
        context.part1();
    }
    {
        Context<32> context;
        context.parse(lines);
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
