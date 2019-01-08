#include "../../common.h"

using Group = Set<int>;
using Config = Vector<Group>;

inline bool collides(const Group &ga, const Group &gb) {
    for (auto a : ga) {
        for (auto b : gb) {
            if (a == b) {
                return true;
            }
        }
    }

    return false;
}

struct Context {
    int separateCount;
    Vector<int> packages;
    int groupValue;
    Set<Group> possibleGroups;
    Set<Config> configs;
    Set<Group> firstGroups;

    void run() {
        int total = std::accumulate(packages.begin(), packages.end(), 0);
        groupValue = total / separateCount;

        process(packages, {}, 0, 0);

        log << "Possible groups: " << possibleGroups.size() << endl;

        Vector<Group> possibleGroupsVector;
        for (auto &group : possibleGroups) {
            possibleGroupsVector.push_back(group);
        }

        std::sort(possibleGroupsVector.begin(), possibleGroupsVector.end(), [](const auto &a, const auto &b) {
            if (a.size() < b.size()) {
                return true;
            } else if (a.size() > b.size()) {
                return false;
            } else {
                ull pa = std::accumulate(a.begin(), a.end(), 1.0, std::multiplies<>());
                ull pb = std::accumulate(b.begin(), b.end(), 1.0, std::multiplies<>());

                return pa < pb;
            }
        });

        part1(possibleGroupsVector);
    }

    void process(const Vector<int> &inpackages, const Group &ingroup, const int total, const int start) {
        for (int i{start}; i < inpackages.size(); ++i) {
            auto p = inpackages[i];
            if (ingroup.find(p) != ingroup.end())
                continue;
            if (total + p <= groupValue) {
                auto copied_group = ingroup;
                copied_group.insert(p);

                if (total + p == groupValue) {
                    possibleGroups.insert(copied_group);
                    break;
                } else {
                    process(inpackages, copied_group, total + p, i);
                }
            } else {
                break;
            }
        }
    }

    void part1(const Vector<Group> &ingroups) {
        for (int i{0}; i < ingroups.size(); ++i) {
            const auto &group = ingroups[i];
            log << "testing:" << group << endl;

            Config config{group};
            Vector<int> indices{i};

            for (int j{0}; j < ingroups.size(); ++j) {
                if (std::find(indices.begin(), indices.end(), j) == indices.end()) {
                    const auto &other_group = ingroups[j];
                    bool ok =
                        std::all_of(config.begin(), config.end(), [&](auto &g) { return !collides(other_group, g); });
                    if (ok) {
                        config.push_back(other_group);

                        if (config.size() == separateCount) {

                            ull qe = std::accumulate(group.begin(), group.end(), 1.0, std::multiplies<>());
                            log << "qe = " << qe << endl;
                            return;
                        }
                    }
                }
            }
        }
    }
};

void process(const String filename, const int s) {
    log << "\nProcessing " << filename << " with " << s << endl;
    auto lines = getFileLines(filename);
    Context ctx{s};

    for (auto &line : lines) {
        InputStringStream iss{line};
        int i;
        iss >> i;
        ctx.packages.push_back(i);
    }

    ctx.run();
}

int main() {
    process("sample.txt", 3);
    process("input.txt", 3);

    process("sample.txt", 4);
    process("input.txt", 4);
    return 0;
}
