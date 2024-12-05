#include "../../common_fast.h"

struct Context {

    using Rule = Pair<Int64, Int64>;

    void process(auto content) {
        auto result{0};

        auto to_prod = [](auto line) { return line | rv::split_sv(',') | rv::to_ints; };

        auto to_rule = [](auto line) {
            auto a = line | rv::split_sv('|') | rv::get0 | rv::to_int;
            auto b = line | rv::split_sv('|') | rv::get1 | rv::to_int;

            return Rule{a, b};
        };

        auto prod_value = [](auto v) {
            auto len = std::distance(v.begin(), v.end());
            auto mid = std::next(v.begin(), int(len / 2));
            return *mid;
        };

        auto parts = content | rv::split_sv("\n\n");
        auto rules_v = parts | rv::get0 | rv::split_sv('\n') | rv::transform(to_rule);

        auto is_valid = [&](auto a, auto b) { return rs::contains(rules_v, Rule{a, b}); };

        auto is_valid_prod = [&](auto v) { return std::is_sorted(v.begin(), v.end(), is_valid); };
        auto is_invalid_prod = [&](auto v) { return !std::is_sorted(v.begin(), v.end(), is_valid); };

        auto valid_prod_v = parts | rv::get1 | rv::split_sv('\n') | rv::transform(to_prod) | rv::filter(is_valid_prod) |
                            rv::transform(prod_value);

        auto prod_value2 = [&](auto prod) {
            Vector<int> v(prod.begin(), prod.end());
            std::sort(v.begin(), v.end(), is_valid);
            return prod_value(v);
        };

        auto invalid_prod_v = parts | rv::get1 | rv::split_sv('\n') | rv::transform(to_prod) |
                              rv::filter(is_invalid_prod) | rv::transform(prod_value2);

        result = rs::fold_left(valid_prod_v, 0, std::plus());

        log << "Part1: " << result << endl;

        result = rs::fold_left(invalid_prod_v, 0, std::plus());

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context context;
        context.process(content);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
