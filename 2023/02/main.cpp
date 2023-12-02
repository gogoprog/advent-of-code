#include "../../common.h"

using Grab = Map<char, int>;

struct Game {
    int id;
    Vector<Grab> grabs;

    Grab minimumGrab;

    bool isValid() {

        for (auto &grab : grabs) {
            if (grab['r'] > 12 || grab['g'] > 13 || grab['b'] > 14) {
                return false;
            }
        }

        return true;
    }

    int compute() {
        minimumGrab = grabs[0];

        for (auto &grab : grabs) {
            minimumGrab['r'] = std::max(grab['r'], minimumGrab['r']);
            minimumGrab['g'] = std::max(grab['g'], minimumGrab['g']);
            minimumGrab['b'] = std::max(grab['b'], minimumGrab['b']);
        }

        return minimumGrab['r'] * minimumGrab['g'] * minimumGrab['b'];
    }
};

struct Context {

    Vector<Game> games;

    void parse(const Strings &lines) {
        String str;
        char ch;
        for (auto &line : lines) {

            auto &game = games.emplace_back();

            InputStringStream iss(line);

            iss >> str >> game.id >> ch;

            std::getline(iss, str);

            auto entries = splitString(str, ';');

            for (auto entry : entries) {

                auto str2 = splitString(entry, ',');

                auto &grab = game.grabs.emplace_back();

                for (auto str3 : str2) {
                    InputStringStream iss2(str3);
                    int count;
                    char letter;

                    iss2 >> count >> letter;

                    grab[letter] = count;
                }
            }
        }
    }

    void part1() {
        auto result{0};

        for (auto &game : games) {
            if (game.isValid()) {
                result += game.id;
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2() {
        auto result{0};

        for (auto &game : games) {
            result += game.compute();
        }

        log << "Part2: " << result << endl;
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
