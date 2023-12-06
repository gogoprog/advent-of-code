#include "../../common_fast.h"

auto filterEmpty = [](auto range) {
    auto line = getStringView(range);
    return line.size() > 0;
};

auto toInt = [](auto range) {
    auto line = getStringView(range);

    int result;

    std::from_chars(line.data(), line.data() + line.size(), result);
    return result;
};

struct Context {

    void part1(auto lines) {
        auto times = lines | rv::get0 | rv::split_string_view(':') | rv::get1 | rv::split_string_view(' ') |
                     rv::filter_empty | rv::to_ints;

        auto distances = lines | rv::get1 | rv::split_string_view(':') | rv::get1 | rv::split_string_view(' ') |
                         rv::filter_empty | rv::to_ints;

        auto compute = [](auto time, auto distance) {
            auto b = -time;
            auto c = distance;

            int speed = (-b - std::sqrt(b * b - 4 * c)) / 2;
            int speed2 = (-b + std::sqrt(b * b - 4 * c)) / 2;

            if (speed2 * (time - speed2) == distance) {
                speed2 -= 1;
            }

            return speed2 - speed;
        };

        auto view = rv::zip_transform(compute, times, distances);

        auto result = rs::fold_left(view, 1, std::multiplies<int>());
        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {

        auto strtime =
            lines | rv::get0 | rv::split_string_view(':') | rv::get1 | rv::split(' ') | rv::join | rv::to_string;
        auto strdist =
            lines | rv::get1 | rv::split_string_view(':') | rv::get1 | rv::split(' ') | rv::join | rv::to_string;

        auto compute = [](auto time, auto distance) {
            auto b = -time;
            auto c = distance;

            int speed = (-b - std::sqrt(b * b - 4 * c)) / 2;
            int speed2 = (-b + std::sqrt(b * b - 4 * c)) / 2;

            return speed2 - speed;
        };

        auto result = compute(std::stol(strtime), std::stol(strdist));

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileContent(filename) | rv::split_string_view('\n');
    {
        Context context;
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
