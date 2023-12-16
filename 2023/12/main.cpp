#include "../../common_fast.h"

using Ints = Vector<int>;

using Int = uint64_t;

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

struct Solver {
    struct Node {
        int position;
        int groupIndex;
        Int count;

        /* Set<Node *> nextNodes; */
        /* Set<Node *> previousNodes; */

        /*
        bool canReach(const int group_index) {
            if (group_index == groupIndex)
                return true;

            bool result = false;

            for (auto node : nextNodes) {

                result = result || node->canReach(group_index);
            }

            return result;
        }

        int compute() const {
            auto result = 1;
            for (auto node : previousNodes) {
                result += node->compute();
            }
            return result;
        }
        */

        void print() {
            log << "node with group:" << groupIndex << " position:" << position << " count:" << count << "\n";
        }
    };

    Array<Node, 4096> nodes;
    int nodesCount{0};
    Map<int, Map<int, Node *>> nodesMap;

    Set<Node *> finalNodes;
    Set<Node *> goodNodes;

    Map<int, Set<Node *>> goodNodesPerGroupIndex;

    Node *createNode(int position, int groupIndex, Int count) {
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

    /* void clean(Node *node, const int group_index) { */

    /*     auto &nodes = node->nextNodes; */
    /*     auto len = nodes.size(); */

    /*     auto cont = true; */
    /*     while (cont) { */
    /*         cont = false; */

    /*         for (auto next : nodes) { */
    /*             if (!next->canReach(group_index)) { */
    /*                 nodes.erase(next); */
    /*                 cont = true; */
    /*                 break; */
    /*             } */
    /*         } */
    /*     } */

    /*     for (auto next : nodes) { */
    /*         clean(next, group_index); */
    /*     } */
    /* } */

    /* void findGoodNodes(Node *node, int group_index) { */

    /*     if (node->canReach(group_index)) { */
    /*         goodNodes.insert(node); */

    /*         goodNodesPerGroupIndex[node->groupIndex].insert(node); */
    /*     } */

    /*     for (auto next : node->nextNodes) { */
    /*         findGoodNodes(next, group_index); */
    /*     } */
    /* } */

    void solve(StringView row, const Ints groups) {

        /* log << "starting " << row << endl; */
        Queue<Node *> q;

        auto first = createNode(0, 0, 1);

        q.push(first);

        while (!q.empty()) {

            auto node = q.front();
            q.pop();

            auto offset = node->position;
            auto group_index = node->groupIndex;

            if (group_index >= groups.size()) {

                if (offset < row.size()) {

                    auto rest = row.substr(offset);

                    if (rest.find('#') == StringView::npos) {
                        finalNodes.insert(node);
                    }
                } else {
                    finalNodes.insert(node);
                }

                continue;
            }
            if (offset >= row.size()) {
                continue;
            }

            auto group = groups[group_index];

            while (row[offset] == '.') {
                offset++;
            }

            for (int i = offset; i < row.size(); i++) {

                if (row[i - 1] != '#') {

                    auto str = row.substr(i, group + 1);

                    auto match = can_match(str, group);

                    if (match) {

                        auto next = i + group + 1;

                        while (row[next] == '.') {
                            next++;
                        }

                        auto next_node = getNode(next, group_index + 1);

                        if (next_node->count == 0) {
                            q.push(next_node);
                        }

                        next_node->count += node->count;
                        /* next_node->previousNodes.insert(node); */

                        /* node->nextNodes.insert(next_node); */
                    }
                    if (row[i] == '#')
                        break;
                }
            }
        }

        /* findGoodNodes(getNode(0, 0), groups.size()); */
        /* clean(getNode(0, 0), groups.size()); */

        /* for (auto node : goodNodes) { */

        /*     log << "good node with group:" << node->groupIndex << " position:" << node->position */
        /*         << " count: " << node->count << endl; */
        /* } */
        /* for (auto node : finalNodes) { */

        /*     log << "final node with group:" << node->groupIndex << " position:" << node->position */
        /*         << " count: " << node->count << endl; */
        /* } */
    }

    Int computeResult(const Ints groups) {

        Int result = 0;

        for (auto node : finalNodes) {
            result += node->count;
        }

        return result;
    }
};

struct Context {

    void part1(auto lines) {
        Int result{0};

        auto compute_arrangements = [&](auto line) {
            auto row = line | rv::split_sv(' ') | rv::get0;
            auto groups_v = line | rv::split_sv(' ') | rv::get1 | rv::split_sv(',') | rv::to_ints;

            Vector<int> groups;
            rs::copy(groups_v, std::back_inserter(groups));

            Int count = 0;
            String final_row = String(row);

            final_row = '.' + final_row + '.';

            /* doit2(final_row, 0, groups, 0, 1, count); */

            Solver solver;
            solver.solve(final_row, groups);

            auto r = solver.computeResult(groups);

            return r;
        };

        for (auto line : lines) {

            auto r = compute_arrangements(line);

            /* log << line << " : " << r << endl; */

            result += r;
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        Int result{0};

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

            /* Int count = 0; */
            /* doit2(final_row, 0, final_groups, 0, 1, count); */
            Solver solver;
            solver.solve(final_row, final_groups);

            auto r = solver.computeResult(final_groups);

            return r;
        };

        int i = 0;
        for (auto line : lines) {

            log << i++ << ": " << line << " :" << endl;

            auto r = compute_arrangements(line);

            log << "        -> " << r << endl;

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
        context.part2(lines);
    }
}

int main() {
    /* process("sample2.txt"); */
    process("sample.txt");
    process("input.txt");
    return 0;
}
