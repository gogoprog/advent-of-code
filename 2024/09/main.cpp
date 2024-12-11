#include "../../common_fast.h"

struct Context {
    static constexpr int FREE = -1;

    struct Node {
        int id;
        int size;
    };

    struct Nodes : public Vector<Node> {

        Int64 checksum() const {
            Int64 result = 0;
            int pos = 0;

            auto &nodes = *this;

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

        void draw() const {
            for (const auto &node : *this) {
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

        bool findFreeNode(int size, int start_index, int max_index, int &free_index) const {
            bool found = false;
            auto &nodes = *this;

            for (free_index = start_index; free_index <= max_index; free_index++) {
                if (nodes[free_index].id == FREE && nodes[free_index].size >= size) {
                    return true;
                }
            }

            return false;
        }

        void moveNode(int source_index, int target_index, int size) {
            auto &nodes = *this;
            auto left = nodes[target_index].size - size;

            auto left_source = nodes[source_index].size - size;

            nodes[target_index].id = nodes[source_index].id;
            nodes[target_index].size = size;

            if (!left_source) {
                nodes[source_index].id = FREE;
            } else {
                nodes[source_index].size = left_source;
                nodes.insert(nodes.begin() + source_index + 1, {FREE, size - left_source});
            }

            if (left) {
                nodes.insert(nodes.begin() + target_index + 1, {FREE, left});
            }
        }
    };

    Nodes nodes;

    int idCount;

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

        auto copynodes = nodes;
        int free_index = 0;
        int index = copynodes.size() - 1;

        while (true) {
            while (copynodes[index].id == FREE) {
                --index;
            }

            if (copynodes.findFreeNode(1, free_index, index, free_index)) {
                copynodes.moveNode(index, free_index, 1);
                ++index;
            } else {
                break;
            }
        }

        result = copynodes.checksum();

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0_int64};

        auto copynodes = nodes;
        int id = idCount - 1;
        int index = copynodes.size() - 1;
        int free_index;

        while (id >= 0) {

            while (copynodes[index].id != id) {
                --index;
            }

            if (copynodes.findFreeNode(copynodes[index].size, 0, index, free_index)) {
                copynodes.moveNode(index, free_index, copynodes[index].size);
            }

            id--;
        }

        result = copynodes.checksum();

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
