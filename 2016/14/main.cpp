#include "../../common.h"
#include <openssl/md5.h>

String md5(const String input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char *)input.c_str(), input.length(), (unsigned char *)&digest);
    char mdString[33];
    for (int i = 0; i < 16; i++)
        sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);

    return mdString;
}

struct Context {
    String salt;
    int index;

    static char findTriplet(const String input) {
        int len = input.length();

        for (int i{0}; i < len - 2; ++i) {
            if (input[i] == input[i + 1] && input[i] == input[i + 2]) {
                return input[i];
            }
        }

        return 0;
    }

    static bool hasFive(const char target, const String input) {
        int len = input.length();

        for (int i{0}; i < len - 4; ++i) {
            char c = input[i];
            if (c == target) {
                if (c == input[i + 1] && c == input[i + 2] && c == input[i + 3] && c == input[i + 4]) {
                    return true;
                }
            }
        }

        return false;
    }

    void run(const bool part2 = false) {
        index = 0;
        Map<int, char> triplets;
        Vector<int> toremove;
        int count{0};

        while (count < 64) {
            String str = salt + std::to_string(index);
            str = md5(str);
            if (part2) {
                for (int i{0}; i < 2016; ++i) {
                    str = md5(str);
                }
            }

            for (auto &kv : triplets) {
                if (index < kv.first + 1000) {
                    if (hasFive(kv.second, str)) {
                        toremove.push_back(kv.first);
                        ++count;
                        if (count == 64) {
                            log << "index for 64th key: " << kv.first << endl;
                        }
                    }
                } else {
                    toremove.push_back(kv.first);
                }
            }

            for (auto i : toremove) {
                triplets.erase(i);
            }

            toremove.resize(0);

            char t = findTriplet(str);

            if (t) {
                /* log << index << " " << t << endl; */
                triplets[index] = t;
            }

            ++index;
        }
    }
};

void process(const String input) {
    log << "Processing " << input << endl;
    Context ctx{input};
    ctx.run();
    ctx.run(true);
}

int main() {
    process("abc");
    process("ahsbgdzn");
    return 0;
}
