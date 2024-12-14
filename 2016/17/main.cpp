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
    String passcode;

    void parse(String line) {
        InputStringStream iss{line};
    }

    struct Node {
        Coord coord;
        String path;
        bool operator<(const Node &other) const {
            return path.size() < other.path.size();
        }
    };

    void run() {
        Coord start{0, 0};
        Coord target{3, 3};
        Grid<int> visitedSteps;
        int minSteps{10000};
        int maxSteps{0};
        String minPath;

        std::priority_queue<Node> q;
        q.push({start, ""});

        while (!q.empty()) {
            auto node = q.top();
            q.pop();

            const auto &coord = node.coord;

            if (coord == target) {
                if (node.path.size() < minSteps) {
                    minSteps = node.path.size();
                    minPath = node.path;
                }
                if (node.path.size() > maxSteps) {
                    maxSteps = node.path.size();
                }
            } else {
                static Array<Coord, 4> dirs = {Coord{0, -1}, Coord{0, 1}, Coord{-1, 0}, Coord{1, 0}};
                static String charDirs = "UDLR";

                String hash = md5(passcode + node.path);

                for (int i{0}; i < 4; ++i) {
                    auto dir = dirs[i];
                    Coord ncoord{coord.x + dir.x, coord.y + dir.y};

                    if (ncoord.x >= 0 && ncoord.x < 4 && ncoord.y >= 0 && ncoord.y < 4) {
                        char c = hash[i];
                        if (c >= 'b' && c <= 'f') {
                            q.push({ncoord, node.path + charDirs[i]});
                        }
                    }
                }
            }
        }

        log << "minPath = " << minPath << endl;
        log << "maxSteps = " << maxSteps << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx{filename};

    for (auto line : lines) {
        ctx.parse(line);
    }

    ctx.run();
}

int main() {
    process("hijkl");
    process("ihgpwlah");
    process("kglvqrro");
    process("ulqzkmiv");
    process("qljzarfv");
    return 0;
}
