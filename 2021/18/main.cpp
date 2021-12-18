#include "../../common.h"

struct SFN {
    int value{-1};
    SFN *left{0};
    SFN *right{0};

    void print() {
        if (value != -1) {
            log << value;
        } else {
            log << "[";
            left->print();
            log << ",";
            right->print();
            log << "]";
        }
    }

    int magnitude() const {

        if (value != -1) {
            return value;
        } else {
            return 3 * left->magnitude() + 2 * right->magnitude();
        }
    }

    void split() {
        if (value >= 10) {
            auto a = std::floor(value / 2.0);
            auto b = std::ceil(value / 2.0);

            value = -1;

            left = new SFN(a);
            right = new SFN(b);
        }
    }

    SFN *findExploder(const int depth = 0) {

        if (depth == 4 && left && right) {
            return this;
        }

        if (left) {
            auto r = left->findExploder(depth + 1);

            if (r)
                return r;
        }

        if (right) {
            auto r = right->findExploder(depth + 1);

            if (r)
                return r;
        }

        return nullptr;
    }

    void computeRegularList(Vector<SFN *> &result) {

        if (value != -1) {
            result.push_back(this);
        } else {

            left->computeRegularList(result);
            right->computeRegularList(result);
        }
    }

    void explode(SFN *exploder) {
        Vector<SFN *> list;

        exploder->value = 0;

        computeRegularList(list);

        auto it = std::find(list.begin(), list.end(), exploder);
        auto before = std::prev(it, 1);
        auto after = std::next(it, 1);

        if (before >= list.begin()) {
            (*before)->value += exploder->left->value;
        }
        if (after != list.end()) {
            (*after)->value += exploder->right->value;
        }

        exploder->left = exploder->right = nullptr;
    }

    void reduce() {
        bool working{true};
        Vector<SFN *> list;

        while (working) {
            working = false;

            while (true) {
                auto exploder = findExploder(0);

                if (exploder) {
                    working = true;
                    explode(exploder);
                }

                else {
                    break;
                }
            }

            list.resize(0);
            computeRegularList(list);

            for (auto sfn : list) {
                if (sfn->value >= 10) {
                    sfn->split();
                    working = true;
                    break;
                }
            }
        }
    }

    static SFN *add(SFN *left, SFN *right) {
        SFN *result;
        result = new SFN();

        result->left = left;
        result->right = right;

        result->reduce();

        return result;
    }

    static SFN *parse(const String line) {
        Vector<SFN *> stack;
        SFN *current{nullptr};
        SFN *result;

        for (auto c : line) {
            switch (c) {
                case '[': {
                    auto new_sfn = new SFN();

                    if (current) {
                        if (current->left == nullptr) {
                            current->left = new_sfn;
                        } else {
                            current->right = new_sfn;
                        }
                    } else {
                        result = new_sfn;
                    }

                    stack.push_back(new_sfn);
                    current = stack.back();
                } break;

                case ']': {
                    stack.pop_back();
                    current = stack.back();
                } break;

                case ',': {
                } break;

                default:
                    auto value = c - '0';
                    auto new_sfn = new SFN{value};

                    if (current->left == nullptr) {
                        current->left = new_sfn;
                    } else {
                        current->right = new_sfn;
                    }

                    break;
            }
        }

        return result;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    auto sfn = SFN::parse(lines[0]);

    for (auto line : lines | rv::drop(1)) {
        auto sfn2 = SFN::parse(line);
        sfn = SFN::add(sfn, sfn2);
    }

    log << "Part1: " << sfn->magnitude() << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
