#include "../../common.h"

struct Node {
    int value;
    Vector<Node *> children;

    bool isValue{false};

    void setValue(const int v) {
        value = v;
        isValue = true;
    }

    void convert() {
        isValue = false;
        auto new_node = new Node;
        new_node->setValue(value);
        children.push_back(new_node);
    }

    void print() const {
        if (isValue) {
            log << value;
        } else {
            log << "[";

            bool first{true};

            for (auto child : children) {
                if (!first) {
                    log << ',';
                }
                child->print();
                first = false;
            }

            log << "]";
        }
    }
};

std::ostream &operator<<(std::ostream &out, const Node &v) {
    v.print();
    return out;
}

Node *parse(String str) {

    Vector<Node *> stack;

    for (int i = 0; i < str.size(); ++i) {
        auto c = str[i];

        if (c == '[') {
            auto new_node = new Node;

            if (!stack.empty()) {
                stack.back()->children.push_back(new_node);
            }

            stack.push_back(new_node);
        } else if (c == ']') {
            stack.pop_back();
        } else if (c == ',') {
        } else {
            String temp;
            while (std::isdigit(c)) {
                temp += c;
                i++;
                c = str[i];
            }
            i--;

            auto new_node = new Node;
            new_node->setValue(convert<String, int>(temp));
            stack.back()->children.push_back(new_node);
        }
    }

    return stack[0];
}

enum { INCORRECT = -1, UNKNOWN = 0, CORRECT = 1 };

int isOrdered(const Node &left, const Node &right) {
    /* log << left << " vs " << right << endl; */

    if (left.isValue && right.isValue) {
        if (left.value < right.value) {
            return CORRECT;
        } else if (left.value == right.value) {
            return UNKNOWN;
        } else {
            return INCORRECT;
        }
    } else if (!left.isValue && !right.isValue) {

        int i = 0;

        while (true) {
            if (i >= left.children.size() && i >= right.children.size()) {
                return UNKNOWN;
            }

            if (i >= left.children.size()) {
                return CORRECT;
            } else if (i >= right.children.size()) {
                return INCORRECT;
            }

            auto cl = left.children[i];
            auto cr = right.children[i];

            auto r = isOrdered(*cl, *cr);

            if (r != UNKNOWN) {
                return r;
            }

            ++i;
        }

        return true;
    } else {
        if (left.isValue) {
            auto copy = left;
            copy.convert();
            return isOrdered(copy, right);
        } else {
            auto copy = right;
            copy.convert();
            return isOrdered(left, copy);
        }
    }

    throw "nop";
    return false;
}

bool isOrdered(String left, String right) {
    auto a = parse(left);
    auto b = parse(right);

    auto r = isOrdered(*a, *b);

    return r == CORRECT;
}

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    {
        auto total{0};

        for (int i = 0; i < lines.size(); i += 3) {

            /* log << "== Pair " << i / 3 + 1 << endl; */
            bool r = isOrdered(lines[i], lines[i + 1]);
            /* log << r << endl; */

            if (r) {
                total += i / 3 + 1;
            }
        }

        log << "Part1: " << total << endl;
    }

    {
        Vector<Node *> nodes;

        for (auto line : lines) {

            if (line != "") {
                nodes.push_back(parse(line));
            }
        }

        auto divider2 = parse("[[2]]");
        auto divider6 = parse("[[6]]");
        nodes.push_back(divider2);
        nodes.push_back(divider6);

        std::sort(nodes.begin(), nodes.end(), [](auto a, auto b) { return isOrdered(*a, *b) == CORRECT; });

        auto it2 = std::find(nodes.begin(), nodes.end(), divider2);
        auto it6 = std::find(nodes.begin(), nodes.end(), divider6);

        auto pos2 = std::distance(nodes.begin(), it2) + 1;
        auto pos6 = std::distance(nodes.begin(), it6) + 1;

        auto r = pos2 * pos6;

        log << "Part2: " << r << endl;
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
