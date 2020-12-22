#include "../../common.h"

struct Entry {
    Set<String> ingredients;
    Set<String> allergens;
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    Vector<Entry> entries;

    for (auto &line : lines) {
        entries.push_back({});
        auto &entry = entries.back();
        auto parts = splitString(line, '(');
        Vector<String> ingredients;
        Vector<String> allergens;
        ingredients = splitString(parts[0], ' ');
        parts[1].resize(parts[1].length() - 1);
        allergens = splitString(parts[1], ',');
        allergens[0].erase(0, 9);
        for (auto &allergen : allergens) {
            if (allergen[0] == ' ') {
                allergen.erase(0, 1);
            }
        }

        for (auto ing : ingredients) {
            entry.ingredients.insert(ing);
        }
        for (auto all : allergens) {
            entry.allergens.insert(all);
        }
    }

    Map<String, Vector<Set<String>>> allergenToIngredientsMap;

    Map<String, bool> containMap;

    for (auto &entry : entries) {
        for (auto allergen : entry.allergens) {

            Set<String> set;
            for (auto ing : entry.ingredients) {
                set.insert(ing);

                containMap[ing] = false;
            }
            allergenToIngredientsMap[allergen].push_back(set);
        }
    }

    Map<String, Set<String>> possibilities;

    for (auto &kv : allergenToIngredientsMap) {
        if (kv.second.size() > 0) {
            auto set = kv.second[0];

            for (auto &set2 : kv.second) {
                Set<String> result;
                std::set_intersection(set.begin(), set.end(), set2.begin(), set2.end(),
                                      std::inserter(result, result.begin()));
                set = result;
            }

            for (auto ing : set) {
                containMap[ing] = true;
            }

            possibilities[kv.first] = set;
        }
    }

    Set<String> cannotContainSet;

    for (auto &kv : containMap) {
        if (kv.second == 0) {
            cannotContainSet.insert(kv.first);
        }
    }

    auto r{0};

    for (auto &entry : entries) {

        for (auto &ing : cannotContainSet) {
            if (entry.ingredients.find(ing) != entry.ingredients.end()) {
                r++;
            }
        }
    }

    log << "Part1: " << r << endl;

    {

        Vector<Pair<String, String>> solutions;

        while (solutions.size() < possibilities.size()) {
            for (auto &kv : possibilities) {
                if (kv.second.size() == 1) {
                    auto ing = *(kv.second.begin());
                    solutions.push_back({kv.first, ing});

                    for (auto &kv : possibilities) {
                        kv.second.erase(ing);
                    }
                    break;
                }
            }
        }

        std::sort(solutions.begin(), solutions.end(), [](auto a, auto b) { return a.first < b.first; });

        log << "Part2: ";
        for(auto & sol : solutions) {
            log << sol.second << ",";
        }

        log << "\b " << endl;
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
