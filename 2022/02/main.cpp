#include "../../common.h"

enum Shape { ROCK, PAPER, SCISSORS };

enum Outcome { LOSS, DRAW, WIN };

struct Game {
    int p1score{0};
    int p2score{0};

    void play(int p1, int p2) {

        p1score += p1 + 1;
        p2score += p2 + 1;

        if (p2 < p1) {
            p2 += 3;
        }

        auto diff = p2 - p1;

        diff %= 3;

        if (diff == 1) {
            p2score += 6;
        } else if (diff == 2) {
            p1score += 6;
        } else {
            p1score += 3;
            p2score += 3;
        }
    }

    void play2(int p1, int p2outcome) {

        int p2 = p1;

        switch (p2outcome) {
            case LOSS:
                p2 += 2;
                p1score += 6;
                break;

            case DRAW:
                p1score += 3;
                p2score += 3;
                break;
            case WIN:
                p2 += 1;
                p2score += 6;
                break;
        }

        p2 %= 3;

        p1score += p1 + 1;
        p2score += p2 + 1;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto r = 0;

    Game game;
    Game game2;

    for (auto line : lines) {
        InputStringStream iss{line};

        char p1, p2;
        iss >> p1 >> p2;

        game.play(p1 - 'A', p2 - 'X');

        game2.play2(p1 - 'A', p2 - 'X');
    }

    log << "Part1: " << game.p2score << endl;
    log << "Part2: " << game2.p2score << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
