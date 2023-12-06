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

using Range = Array<ull, 3>;

struct Recipe {
    StringView source;
    StringView destination;
    Vector<Range> ranges;

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

        auto parts = content | rv::split_sv("\n\n");
        auto view = parts | rv::get0 | rv::split_string_view(':') | rv::get1 | rv::split_sv(' ') | rv::filter_empty |
                    rv::to_ints;
        rs::copy(view, std::back_inserter(seeds));

        auto block_view = parts | rv::drop(1);

        auto to_recipe = [](auto block) {
            Recipe result;
            auto lines = block | rv::split_sv('\n');

            auto to_range = [](auto line) {
                Range result;
                auto ints = line | rv::split_sv(' ') | rv::filter_empty | rv::to_ints;

                rs::copy(ints, result.begin());

                return result;
            };

            auto view = lines | rv::drop(1) | rv::transform(to_range);
            rs::copy(view, std::back_inserter(result.ranges));

            return result;
        };

        auto recipe_view = block_view | rv::transform(to_recipe);
        rs::copy(recipe_view, std::back_inserter(recipes));
    }

    void part1() {
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

    void part2() {
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
    auto lines = getFileContent(filename);

    {
        Context context;
        context.parse(lines);
        context.part1();
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
