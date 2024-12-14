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

Vector<Transform> allTransforms;

Vector3 transformVector3(const Vector3 &input, const Transform &transform) {
    auto &perm = transform.permutation;
    auto &dir = transform.direction;
    return {input[perm[0]] * dir[0], input[perm[1]] * dir[1], input[perm[2]] * dir[2]};
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
                    scanners.back().beacons.push_back(parsePoint3(line));
                }
            }
        }
    }

    bool findOverlap(const Scanner &scanner_a, const Scanner &scanner_b, const Transform &tt, Vector3 &delta_result,
                     Transform &transform_result) {

        for (auto &this_transform : allTransforms) {
            for (auto &beacon_a : scanner_a.beacons) {
                auto transformed_beacon = transformVector3(beacon_a, this_transform);

                for (auto &beacon_b : scanner_b.beacons) {
                    auto transformed_beacon_b = transformVector3(beacon_b, tt);
                    auto delta = transformed_beacon_b - transformed_beacon;

                    auto matching = 0;

                    for (auto &other_beacon_a : scanner_a.beacons) {
                        auto other_transformed_beacon = transformVector3(other_beacon_a, this_transform);
                        for (auto &other_beacon_b : scanner_b.beacons) {
                            if (other_beacon_b != beacon_b && other_beacon_a != beacon_a) {
                                auto other_transformed_beacon_b = transformVector3(other_beacon_b, tt);
                                auto other_delta = other_transformed_beacon_b - other_transformed_beacon;

                                if (delta == other_delta) {
                                    matching++;
                                }
                            }
                        }
                    }

                    if (matching >= 11) {
                        transform_result = this_transform;
                        delta_result = delta;

                        return true;
                    }
                }
            }
        }

        return false;
    }

    void run() {
        struct Solution {
            Map<int, Vector3> positions;
            Map<int, Transform> transforms;
        };

        Map<int, Vector<int>> no_overlap;

        Transform transform;
        Vector3 delta;

        Solution solution;
        solution.positions[0] = {0, 0, 0};
        solution.transforms[0] = {axisPermutations[0], axisDirections[0]};

        while (solution.positions.size() < scanners.size()) {

            bool found{false};

            for (auto &kv : solution.positions) {
                auto &scanner_a = scanners[kv.first];
                auto &scanner_a_position = kv.second;
                auto &scanner_a_transform = solution.transforms[kv.first];
                auto &scanner_a_forbidden = no_overlap[scanner_a.id];

                for (auto &scanner_b : scanners) {

                    if (solution.positions.find(scanner_b.id) == solution.positions.end() &&
                        rs::find(scanner_a_forbidden, scanner_b.id) == scanner_a_forbidden.end()) {
                        auto found = findOverlap(scanner_b, scanner_a, scanner_a_transform, delta, transform);

                        if (found) {
                            Vector3 result = delta;

                            result = result + scanner_a_position;

                            solution.positions[scanner_b.id] = result;
                            solution.transforms[scanner_b.id] = transform;
                            found = true;
                            break;
                        } else {
                            no_overlap[scanner_a.id].push_back(scanner_b.id);
                            no_overlap[scanner_b.id].push_back(scanner_a.id);
                        }
                    }
                }
            }
        }

        {
            Set<Vector3> all_beacons;

            for (auto &scanner : scanners) {
                auto sid = scanner.id;
                auto &transform = solution.transforms[sid];
                auto &position = solution.positions[sid];

                for (auto &beacon : scanner.beacons) {
                    auto result = transformVector3(beacon, transform);
                    result = result + position;

                    all_beacons.insert(result);
                }
            }

            log << "Part1: " << all_beacons.size() << endl;
        }

        {

            ull best = 0;

            for (auto &kv : solution.positions) {
                for (auto &kv2 : solution.positions) {
                    auto delta = kv2.second - kv.second;
                    ull manhattan = std::abs(delta.x) + std::abs(delta.y) + std::abs(delta.z);

                    best = std::max(best, manhattan);
                }
            }

            log << "Part2: " << best << endl;
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
    for (auto &permutation : axisPermutations) {
        for (auto &direction : axisDirections) {
            allTransforms.push_back(Transform{permutation, direction});
        }
    }

    process("sample.txt");
    process("input.txt");
    return 0;
}
