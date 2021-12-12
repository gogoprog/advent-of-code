#include "../../common.h"

struct Cave {
    String name;
    Vector<String> connections;
};

static bool isSmallCave(const String &name) {
    return std::all_of(name.begin(), name.end(), [](unsigned char c) { return std::islower(c); });
}

struct Node {
    Vector<String> names;
    bool didTheTrick{false};

    bool hasVisited(const String &name) const {
        return std::find(names.begin(), names.end(), name) != names.end();
    }

    int count(const String &name) const {
        auto count = std::count_if(names.begin(), names.end(), [name](auto &other) { return other == name; });
        return count;
    }

    String toString() const {
        String result;

        for (auto &name : names) {
            result += name;
            result += "-";
        }
        return result;
    }

    template <int part> void visit(const String &name) {
        auto count = std::count_if(names.begin(), names.end(), [name](auto &other) { return other == name; });
    }
};

struct Context {
    Map<String, String> connections;
    Map<String, Cave> caves;

    void parse(const String &line) {
        auto [a, b] = splitNString<String, 2>(line, '-');

        if (b != "start") {
            caves[a].connections.push_back(b);
        }

        if (a != "start") {
            caves[b].connections.push_back(a);
        }
    }

    template <int part> void compute() {
        Vector<Node> solutions;
        std::queue<Node> q;
        q.push(Node{{"start"}});

        while (!q.empty()) {
            const auto node = q.front();
            q.pop();

            auto &name = node.names.back();

            if (name != "end") {
                const auto &cave = caves[name];

                for (const auto &connection : cave.connections) {

                    if (!isSmallCave(connection)) {
                        auto copy = node;
                        copy.names.push_back(connection);
                        q.push(copy);
                    } else {
                        auto count = node.count(connection);
                        auto condition = (part == 1 || node.didTheTrick) ? count < 1 : count < 2;

                        if (condition) {
                            auto copy = node;
                            copy.names.push_back(connection);
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
