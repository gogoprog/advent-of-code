#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto r = 0;

    auto items = splitString(lines[0], ',');

    for (int noun = 0; noun < 99; noun++) {
        for (int verb = 0; verb < 99; verb++) {
            Vector<int> values;

            for (int i = 0; i < items.size(); i++) {
                auto item = items[i];
                InputStringStream iss{item};
                int a;
                iss >> a;
                values.push_back(a);
            }

            values[1] = noun;
            values[2] = verb;

            int i{0};
            while (i < values.size()) {

                auto v = values[i];
                if (v == 1 || v == 2) {

                    auto a = values[i + 1];
                    auto b = values[i + 2];
                    auto c = values[i + 3];

                    auto va = values[a];
                    auto vb = values[b];

                    if (v == 1) {

                        int s = va + vb;
                        values[c] = s;

                    } else {
                        int s = va * vb;
                        values[c] = s;
                    }
                    i += 4;
                } else {
                    if (v == 99) {
                        i = 100000;
                    } else {

                        i++;
                        i %= values.size();
                    }
                }
            }

            r = values[0];

            if (r == 19690720) {
                log << 100 * noun + verb << endl;
            }
        }
    }
}

int main() {
    process("input.txt");
    return 0;
}
