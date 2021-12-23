#include "../../common.h"

struct Pod {
    char type;
    Point position;

    bool isInHallway() const {
        return position.y == 1;
    }

    bool operator==(const Pod &other) const {
        return type == other.type && position == other.position;
    }

    bool operator<(const Pod &other) const {

        if (type == other.type) {
            return position < other.position;
        }
        return type < other.type;
    }
};

struct Context {
    Map<Point, bool> walls;
    Vector<Pod> pods;

    void init(const Vector<String> &lines) {
        auto len = lines.size();

        for (int l = 0; l < len; ++l) {

            auto &line = lines[l];

            for (int x = 0; x < line.size(); ++x) {

                char c = line[x];

                walls[{x, l}] = c == '#';

                if (c >= 'A' && c <= 'D') {
                    pods.push_back({c, {x, l}});
                }
            }
        }
    }

    struct Node {
        Vector<Pod> pods;
        int cost = 0;

        Vector<int> history;

        bool operator<(const Node &other) const {

            if (cost == other.cost) {
                return pods < other.pods;
            }

            return cost > other.cost;
        }

        bool operator==(const Node &other) const {
            return pods == other.pods && cost == other.cost;
        }

        bool isArrived(const Pod &pod) const {
            if (pod.position.y == 1)
                return false;

            constexpr int roomXs[4] = {3, 5, 7, 9};
            auto roomx = roomXs[pod.type - 'A'];

            if (pod.position.x != roomx) {
                return false;
            }

            if (pod.position.y == 3)
                return true;

            if (getPodTypeAtPosition({roomx, 3}) == pod.type) {
                return true;
            }

            return false;
        }

        bool canCrossTo(const Pod &pod, const int x) const {

            auto x1 = std::min(pod.position.x, x);
            auto x2 = std::max(pod.position.x, x);

            for (auto &other : pods) {
                if (&pod != &other) {
                    if (other.position.y == 1) {
                        if (other.position.x >= x1 && other.position.x <= x2) {
                            return false;
                        }
                    }
                }
            }

            return true;
        }

        bool canMoveToHallway(const Pod &pod, const int x) const {

            if (pod.position.y == 1)
                return false;

            auto x1 = std::min(pod.position.x, x);
            auto x2 = std::max(pod.position.x, x);

            if (!canCrossTo(pod, x))
                return false;

            if (pod.position.y == 2 || getPodTypeAtPosition({pod.position.x, 2}) == 0) {
                return true;
            }

            return false;
        }

        bool canMoveToItsRoom(const Pod &pod, Point &target) const {
            constexpr int roomXs[4] = {3, 5, 7, 9};

            auto roomx = roomXs[pod.type - 'A'];

            /* if (pod.position.y == 1 || canMoveToHallway(pod, roomx)) { */
            if (pod.position.y == 1) {

                if (pod.position.x == roomx)
                    return false;

                if (!canCrossTo(pod, roomx)) {
                    return false;
                }

                auto r2 = getPodTypeAtPosition({roomx, 2});
                auto r3 = getPodTypeAtPosition({roomx, 3});

                if (!r2 && !r3) {
                    target = {roomx, 3};
                    return true;
                }

                if (!r2 && r3 == pod.type) {
                    target = {roomx, 2};
                    return true;
                }
            }

            return false;
        }

        char getPodTypeAtPosition(const Point &p) const {

            for (auto &pod : pods) {

                if (pod.position == p) {

                    return pod.type;
                }
            }

            return 0;
        }

        bool isCompleted() const {
            for (int t = 0; t < 4; ++t) {

                auto expected = 'A' + t;

                for (int c = 0; c < 2; ++c) {
                    auto pos = Point{3 + t * 2, 2 + c};

                    if (expected != getPodTypeAtPosition(pos)) {
                        return false;
                    }
                }
            }

            return true;
        }

        void moveTo(const int pod_index, const Point &to) {
            constexpr int factors[4] = {1, 10, 100, 1000};

            auto &pod = pods[pod_index];
            auto from = pod.position;
            pod.position = to;

            auto value = (std::abs(to.x - from.x) + std::abs(to.y - from.y)) * factors[pod.type - 'A'];

            cost += value;
        }
    };

    void draw(const Node &node) {
        draw(node.pods);
    }

    void draw(const Vector<Pod> &pods) {

        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 13; ++x) {

                auto pos = Point{x, y};

                if (walls[pos]) {
                    log << '#';
                } else {

                    bool found{false};

                    for (auto &pod : pods) {

                        if (pod.position == pos) {
                            log << pod.type;
                            found = true;
                            break;
                        }
                    }

                    if (!found) {

                        log << '.';
                    }
                }
            }

            log << endl;
        }
    }

    void run() {

        const auto hallwayXs = {1, 2, 4, 6, 8, 10, 11};
        Node start;
        Node best;

        start.pods = pods;

        best.cost = std::numeric_limits<int>::max();

        PriorityQueue<Node> q;
        q.push(start);

        /* Vector<Vector<Pod>> visited; */
        /* Map<Vector<Pod>, int> ivisited; */

        Map<Vector<Pod>, int> visitedCosts;

        draw(start);

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();

            Point target;

            if (visitedCosts[node.pods] != 0 && visitedCosts[node.pods] <= node.cost) {
                continue;
            }

            /* if (visitedCosts[node.pods] == 0) { */
            /*     visited.push_back(node.pods); */
            /*     ivisited[node.pods] = visited.size() - 1; */
            /* } */

            visitedCosts[node.pods] = node.cost;

            if (best.cost < node.cost)
                continue;

            /* log << node.cost << endl; */
            /* draw(node); */
            /* system("read"); */

            if (!node.isCompleted()) {

                int pod_index = 0;
                for (auto &pod : node.pods) {
                    for (auto x : hallwayXs) {

                        if (!node.isArrived(pod)) {

                            if (node.canMoveToItsRoom(pod, target)) {
                                auto copy = node;
                                /* copy.history.push_back(ivisited[node.pods]); */
                                copy.moveTo(pod_index, target);
                                q.push(copy);
                            }

                            else if (node.canMoveToHallway(pod, x)) {
                                auto copy = node;
                                /* copy.history.push_back(ivisited[node.pods]); */
                                copy.moveTo(pod_index, {x, 1});
                                q.push(copy);
                            }
                        }
                    }
                    pod_index++;
                }
            } else {

                if (node.cost < best.cost) {
                    best = node;
                }
            }
        }

        log << "Part1: " << best.cost << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx;
    ctx.init(lines);
    ctx.run();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
