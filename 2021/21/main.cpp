#include "../../common.h"

struct Dice {
    int value{99};

    int next() {
        value += 1;
        value %= 100;
        return value + 1;
    }

    int next3() {
        int r = 0;
        r += next();
        r += next();
        r += next();
        return r;
    }
};

struct Player {
    int position;
    int score{0};

    void advance(const int value) {
        position += value;
        position %= 10;

        score += getPosition();
    }

    int getPosition() {
        return position + 1;
    }
};

struct Context {
    Array<Player, 2> players;

    void init(const int p1, const int p2) {
        players[0].position = p1 - 1;
        players[1].position = p2 - 1;
    }

    void run() {
        Dice dice;
        int i{0};

        int p{0};

        bool finished{false};
        Player *loser = nullptr;

        while (loser == nullptr) {
            int roll = dice.next3();
            auto &player = players[p];

            player.advance(roll);

            /* log << "Player " << (p+1) << " is " << player.getPosition() << " | " << player.score << endl; */

            ++i;
            p = 1 - p;

            if (player.score >= 1000) {
                finished = true;
                loser = &players[p];
            }
        }


        log << "Part1: " << loser->score * i * 3 << endl;
    }
};

void process(const String filename, const int p1, const int p2) {
    log << "Processing " << filename << endl;

    Context ctx;
    ctx.init(p1, p2);
    ctx.run();
}

int main() {
    process("sample.txt", 4, 8);
    process("input.txt", 3, 4);
    return 0;
}
