#include "../../common_fast.h"

using Int = uint64_t;

struct Rating {
    Map<char, Int> values;
    Int compute() const {
        Int result = 0;
        for (auto &kv : values) {
            result += kv.second;
        }
        return result;
    }
};

struct Rule {
    char part;
    char op;
    Int value;
    String target;

    void print() {
        log << part << op << value << ":" << target << endl;
    }
};

enum Result { ACCEPT, REJECT, SEND };

struct Workflow {
    String name;
    Vector<Rule> rules;

    void print() {
        log << name << endl;
        for (auto &r : rules) {
            log << "    ";
            r.print();
        }
    }

    String compute(Rating &rating) {
        for (auto &rule : rules) {
            if (rule.op) {
                auto val = rating.values[rule.part];

                switch (rule.op) {
                    case '<':
                        if (val < rule.value) {
                            return rule.target;
                        }
                        break;
                    case '>':
                        if (val > rule.value) {
                            return rule.target;
                        }
                        break;
                }
            } else {
                return rule.target;
            }
        }
    }

    Result compute(Rating &rating, String &target) {
        String ret = compute(rating);
        if (ret[0] == 'A') {
            return ACCEPT;
        } else if (ret[0] == 'R') {
            return REJECT;
        } else {
            target = ret;
            return SEND;
        }
    }
};

struct Context {
    Vector<Workflow> workflows;
    Map<String, Workflow> workflowsMap;
    Vector<Rating> ratings;

    void parse(auto content) {
        auto view = content | rv::split_sv("\n\n");

        auto view_workflows = view | rv::get0 | rv::split_sv('\n');
        auto view_ratings = view | rv::get1 | rv::split_sv('\n');

        auto to_workflows = rv::transform([](auto line) {
            Workflow wf;

            wf.name = line | rv::split_sv('{') | rv::get0;

            auto to_rules = rv::transform([](auto line) {
                Rule rule;

                if (line.find('<') != StringView::npos || line.find('>') != StringView::npos) {
                    rule.part = line[0];
                    rule.op = line[1];

                    auto parts = line | rv::split_sv(':');
                    auto left = parts | rv::get0;
                    auto right = parts | rv::get1;

                    std::from_chars(left.data() + 2, left.data() + left.size(), rule.value);

                    rule.target = right;
                } else {
                    rule.part = 0;
                    rule.op = 0;
                    rule.value = 0;
                    rule.target = line;
                }

                return rule;
            });

            auto part2 = line | rv::split_sv('{') | rv::get1;

            auto rules_v = part2.substr(0, part2.size() - 1) | rv::split_sv(',') | to_rules;

            rs::copy(rules_v, std::back_inserter(wf.rules));

            return wf;
        });

        auto to_ratings = rv::transform([](auto line) {
            Rating rating;

            auto subline = line.substr(1, line.size() - 2);

            for (auto part : subline | rv::split_sv(',')) {
                Int value;
                char name = part[0];

                std::from_chars(part.data() + 2, part.data() + part.size(), value);

                rating.values[name] = value;
            }

            return rating;
        });

        rs::copy(view_workflows | to_workflows, std::back_inserter(workflows));
        rs::copy(view_ratings | to_ratings, std::back_inserter(ratings));

        for (auto &wf : workflows) {
            /* wf.print(); */
            workflowsMap[wf.name] = wf;
        }
    }

    void part1() {
        auto result{0};

        for (auto &rating : ratings) {

            auto current = &workflowsMap["in"];

            auto cont = true;

            while (cont) {
                String target;
                auto r = current->compute(rating, target);
                switch (r) {
                    case ACCEPT:
                        result += rating.compute();
                        cont = false;
                        break;
                    case REJECT:
                        cont = false;
                        break;
                    case SEND:
                        current = &workflowsMap[target];
                        break;
                }
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        Int result{0};

        struct Node {
            String flow;
            Map<char, Pair<Int, Int>> values;
        };

        Node start;
        start.flow = "in";
        start.values['x'] = {0, 4000};
        start.values['m'] = {0, 4000};
        start.values['a'] = {0, 4000};
        start.values['s'] = {0, 4000};

        Queue<Node> q;

        q.push(start);

        auto accept = [&](auto &node) {
            Int r = 1;

            for (auto &kv : node.values) {

                r *= (kv.second.second - kv.second.first);
            }

            result += r;
        };

        while (!q.empty()) {
            auto node = q.front();
            q.pop();

            auto flow = &workflowsMap[node.flow];

            for (auto &rule : flow->rules) {

                if (rule.op) {
                    auto &val = node.values[rule.part];
                    auto copy = node;
                    auto &new_val = copy.values[rule.part];
                    switch (rule.op) {
                        case '<':
                            new_val.first = std::max(new_val.first, rule.value);
                            val.second = std::min(val.second, rule.value);
                            break;
                        case '>':
                            new_val.second = std::min(new_val.second, rule.value);
                            val.first = std::max(val.first, rule.value);
                            break;
                    }

                    if (rule.target[0] == 'A') {
                        accept(node);
                    } else if (rule.target[0] == 'R') {
                    } else {
                        copy.flow = rule.target;
                        q.push(copy);
                    }
                } else {
                    if (rule.target[0] == 'A') {
                        accept(node);
                    } else if (rule.target[0] == 'R') {
                    } else {
                        node.flow = rule.target;
                        q.push(node);
                    }
                }
            }
        }

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
