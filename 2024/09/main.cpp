#include "../../common_fast.h"

struct Context {
    static constexpr int FREE = -1;

    struct Node {
        int id;
        int size;
    };

    Vector<Node> nodes;
    int idCount;

    static Int64 checksum(const auto nodes) {
        Int64 result = 0;
        int pos = 0;

        for (const auto &node : nodes) {
            for (int i = 0; i < node.size; ++i) {

                if (node.id != FREE) {
                    result += pos * node.id;
                }

                pos++;
            }
        }

        return result;
    }

    static void draw(const auto nodes) {
        for (const auto &node : nodes) {
            for (int i = 0; i < node.size; ++i) {
                if (node.id != FREE) {
                    log << node.id;
                } else {
                    log << '.';
                }
            }
        }

        log << endl;
    }

    void parse(auto line) {
        int id = 0;

        int l = 0;
        for (auto c : line) {
            auto n = c - '0';

            if (!(l & 1)) {
                nodes.push_back({id, n});
                id++;
            } else {
                nodes.push_back({FREE, n});
            }
            ++l;
        }

        idCount = id;
    }

    void part1() {
        auto result{0_int64};

        int index = 0;
        /*

        while (hasGap()) {

            Node last = nodes.back();
            nodes.pop_back();

            while (last.id == FREE) {
                last = nodes.back();
                nodes.pop_back();
            }

            Node current = nodes[index];

            while (current.id != FREE) {
                index++;
                current = nodes[index];
            }

            nodes[index] = last;
        }

        result = checksum(nodes);
        */

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0_int64};
        auto copynodes = nodes;

        /* draw(copynodes); */

        int id = idCount - 1;
        int index = copynodes.size() - 1;

        while (id >= 0) {

            while (copynodes[index].id != id) {
                --index;
            }

            int free_index;
            bool found = false;

            for (free_index = 0; free_index <= index; free_index++) {
                if (copynodes[free_index].id == FREE && copynodes[free_index].size >= copynodes[index].size) {
                    found = true;
                    break;
                }
            }

            if (found) {
                auto left = copynodes[free_index].size - copynodes[index].size;
                copynodes[free_index] = copynodes[index];
                copynodes[index].id = FREE;

                if (left) {
                    copynodes.insert(copynodes.begin() + free_index + 1, {FREE, left});
                }
            }

            id--;
            /* draw(copynodes); */
        }

        /* draw(copynodes); */

        result = checksum(copynodes);

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    {
        Context context;
        context.parse(content);
        context.part1();
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
