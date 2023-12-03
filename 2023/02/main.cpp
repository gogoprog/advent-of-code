#include "../../common.h"

using Grab = Map<char, int>;
auto colors = Vector<char>{'r', 'g', 'b'};
auto maxs = Vector<int>{12, 13, 14};

struct Game {
    int id;
    Vector<Grab> grabs;

    Grab minimumGrab;

    bool isValid() {

        for (auto &grab : grabs) {

            for (int i = 0; i < 3; i++) {

                if (grab[colors[i]] > maxs[i]) {
                    return false;
                }
            }
        }

        return true;
    }

    int compute() {
        minimumGrab = grabs[0];

        for (auto &grab : grabs) {
            for (auto c : colors) {
                minimumGrab[c] = std::max(grab[c], minimumGrab[c]);
            }
        }

        auto result = 1;

        for (auto c : colors) {
            result *= minimumGrab[c];
        }

        return result;
    }
};

struct Context {
    void parse(const StringView content) {
        auto toGame = [&](auto range) {
            auto line = getStringView(range);
            InputStringStream iss{String(line)};
            String str;
            char ch;
            Game game;

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

            return game;
        };

        auto view = rs::split_view(content, '\n') | rv::transform(toGame);

        auto part1view = view | rv::filter([](auto game) { return game.isValid(); }) |
                         rv::transform([](auto game) { return game.id; });
        auto part1 = rs::fold_left(part1view, 0, std::plus());

        log << "Part1: " << part1 << endl;

        auto part2view = view | rv::transform([](auto game) { return game.compute(); });
        auto part2 = rs::fold_left(part2view, 0, std::plus());

        log << "Part2: " << part2 << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    {
        Context context;
        context.parse(content);
    }
}

int main() {
    /* process("sample.txt"); */
    process("input.txt");
    return 0;
}
