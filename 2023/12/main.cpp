#include "../../common_fast.h"

using Ints = Vector<int>;

bool can_match(StringView row, const int group) {

    int i = 0;
    int current = 0;

    while (row[i] != '.') {
        current++;

        if (current == group) {

            return row[i + 1] != '#';
        }

        ++i;
    }

    return false;
}

using Result = Map<int, int>;

static Map<int, Map<String, Result>> cache;

void doit2(StringView row, int offset, const Ints &groups, int group_index, int count, int64_t &final_count) {

    if (group_index >= groups.size()) {

        if (offset < row.size()) {

            auto rest = row.substr(offset);

            if (rest.find('#') == StringView::npos) {
                final_count = final_count + count;
            }
        } else {

            final_count = final_count + count;
        }

        return;
    }

    if (offset >= row.size()) {
        return;
    }

    auto group = *(groups.begin() + group_index);

    while (row[offset] == '.') {
        offset++;
    }

    Result todos;

    int i;
    for (i = offset; i < row.size(); i++) {

        if (row[i - 1] != '#') {

            auto str = row.substr(i, group + 1);

            auto match = can_match(str, group);

            if (match) {

                auto next = i + group + 1 - offset;

                if (row[next + offset] == '.') {
                    next++;
                }

                todos[next]++;
            }
            if (row[i] == '#')
                break;
        }
    }

    for (auto &kv : todos) {
        doit2(row, kv.first + offset, groups, group_index + 1, count * kv.second, final_count);
    }
};

struct Solver {
    struct Node {
        int position;
        int groupIndex;
        int count;

        Vector<Node *> nextNodes;
    };

    Array<Node, 128> nodes;
    int nodesCount{0};
    Map<int, Map<int, Node *>> nodesMap;

    Vector<Node *> goodNodes;

    Node *createNode(int position, int groupIndex, int count) {
        auto node = &nodes[nodesCount];
        nodesCount++;

        node->position = position;
        node->groupIndex = groupIndex;
        node->count = count;

        nodesMap[groupIndex][position] = node;

        return node;
    }

    Node *getNode(int position, int group_index) {

        if (nodesMap[group_index].contains(position)) {
            return nodesMap[group_index][position];
        }

        return createNode(position, group_index, 0);
    }

    void solve(StringView row, const Ints groups) {

        log << "starting " << row << endl;
        Queue<Node *> q;

        auto first = createNode(0, 0, 1);

        q.push(first);

        while (!q.empty()) {
            auto node = q.front();
            q.pop();

            auto offset = node->position;
            auto group_index = node->groupIndex;

            auto group = groups[group_index];

            log << "node with group " << group_index << " offset " << offset << endl;

            if (group_index >= groups.size()) {

                if (offset < row.size()) {

                    auto rest = row.substr(offset);

                    if (rest.find('#') == StringView::npos) {
                        goodNodes.push_back(node);
                    }
                } else {
                    goodNodes.push_back(node);
                }

                continue;
            }

            while (row[offset] == '.') {
                offset++;
            }

            for (int i = offset; i < row.size(); i++) {

                if (row[i - 1] != '#') {

                    auto str = row.substr(i, group + 1);

                    auto match = can_match(str, group);

                    if (match) {

                        auto next = i + group + 1;

                        if (row[next] == '.') {
                            next++;
                        }

                        auto next_node = getNode(next, group_index + 1);

                        next_node->count++;

                        if (next_node->count == 1) {
                            node->nextNodes.push_back(next_node);
                            q.push(next_node);
                        }
                        /* q.push(new_node); */
                    }
                    if (row[i] == '#')
                        break;
                }
            }
        }

        for (auto node : goodNodes) {

            log << "good node with group:" << node->groupIndex << " position:" << node->position
                << " count: " << node->count << endl;
        }
    }

    int compute(Node *node) {
        auto result = node->count;

        for (auto nextNode : node->nextNodes) {

            result += node->count * compute(nextNode);
        }

        return result;
    }

    int computeResult() {

        /* return goodNodes.size(); */
        return compute(getNode(0, 0));
    }
};

struct Context {

    void part1(auto lines) {
        auto result{0};

        auto compute_arrangements = [&](auto line) {
            auto row = line | rv::split_sv(' ') | rv::get0;
            auto groups_v = line | rv::split_sv(' ') | rv::get1 | rv::split_sv(',') | rv::to_ints;

            Vector<int> groups;
            rs::copy(groups_v, std::back_inserter(groups));

            int64_t count = 0;
            String final_row = String(row);

            final_row = '.' + final_row + '.';

            doit2(final_row, 0, groups, 0, 1, count);

            Solver solver;
            solver.solve(final_row, groups);

            if (count != solver.computeResult()) {
                log << "failed: " << solver.computeResult() << " != " << count << "\n";
            }

            return count;
        };

        for (auto line : lines) {

            auto r = compute_arrangements(line);

            log << line << " : " << r << endl;

            result += r;
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        int64_t result{0};

        auto compute_arrangements = [&](auto line) {
            auto row = line | rv::split_sv(' ') | rv::get0;
            auto groups_v = line | rv::split_sv(' ') | rv::get1 | rv::split_sv(',') | rv::to_ints;

            Vector<int> groups;
            rs::copy(groups_v, std::back_inserter(groups));

            String final_row;
            Ints final_groups;
            bool first = true;

            for (int i = 0; i < 5; ++i) {
                if (!first) {
                    final_row += '?';
                }

                final_row += row;

                first = false;

                final_groups.insert(final_groups.end(), groups.begin(), groups.end());
            }

            final_row = "." + final_row + '.';

            int64_t count = 0;
            doit2(final_row, 0, final_groups, 0, 1, count);

            return count;
        };

        for (auto line : lines) {

            auto r = compute_arrangements(line);

            log << line << " : " << r << endl;

            result += r;
        }
        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = rs::split_string_view(getFileContent(filename), '\n');
    {
        Context context;
        context.part1(lines);
        /* context.part2(lines); */
    }
}

int main() {
    /* process("sample2.txt"); */
    process("sample.txt");
    /* process("input.txt"); */
    return 0;
}
