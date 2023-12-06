#include "../../common_fast.h"

struct Card {
    int id;
    Vector<int> winningNumbers;
    Vector<int> numbers;

    int compute() {
        auto result = 0;
        auto score = 1;

        for (auto n : numbers) {

            if (std::find(winningNumbers.begin(), winningNumbers.end(), n) != winningNumbers.end()) {

                if (!result) {
                    result = 1;
                } else {
                    result *= 2;
                }
            }
        }

        return result;
    }

    int wins() {
        auto result = 0;

        for (auto n : numbers) {
            if (std::find(winningNumbers.begin(), winningNumbers.end(), n) != winningNumbers.end()) {
                result++;
            }
        }

        return result;
    }
};

struct Context {

    auto getView(const StringView lines) {
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

        auto toCard = [&](auto line) {
            Card card;

            card.id = line | rv::split_sv(':') | rv::get0 | rv::split(' ') | rv::filter_empty | rv::get1 | rv::to_int;

            auto wins = line | rv::split_sv(':') | rv::get1 | rv::split_sv('|') | rv::get0 | rv::split_sv(' ') |
                        rv::filter_empty | rv::to_ints;
            rs::copy(wins, std::back_inserter(card.winningNumbers));

            auto nums = line | rv::split_sv(':') | rv::get1 | rv::split_sv('|') | rv::get1 | rv::split_sv(' ') |
                        rv::filter_empty | rv::to_ints;
            rs::copy(nums, std::back_inserter(card.numbers));

            return card;
        };

        auto view = rs::split_string_view(lines, '\n') | rv::transform(toCard);
        return view;
    }

    void part1(auto lines) {
        auto result =
            rs::fold_left(getView(lines) | rv::transform([](auto card) { return card.compute(); }), 0, std::plus());

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto cards = getView(lines);

        auto len = std::distance(cards.begin(), cards.end());

        Vector<int> copies(len, 0);

        for (auto card : cards) {
            auto repeats = copies[card.id - 1] + 1;
            auto wins = card.wins();

            for (int w = 0; w < wins; w++) {
                copies[card.id + w + 1 - 1] += repeats;
            }
        }

        auto result = std::accumulate(copies.begin(), copies.end(), 0);

        result += len;

        log << "Part2: " << result << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileContent(filename.c_str());
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
