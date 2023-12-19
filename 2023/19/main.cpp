#include "../../common_fast.h"

using Int = int;

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
                auto ret = current->compute(rating);

                if (ret[0] == 'A') {

                    result += rating.compute();

                    cont = false;

                } else if (ret[0] == 'R') {
                    cont = false;
                } else {
                    current = &workflowsMap[ret];
                }
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0};

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
