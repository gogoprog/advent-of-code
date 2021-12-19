#include "../../common.h"

constexpr Array<Vector3, 6> axisPermutations = {
    Vector3{0, 1, 2}, {1, 2, 0}, {2, 0, 1}, {0, 2, 1}, {1, 0, 2}, {2, 1, 0},
};

constexpr Array<Vector3, 8> axisDirections = {
    Vector3{1, 1, 1}, {1, -1, -1}, {-1, 1, -1}, {-1, -1, 1}, {-1, -1, -1}, {-1, 1, 1}, {1, -1, 1}, {1, 1, -1},
};

struct Transform {
    Vector3 permutation;
    Vector3 direction;
};

int getIndex(const Vector3 &input, int value) {
    for (int i = 0; i < 3; ++i) {
        if (input[i] == value) {
            return i;
        }
    }

    return -1;
}

Transform getInverted(const Transform &transform) {
    auto &p = transform.permutation;
    auto &d = transform.direction;

    auto ip = Vector3{getIndex(p, 0), getIndex(p, 1), getIndex(p, 2)};

    return {ip, {d[ip[0]], d[ip[1]], d[ip[2]]}};
}

Vector3 transformVector3(const Vector3 &input, const Vector3 &perm, const Vector3 &dir) {
    return {input[perm[0]] * dir[0], input[perm[1]] * dir[1], input[perm[2]] * dir[2]};
}

Vector3 transformVector3(const Vector3 &input, const Transform &transform) {
    return transformVector3(input, transform.permutation, transform.direction);
}

Transform getTransformed(const Transform &a, const Transform &b) {

    Vector3 tmp{1, 2, 3};

    tmp = transformVector3(tmp, a);
    tmp = transformVector3(tmp, b);

    Transform result;

    for (int i = 0; i < 3; i++) {
        auto v = std::abs(tmp[i]) - 1;
        result.permutation[i] = v;
        result.direction[v] = tmp[i] / std::abs(tmp[i]);
    }

    return result;
}

struct Scanner {
    String name;
    int id;
    Vector<Vector3> beacons;
};

struct Context {
    Vector<Scanner> scanners;

    void parse(const Vector<String> &lines) {
        for (auto &line : lines) {
            if (line != "") {
                if (line.substr(0, 3) == "---") {
                    scanners.push_back({line});
                    scanners.back().id = scanners.size() - 1;
                } else {
                    scanners.back().beacons.push_back(parseVector3(line));
                }
            }
        }
    }

    bool findOverlap(const Scanner &scanner_a, const Scanner &scanner_b, const Transform &transform,
                     Vector3 &delta_result, Transform &transform_result) {

        bool found = false;
        for (auto &permutation : axisPermutations) {
            for (auto &direction : axisDirections) {

                auto this_transform = Transform{permutation, direction};
                for (auto &beacon_a : scanner_a.beacons) {
                    auto transformed_beacon = transformVector3(beacon_a, transform);
                    transformed_beacon = transformVector3(transformed_beacon, this_transform);

                    for (auto &beacon_b : scanner_b.beacons) {
                        auto delta = beacon_b - transformed_beacon;

                        auto matching = 0;

                        for (auto &other_beacon_a : scanner_a.beacons) {
                            auto other_transformed_beacon = transformVector3(other_beacon_a, transform);
                            other_transformed_beacon = transformVector3(other_transformed_beacon, this_transform);
                            for (auto &other_beacon_b : scanner_b.beacons) {
                                if (other_beacon_b != beacon_b && other_beacon_a != beacon_a) {
                                    auto other_delta = other_beacon_b - other_transformed_beacon;

                                    if (delta == other_delta) {
                                        matching++;
                                    }
                                }
                            }
                        }

                        if (matching >= 11) {

                            if (found) {

                                if (permutation != transform_result.permutation ||
                                    direction != transform_result.direction) {
                                    log << "Hey it is different" << endl;
                                }
                            }
                            transform_result = {permutation, direction};
                            delta_result = delta;

                            return true;
                            found = true;
                        }
                    }
                }
            }
        }

        return found;

        return false;
    }

    void run() {
        struct Node {
            Map<int, Vector3> positions;
            Map<int, Transform> transforms;
        };

        Transform transform;
        Vector3 delta;

        Queue<Node> q;

        Node start;
        Node solution;
        start.positions[0] = {0, 0, 0};
        start.transforms[0] = {axisPermutations[0], axisDirections[0]};

        q.push(start);

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            if (node.positions.size() == scanners.size()) {
                solution = node;
            } else {
                auto copy = node;
                bool mustPush = false;

                for (auto &kv : node.positions) {
                    auto &scanner_a = scanners[kv.first];
                    auto scanner_a_position = kv.second;
                    auto scanner_a_transform = copy.transforms[kv.first];

                    for (auto &scanner_b : scanners) {

                        if (node.positions.find(scanner_b.id) == node.positions.end()) {
                            auto found = findOverlap(scanner_b, scanner_a, scanner_a_transform, delta, transform);

                            if (found) {
                                Vector3 result;

                                result = transformVector3(delta, (scanner_a_transform));

                                result = result + scanner_a_position;

                                log << "=> " << result << endl;

                                copy.positions[scanner_b.id] = result;

                                transform = getTransformed(transform ,getInverted(scanner_a_transform));
                                /* transform = getTransformed((scanner_a_transform), transform); */

                                copy.transforms[scanner_b.id] = transform;
                                mustPush = true;
                            }
                        }
                    }
                }

                if (mustPush) {
                    q.push(copy);
                }
            }
        }

        {
            Set<Vector3> all_beacons;

            for (auto &scanner : scanners) {
                auto sid = scanner.id;
                auto transform = solution.transforms[sid];
                auto position = solution.positions[sid];
                for (auto &beacon : scanner.beacons) {
                    auto result = transformVector3(beacon, getInverted(transform));
                    result = result + position;

                    all_beacons.insert(result);
                }
            }

            log << "Part1: " << all_beacons.size() << endl;
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx;
    ctx.parse(lines);

    ctx.run();
}

int main() {
    process("sample.txt");
    /* process("input.txt"); */
    return 0;
}
