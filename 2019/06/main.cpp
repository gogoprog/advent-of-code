#include "../../common.h"

using Orbits = Map<String, Vector<String>>;
using Parents = Map<String, String>;
using Path = Vector<String>;

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto r = 0;

    Map<String, String> parents;
    Map<String, Vector<String>> orbits;

    for (auto &line : lines) {
        auto parts = splitString(line, ')');

        orbits[parts[0]].push_back(parts[1]);
        parents[parts[1]] = parts[0];
    }

    Function<int(const String)> getOrbitCount = [&](const auto &name) {
        int r = 0;
        for (auto &child : orbits[name]) {
            r++;
            r += getOrbitCount(child);
        }

        return r;
    };


    r = std::accumulate(orbits.begin(), orbits.end(), 0,
                        [&](const auto sum, const auto &kv) { return sum + getOrbitCount(kv.first); });

    log << "part1: " << r << endl;


    Function<void(Path &, const String)> fillPath = [&](auto & path, auto from) {
        auto current = parents[from];
        path.push_back(current);

        auto it = parents.find(current);
        while (it != parents.end()) {
            path.push_back(it->second);

            it = parents.find(it->second);
        }
    };

    {
        Vector<String> path1;
        Vector<String> path2;

        fillPath(path1, "YOU");
        fillPath(path2, "SAN");

        for (int i = 0; i < path1.size(); ++i) {
            for (int j = 0; j < path2.size(); ++j) {

                if (path1[i] == path2[j]) {
                    log << "part2: " << i + j << endl;
                    return;
                }
            }
        }
    }
}

int main() {
    process("sample.txt");
    process("sample2.txt");
    process("input.txt");
    return 0;
}
