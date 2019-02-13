#include "../../common.h"

struct Context {
    String initial;
    int diskLength;

    static String step(const String a) {
        auto b = a;

        std::reverse(b.begin(), b.end());

        for (char &c : b) {
            switch (c) {
                case '0':
                    c = '1';
                    break;
                case '1':
                    c = '0';
                    break;
            }
        }

        return a + '0' + b;
    }

    static String checksum(const String input) {
        String r{"un"};
        String in{input};

        while (!(r.size() & 1)) {
            r.clear();

            auto len = in.length() / 2;

            for (int i{0}; i < len; ++i) {
                if (in[i * 2] == in[i * 2 + 1]) {
                    r += '1';
                } else {
                    r += '0';
                }
            }

            in = r;
        }

        return r;
    }

    void run() {
        String current{initial};

        while (current.length() < diskLength) {
            current = step(current);
        }

        current.erase(current.begin() + diskLength, current.end());

        String r = checksum(current);
        log << "checksum is " << r << endl;
    }
};

void process(const String filename, const int len) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx{filename, len};
    ctx.run();
}

int main() {
    process("10000", 20);
    process("10010000000110000", 272);
    process("10010000000110000", 35651584);
    return 0;
}
