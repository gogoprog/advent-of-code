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

    bool operator==(const Player &other) const = default;
};

using Players = Array<Player, 2>;

struct State {
    Players players;
    int turn;
    bool finished{false};

    bool operator==(const State &other) const = default;
};

struct Context {
    Array<Player, 2> players;

    void init(const int p1, const int p2) {
        players[0].position = p1 - 1;
        players[1].position = p2 - 1;
        players[0].score = 0;
        players[1].score = 0;
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

    void run2() {
        Dictionary<State, ull> games;

        games[{players, 0}]++;

        bool playing{true};

        while (playing) {

            playing = false;

            auto games_copy = games;

            log << games_copy.getSize() << endl;

            for (int i{0}; i < games_copy.getSize(); ++i) {
                const auto &state = games_copy.getKeyFromIndex(i);
                const auto count = games_copy.getValueFromIndex(i);

                if (state.finished == false && count > 0) {

                    games[state] -= count;

                    for (int i = 1; i <= 3; ++i) {
                        for (int j = 1; j <= 3; ++j) {
                            for (int k = 1; k <= 3; ++k) {

                                auto copy = state;
                                int roll = i + j + k;

                                copy.players[copy.turn].advance(roll);

                                if (copy.players[copy.turn].score >= 21) {
                                    copy.finished = true;
                                }

                                copy.turn = 1 - copy.turn;

                                games[copy] += count;

                                playing = true;
                            }
                        }
                    }
                }
            }
        }

        ull p1wins{0};
        ull p2wins{0};

        for (int i{0}; i < games.getSize(); ++i) {
            const auto &state = games.getKeyFromIndex(i);
            const auto count = games.getValueFromIndex(i);

            if (state.players[0].score >= 21) {
                p1wins += count;
            } else {
                p2wins += count;
            }
        }

        log << "Part2: " << endl;
        log << p1wins << endl;
        log << p2wins << endl;
    }
};

void process(const String filename, const int p1, const int p2) {
    log << "Processing " << filename << endl;

    Context ctx;
    ctx.init(p1, p2);
    ctx.run();
    ctx.init(p1, p2);
    ctx.run2();
}

int main() {
    process("sample.txt", 4, 8);
    process("input.txt", 3, 4);
    return 0;
}
