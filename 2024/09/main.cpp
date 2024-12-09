#include "../../common_fast.h"

struct Context {
    static constexpr int FREE = -1;

    struct Node {
        Int64 id;
        int size;
    };

    Vector<Node> nodes;

    bool hasGap() const {
        bool last_is_free = false;
        for (const auto &node : nodes) {
            if (node.id == FREE) {
                last_is_free = true;
            } else {
                if (last_is_free) {
                    return true;
                }
                last_is_free = false;
            }
        }
        return false;
    }

    int firstFirstGap() const {
        bool last_is_free = false;
        int i = 0;
        for (const auto &node : nodes) {
            if (node.id == FREE) {
                last_is_free = true;
            } else {
                if (last_is_free) {
                    return i;
                }
                last_is_free = false;
            }
            ++i;
        }
        return -1;
    }

    static Int64 checksum(const auto nodes) {

        Int64 result{0};
        for (int i = 0; i < nodes.size(); ++i) {

            auto id = nodes[i].id;
            if (id != FREE) {
                result += i * nodes[i].id;
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

        int index = nodes.size() - 1;
        while (nodes[index].id == FREE) {
            --index;
        }

        int id = nodes[index].id;

        while (id >= 0) {
            int index = 0;
            for (index = 0; index < copynodes.size(); ++index) {
                if (copynodes[index].id == id) {
                    break;
                }
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

        int pos = 0;

        for (const auto &node : copynodes) {
            for (int i = 0; i < node.size; ++i) {

                if (node.id != FREE) {
                    result += pos * node.id;
                }

                pos++;
            }
        }

        /* draw(copynodes); */

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
