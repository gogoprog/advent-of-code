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

struct Computer {
    Map<String, lli> extras;

    void reset() {
        extras.clear();
    }

    lli cost(const Reactions &reactions, const String &chem, const lli qty) {
        auto rIt = reactions.find(chem);
        if (rIt != reactions.end()) {
            lli total = 0;

            lli available = qty;
            lli reacQty = rIt->second.output.quantity;
            lli count = available / reacQty;

            if (available % reacQty != 0) {
                count++;
                extras[chem] = reacQty - available % reacQty;
            }

            for (auto &c : rIt->second.inputs) {
                lli q = c.quantity * count;
                q -= extras[c.name];

                if (q < 0) {
                    extras[c.name] = -q;
                    q = 0;
                } else {
                    extras[c.name] = 0;
                }

                if (q > 0) {
                    total += cost(reactions, c.name, q);
                }
            }

            if (total < 0) {
                assert(false);
            }
            return total;
        } else {

            return qty;
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

    {

        Computer computer;
        auto r = computer.cost(reactions, "FUEL", 1);
        log << "part1: " << r << endl;
    }

    {
        Computer computer;
        lli lower = 0, upper = 1000000000;

        while (true) {
            computer.reset();
            auto f = (lower + upper) / 2;
            auto r = computer.cost(reactions, "FUEL", f);

            if (r > 1000000000000) {
                upper = f;
            } else {
                lower = f;
            }

            if (upper - lower == 1) {

                log << "part2: " << lower << endl;
                break;
            }
        }
    }
}

int main() {
    process("sample.txt");
    process("sample2.txt");
    process("sample3.txt");
    process("sample4.txt");
    process("sample5.txt");
    process("input.txt");
    return 0;
}
