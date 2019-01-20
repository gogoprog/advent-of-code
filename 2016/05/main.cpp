#include "../../common.h"

#include "md5.h"

inline String hash(const String input) {
    return Chocobo1::MD5().addData(input.c_str(), input.length()).finalize().toString();
}

void process(const String doorId) {
    log << "Processing " << doorId << endl;

    {
        int found{0};
        int i{0};
        String pwd;

        while (found < 8) {
            auto h = hash(doorId + std::to_string(i));
            if (h.substr(0, 5) == "00000") {
                ++found;
                pwd += h[5];
            }

            ++i;
        }

        log << "part1: " << pwd << endl;
    }

    {
        int found{0};
        int i{0};
        String pwd{"????????"};

        while (found < 8) {
            auto h = hash(doorId + std::to_string(i));
            if (h.substr(0, 5) == "00000") {
                char p = h[5];
                if (p >= '0' && p <= '9') {
                    int pos = p - '0';
                    if (pwd[pos] == '?') {

                        ++found;
                        pwd[pos] = h[6];
                    }
                }
            }

            ++i;
        }

        log << "part2: " << pwd << endl;
    }
}

int main() {
    process("abc");
    process("cxdnnyjw");
    return 0;
}
