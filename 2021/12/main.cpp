#include "../../common.h"

struct Cave {
    String name;
    Vector<int> connections;
    bool small;
};

static bool isSmallCave(const String &name) {
    return std::all_of(name.begin(), name.end(), [](unsigned char c) { return std::islower(c); });
}

struct Node {
    SmallVector<int, 16> path;
    bool didTheTrick{false};

    inline int count(const int id) const {
        auto count = std::count_if(path.begin(), path.end(), [id](auto &other) { return other == id; });
        return count;
    }
};

struct Context {
    Vector<Cave> caves;

    void parse(const String &line) {
        auto [a, b] = splitNString<String, 2>(line, '-');

        addConnection(a, b);
        addConnection(b, a);
    }

    void addConnection(const String &a, const String &b) {
        auto a_id = getCaveId(a);
        auto b_id = getCaveId(b);
        if (b != "start") {
            caves[a_id].connections.push_back(b_id);
        }
    }

    int getCaveId(const String &name) {
        for (int i = 0; i < caves.size(); ++i) {
            if (caves[i].name == name) {
                return i;
            }
        }

        caves.push_back({name});
        caves.back().small = isSmallCave(name);
        return caves.size() - 1;
    }

    template <int part> void compute() {
        Vector<Node> solutions;
        std::queue<Node> q;
        q.push(Node{{getCaveId("start")}});

        auto end_id = getCaveId("end");

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            auto &last_cave_id = node.path.back();

            if (last_cave_id != end_id) {
                const auto &cave = caves[last_cave_id];

                for (const auto &connection : cave.connections) {
                    auto &next_cave = caves[connection];

                    if (!next_cave.small) {
                        auto copy = node;
                        copy.path.push_back(connection);
                        q.push(copy);
                    } else {
                        auto count = node.count(connection);
                        auto condition = (part == 1 || node.didTheTrick) ? count < 1 : count < 2;

                        if (condition) {
                            auto copy = node;
                            copy.path.push_back(connection);
                            if (count == 1) {
                                copy.didTheTrick = true;
                            }
                            q.push(copy);
                        }
                    }
                }
            } else {
                solutions.push_back(node);
            }
        }

        log << "Part " << part << ": " << solutions.size() << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context context;

    for (auto &line : lines) {
        context.parse(line);
    }

    context.compute<1>();
    context.compute<2>();
}

int main() {
    process("sample.txt");
    process("sample2.txt");
    process("sample3.txt");
    process("input.txt");
    return 0;
}
