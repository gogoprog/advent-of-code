#include "../../common.h"

struct Rule {
    int quantity;
    String name;
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto r = 0;

    String name, name2, tmp, rest;

    Map<String, Vector<Rule>> rules;

    for (auto &line : lines) {
        InputStringStream iss{line};

        iss >> name >> name2 >> tmp >> tmp;

        std::getline(iss, rest);

        Vector<String> parts = splitString(rest, ',');

        auto &this_rules = rules[name + ' ' + name2];

        for (auto &part : parts) {
            InputStringStream iss{part};
            int count;

            iss >> tmp;

            if (tmp == "no") {
                break;
            } else {
                count = std::stoi(tmp);

                iss >> name >> name2;

                this_rules.push_back({count, name + ' ' + name2});
            }
        }
    }

    std::function<bool(const String, const Vector<Rule> &)> find_bag;

    find_bag = [&](auto name, auto this_rules) {
        for (auto &item : this_rules) {
            if (item.name == name) {
                return true;
            } else {
                if (find_bag(name, rules[item.name])) {
                    return true;
                }
            }
        }
        return false;
    };

    for (auto &kv : rules) {
        if (find_bag("shiny gold", kv.second)) {
            r++;
        }
    }

    log << "Part1: " << r << endl;

    std::function<int(const Vector<Rule> &)> count_bags;

    count_bags = [&](auto this_rules) {
        int r{0};
        for (auto &item : this_rules) {
            r += item.quantity;
            r += item.quantity * count_bags(rules[item.name]);
        }
        return r;
    };

    log << "Part2: " << count_bags(rules["shiny gold"]) << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
