#include "../../common.h"

using Range = Pair<int, int>;
using Ticket = Vector<int>;

struct Rule {
    String name;
    Array<Range, 2> ranges;
    Set<int> forbidden;
    int maxUsage{1};

    bool validate(const int n) const {
        if ((n >= ranges[0].first && n <= ranges[0].second) || (n >= ranges[1].first && n <= ranges[1].second)) {
            return true;
        }

        return false;
    }

    void merge(const Rule &other) {
        for (auto &range : ranges) {
            for (auto &otherRange : other.ranges) {
                if (overlap(range, otherRange)) {
                    range.first = std::min(range.first, otherRange.first);
                    range.second = std::max(range.second, otherRange.second);
                }
            }
        }

        maxUsage++;
    }

    static bool overlap(const Range &a, const Range &b) {
        return a.first <= b.second && b.first <= a.second;
    }
};

using Rules = Vector<Rule>;

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto step = 0;

    Rules rules;
    Ticket myTicket;
    Vector<Ticket> tickets;

    for (const auto &line : lines) {
        if (line == "") {
            step++;
            continue;
        }
        switch (step) {
            case 0: {
                rules.push_back({});
                auto &rule = rules.back();
                auto parts = splitString(line, ':');
                rule.name = parts[0];
                char c;
                String str;
                InputStringStream iss{parts[1]};
                iss >> rule.ranges[0].first >> c >> rule.ranges[0].second >> str >> rule.ranges[1].first >> c >>
                    rule.ranges[1].second;
            } break;
            case 2: {
                myTicket = splitString<int>(line, ',');
            } break;
            case 4: {
                tickets.push_back(splitString<int>(line, ','));
            } break;
            default:
                step++;
        }
    }

    Vector<Ticket> validTickets;

    int sum{0};

    {
        for (const auto &ticket : tickets) {

            auto tvalid = true;
            for (auto n : ticket) {
                auto valid = false;
                for (auto &rule : rules) {
                    if (rule.validate(n)) {
                        valid = true;
                        break;
                    }
                }

                if (!valid) {
                    sum += n;
                    tvalid = false;
                }
            }

            if (tvalid) {
                validTickets.push_back(ticket);
            }
        }
    }

    log << "Part1: " << sum << endl;

    validTickets.push_back(myTicket);

    auto ticketSize = tickets[0].size();

    {
        for (auto &rule : rules) {
            for (int i{0}; i < ticketSize; ++i) {
                bool valid{true};
                for (const auto &ticket : validTickets) {
                    if (!rule.validate(ticket[i])) {
                        valid = false;
                    }
                }
                if (!valid) {
                    rule.forbidden.insert(i);
                }
            }
        }

        std::sort(rules.begin(), rules.end(), [](auto a, auto b) { return a.forbidden.size() > b.forbidden.size(); });
    }

    struct BfsNode {
        Map<int, int> usedRules;
        Map<int, int> indexToRuleMap;
        Set<int> usedIndices;

        bool operator<(const BfsNode &other) const {
            return usedIndices.size() < other.usedIndices.size();
        }
    };

    PriorityQueue<BfsNode> q;
    BfsNode startNode;
    q.push(startNode);
    BfsNode best;
    Map<Map<int, int>, bool> visited;

    while (!q.empty()) {
        auto node = q.top();
        q.pop();

        if (node.usedIndices.size() == ticketSize) {
            best = node;
            break;
        }

        for (int r{0}; r < rules.size(); ++r) {
            auto &rule = rules[r];

            if (node.usedRules[r] < rule.maxUsage) {

                for (int i{0}; i < ticketSize; ++i) {

                    if (rule.forbidden.find(i) == rule.forbidden.end() &&
                        node.usedIndices.find(i) == node.usedIndices.end()) {
                        auto copy = node;
                        copy.usedRules[r]++;
                        copy.usedIndices.insert(i);
                        copy.indexToRuleMap[i] = r;
                        q.push(copy);
                    }
                }
            }
        }
    }

    ull result{1};

    for (auto kv : best.indexToRuleMap) {
        if (rules[kv.second].name.find("departure") != std::string::npos) {
            result *= myTicket[kv.first];
        }
    }

    log << "Part2: " << result << endl;
}

int main() {
    process("sample.txt");
    process("sample2.txt");
    process("input.txt");
    return 0;
}
