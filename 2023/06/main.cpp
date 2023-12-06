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
        auto first = *((lines | rv::take(1)).begin());
        auto second = *(std::next((lines | rv::take(2)).begin()));

        auto split = rs::split_string_view(first, ':');
        auto numbers = *(std::next((split | rv::take(2)).begin()));

        auto split2 = rs::split_string_view(second, ':');
        auto numbers2 = *(std::next((split2 | rv::take(2)).begin()));

        auto times = numbers | rv::split_string_view(' ') | rv::filter_empty | rv::to_int;
        auto distances = numbers2 | rv::split_string_view(' ') | rv::filter_empty | rv::to_int;

        auto compute = rv::transform([](auto entry) {
            auto result = 0;
            auto time = std::get<0>(entry);
            auto distance = std::get<1>(entry);

            auto b = -time;
            auto c = distance;

            int speed = (-b - std::sqrt(b * b - 4 * c)) / 2;
            int speed2 = (-b + std::sqrt(b * b - 4 * c)) / 2;

            if (speed2 * (time - speed2) == distance) {
                speed2 -= 1;
            }

            return speed2 - speed;
        });

        auto view = rs::zip_view(times, distances) | compute;

        auto result = rs::fold_left(view, 1, std::multiplies<int>());
        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto first = *((lines | rv::take(1)).begin());
        auto second = *(std::next((lines | rv::take(2)).begin()));

        auto split = rs::split_string_view(first, ':');
        auto numbers = *(std::next((split | rv::take(2)).begin()));

        auto split2 = rs::split_string_view(second, ':');
        auto numbers2 = *(std::next((split2 | rv::take(2)).begin()));

        auto vtime = numbers | rv::filter([](auto c) { return c != ' '; });
        auto vdist = numbers2 | rv::filter([](auto c) { return c != ' '; });

        String strtime;
        String strdist;

        std::ranges::copy(vtime, std::back_inserter(strtime));
        std::ranges::copy(vdist, std::back_inserter(strdist));

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
    auto lines = rs::split_string_view(getFileContent(filename), '\n');
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
