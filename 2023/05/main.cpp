#include "../../common_fast.h"

auto filterEmpty = [](auto range) {
    auto line = getStringView(range);
    return line.size() > 0;
};

auto toInt = [](auto range) {
    auto line = getStringView(range);

    ull result;

    std::from_chars(line.data(), line.data() + line.size(), result);
    return result;
};

struct Recipe {
    StringView source;
    StringView destination;
    Vector<Vector<ull>> ranges;

    ull convert(ull input) {

        for (auto &range : ranges) {

            auto dest_start = *range.begin();
            auto source_start = *(range.begin() + 1);
            auto length = *(range.begin() + 2);

            if (input >= source_start && input < source_start + length) {
                return dest_start + input - source_start;
            }
        }

        return input;
    }

    ull unconvert(ull input) {

        for (auto &range : ranges) {

            auto dest_start = *range.begin();
            auto source_start = *(range.begin() + 1);
            auto length = *(range.begin() + 2);

            if (input >= dest_start && input < dest_start + length) {
                return source_start + input - dest_start;
            }
        }

        return input;
    }
};

struct Context {

    Vector<ull> seeds;
    Vector<Recipe> recipes;

    void parse(auto content) {
        auto lines = rs::split_string_view(content, '\n');

        Vector<Vector<StringView>> blocks;
        blocks.resize(1);
        auto current = 0;

        for (auto line : lines) {
            blocks[current].push_back(line);

            if (line.empty()) {
                current++;
                blocks.push_back({});
            }
        }

        {

            auto split = rs::split_string_view(blocks[0][0], ':');
            auto it = std::next(split.begin());
            std::ranges::copy(rs::split_string_view(*it, ' ') | rv::filter(filterEmpty) | rv::transform(toInt),
                              std::back_inserter(seeds));
        }

        for (int b = 1; b < blocks.size(); b++) {
            auto &block = blocks[b];

            auto split = rs::split_string_view(block[0], ' ');
            auto split2 = rs::split_string_view(*split.begin(), '-');
            auto source = *split2.begin();
            auto dest = *(std::next(std::next(split2.begin())));

            auto &recipe = recipes.emplace_back();
            recipe.destination = dest;

            for (int l = 1; l < block.size(); l++) {
                Vector<ull> ints;
                std::ranges::copy(rs::split_string_view(block[l], ' ') | rv::filter(filterEmpty) | rv::transform(toInt),
                                  std::back_inserter(ints));

                if (ints.size()) {
                    recipe.ranges.push_back(ints);
                }
            }
        }
    }

    void part1(auto lines) {
        ull min{std::numeric_limits<ull>::max()};

        for (auto seed : seeds) {

            ull value = seed;

            for (auto &recipe : recipes) {
                value = recipe.convert(value);
            }

            if (value < min) {
                min = value;
            }
        }

        log << "Part1: " << min << endl;
    }

    void part2(auto lines) {
        auto is_seed = [&](auto input) {
            for (int s = 0; s < seeds.size(); s += 2) {
                auto start = seeds[s];
                auto len = seeds[s + 1];

                if (input >= start && input < start + len) {
                    return true;
                }
            }

            return false;
        };

        auto found = false;
        ull i = 0;

        while (true) {
            ull value = i;

            for (int r = recipes.size() - 1; r >= 0; r--) {
                auto &recipe = recipes[r];
                value = recipe.unconvert(value);
            }

            if (is_seed(value)) {
                log << "Part2: " << i << endl;
                break;
            }

            ++i;
        }
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    /* auto lines = rs::split_string_view(getFileContent(filename), '\n'); */
    auto lines = getFileContent(filename);
    {
        Context context;
        context.parse(lines);
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
