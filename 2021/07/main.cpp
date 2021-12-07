#include "../../common.h"
#include <limits>
#include <numeric>
#include <ranges>

namespace rs = std::ranges;
namespace rv = std::ranges::views;

struct Crab {
    int position;
    ull count;
};

struct Context {
    Vector<Crab> crabs;
    int min{std::numeric_limits<int>::max()};
    int max{std::numeric_limits<int>::min()};

    void add(const int value, const ull count = 1) {
        for (auto &crab : crabs) {
            if (crab.position == value) {
                crab.count += count;
                return;
            }
        }

        crabs.push_back({value, count});

        min = std::min(value, min);
        max = std::max(value, max);
    }

    void compute() {
        auto compute_cost = [this](auto position) {
            return std::accumulate(crabs.begin(), crabs.end(), 0ull, [position](auto res, auto crab) {
                return res + std::abs(position - crab.position) * crab.count;
            });
        };

        auto v = rs::iota_view(min, max) | rv::transform(compute_cost);
        auto r = rs::min(v);

        log << "Part1: " << r << endl;
    }

    void compute2() {
        auto compute_cost = [this](auto position) {
            return std::accumulate(crabs.begin(), crabs.end(), 0ull, [position](auto res, auto crab) {
                ull delta = std::abs(position - crab.position);
                ull value = delta * (delta + 1) / 2;
                return res + value * crab.count;
            });
        };

        auto v = rs::iota_view(min, max) | rv::transform(compute_cost);
        auto r = rs::min(v);

        log << "Part2: " << r << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto positions = splitString<int>(lines[0], ',');

    Context context;

    for (auto pos : positions) {
        context.add(pos, 1);
    }

    context.compute();
    context.compute2();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
