#include "../../common.h"

struct Pod {
    char type;
    Point position;

    bool isInHallway() const {
        return position.y == 1;
    }

    bool operator<(const Pod &other) const {

        if (type == other.type) {
            return position < other.position;
        }
        return type < other.type;
    }
};

template <int depth> struct Context {
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

            for (int d = 0; d < depth; d++) {
                auto t = getPodTypeAtPosition({roomx, 2 + d});

                if (t && t != pod.type) {
                    return false;
                }
            }

            return true;
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

            for (int y = pod.position.y - 1; y >= 2; y--) {
                auto t = getPodTypeAtPosition({pod.position.x, y});

                if (t) {
                    return false;
                }
            }

            return true;
        }

        bool canMoveToItsRoom(const Pod &pod, Point &target) const {
            constexpr int roomXs[4] = {3, 5, 7, 9};

            auto roomx = roomXs[pod.type - 'A'];

            if (pod.position.y == 1) {

                if (pod.position.x == roomx)
                    return false;

                if (!canCrossTo(pod, roomx)) {
                    return false;
                }

                bool found = false;

                for (int y = 2; y < 2 + depth; y++) {
                    auto t = getPodTypeAtPosition({roomx, y});
                    if (!t) {
                        found = true;
                        target = {roomx, y};
                    } else {
                        if (t != pod.type) {
                            found = false;
                            break;
                        }
                    }
                }

                return found;
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

                for (int c = 0; c < depth; ++c) {
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

        for (int y = 0; y < 3 + depth; ++y) {
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

        Vector<Vector<Pod>> visited;
        Map<Vector<Pod>, int> ivisited;

        Map<Vector<Pod>, int> visitedCosts;

        while (!q.empty()) {
            const auto node = q.top();
            q.pop();

            Point target;

            if (visitedCosts[node.pods] != 0 && visitedCosts[node.pods] <= node.cost) {
                continue;
            }

            visitedCosts[node.pods] = node.cost;

            if (best.cost < node.cost)
                continue;

            if (!node.isCompleted()) {

                int pod_index = 0;
                for (auto &pod : node.pods) {
                    for (auto x : hallwayXs) {

                        if (!node.isArrived(pod)) {

                            if (node.canMoveToItsRoom(pod, target)) {
                                auto copy = node;
                                copy.moveTo(pod_index, target);
                                q.push(copy);
                            }

                            else if (node.canMoveToHallway(pod, x)) {
                                auto copy = node;
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

        log << "Result: " << best.cost << endl;
    }
};

template <int depth> void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context<depth> ctx;
    ctx.init(lines);
    ctx.run();
}

int main() {
    process<2>("sample.txt");
    process<2>("input.txt");
    process<4>("sample2.txt");
    process<4>("input2.txt");
    return 0;
}
