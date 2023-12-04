#include "../../common.h"

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

    Vector<Card> cards;

    void parse(const Strings &lines) {
        for (auto &line : lines) {
            auto &card = cards.emplace_back();
            card.id = cards.size();

            auto split = splitString(line, ':');
            auto str = split[1];

            auto parts = splitString(str, '|');

            card.winningNumbers = splitString<int>(parts[0], ' ');
            card.numbers = splitString<int>(parts[1], ' ');
        }
    }

    void part1() {
        auto result{0};

        for (auto &card : cards) {

            result += card.compute();
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0};
        auto originalCards = cards;
        Vector<Card> list{cards.begin(), cards.end()};
        Vector<Card> temp;

        for (int i = 0; i < list.size(); ++i) {

            auto &card = list[i];

            auto wins = card.wins();

            temp = {originalCards.begin() + card.id , originalCards.begin() + card.id + wins};

            /* log << "[" << card.id << "] " <<  temp.size() << endl; */

            /* log << " wins "; */
            /* for(auto t : temp) { */
            /*     log << t.id << " "; */

            /* } */
            /* log << endl; */


            list.insert(list.begin() + i + 1, temp.begin(), temp.end());

        }

        log << "Part2: " << list.size() << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    {
        Context context;
        context.parse(lines);
        context.part1();
    }
    {
        Context context;
        context.parse(lines);
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
