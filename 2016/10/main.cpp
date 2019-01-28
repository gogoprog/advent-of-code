#include "../../common.h"

struct Bot {
    Vector<int> values;

    int lowTarget;
    int highTarget;
    bool lowTargetOutput{false};
    bool highTargetOutput{false};
};

struct Giving {
    int value;
    int to;
};

struct Context {
    int cmpHighTarget;
    int cmpLowTarget;
    Map<int, Bot> bots;
    Map<int, int> outputs;

    Vector<Giving> initialGivings;

    void parse(String line) {
        InputStringStream iss{line};
        String first;
        String str;
        iss >> first;
        if (first == "value") {
            int a, b;

            iss >> a >> str >> str >> str >> b;
            initialGivings.push_back({a, b});
        } else if (first == "bot") {
            String lowName, highName;
            int i;
            iss >> i;
            auto &bot = bots[i];
            iss >> str >> str >> str >> lowName >> bot.lowTarget;
            iss >> str >> str >> str >> highName >> bot.highTarget;

            bot.lowTargetOutput = (lowName == "output");
            bot.highTargetOutput = (highName == "output");

            /* log << i << " gives to " << bot.lowTarget << " and " << bot.highTarget << endl; */
        }
    }

    void process() {
        auto givings = initialGivings;

        while (givings.size() > 0) {
            /* log << givings.size() << endl; */
            for (auto &giving : givings) {
                auto &bot = bots[giving.to];

                bot.values.push_back(giving.value);

                /* log << giving.to << " receives " << giving.value << endl; */
            }

            givings.clear();

            for (auto &kv : bots) {
                int i = kv.first;
                auto &bot = kv.second;

                if (bot.values.size() == 2) {
                    std::sort(bot.values.begin(), bot.values.end());
                    auto low = bot.values[0];
                    auto high = bot.values[1];

                    if (high == cmpHighTarget && low == cmpLowTarget) {
                        log << "part1: " << i << endl;
                    }

                    if (bot.lowTargetOutput) {
                        outputs[bot.lowTarget] = low;
                    } else {
                        givings.push_back({low, bot.lowTarget});
                    }
                    if (bot.highTargetOutput) {
                        outputs[bot.highTarget] = high;
                    } else {
                        givings.push_back({high, bot.highTarget});
                    }

                    bot.values.clear();
                }
            }
        }

        int result = outputs[0] * outputs[1] * outputs[2];
        log << "part2: " << result << endl;
    }
};

void process(const String filename, const int a, const int b) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx{a, b};

    for (auto line : lines) {
        ctx.parse(line);
    }

    ctx.process();
}

int main() {
    process("sample.txt", 5, 2);
    process("input.txt", 61, 17);
    return 0;
}
