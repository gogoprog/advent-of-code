#include "../../common.h"

struct Player {
    List<int> cards;

    int score() const {

        int res{0};
        auto len = cards.size();

        for (auto card : cards) {
            res += len * card;
            --len;
        }

        return res;
    }

    bool operator<(const Player &other) const {
        return cards < other.cards;
    }
};

struct Game {
    Array<Player, 2> players;

    bool operator<(const Game &other) const {
        return players < other.players;
    }

    void process() {
        while (!players[0].cards.empty() && !players[1].cards.empty()) {

            auto max = std::max_element(players.begin(), players.end(),
                                        [](auto &a, auto &b) { return a.cards.front() < b.cards.front(); });

            auto &winner = players[max - players.begin()];
            auto &loser = players[1 - (max - players.begin())];

            winner.cards.push_back(winner.cards.front());
            winner.cards.push_back(loser.cards.front());

            winner.cards.pop_front();
            loser.cards.pop_front();
        }

        int winnerIdx;
        if (players[0].cards.empty()) {
            winnerIdx = 1;
        } else {
            winnerIdx = 0;
        }

        log << "Part1: " << players[winnerIdx].score() << endl;
    }

    void process2() {
        Map<Game, bool> history;
        int winnerIdx = process3();
        log << "Part2: " << players[winnerIdx].score() << endl;
    }

    int process3() {
        int round{1};
        Map<Game, bool> history;

        while (!players[0].cards.empty() && !players[1].cards.empty()) {
            if (history[*this]) {
                return 0;
            }

            history[*this] = true;

            auto isSubgame = std::all_of(players.begin(), players.end(),
                                         [](auto &player) { return player.cards.front() <= player.cards.size() - 1; });

            if (isSubgame) {
                Game subgame;
                int topcards[2];
                for (int i{0}; i < 2; ++i) {
                    topcards[i] = players[i].cards.front();
                    players[i].cards.pop_front();

                    int j{0};
                    for (auto card : players[i].cards) {
                        subgame.players[i].cards.push_back(card);
                        j++;
                        if (j == topcards[i]) {
                            break;
                        }
                    }
                }

                int winnerIdx = subgame.process3();
                auto &winner = players[winnerIdx];
                winner.cards.push_back(topcards[winnerIdx]);
                winner.cards.push_back(topcards[1 - winnerIdx]);
            } else {

                auto max = std::max_element(players.begin(), players.end(),
                                            [](auto &a, auto &b) { return a.cards.front() < b.cards.front(); });

                auto &winner = players[max - players.begin()];
                auto &loser = players[1 - (max - players.begin())];

                winner.cards.push_back(winner.cards.front());
                winner.cards.push_back(loser.cards.front());

                winner.cards.pop_front();
                loser.cards.pop_front();
            }

            ++round;
        }

        if (players[0].cards.empty()) {
            return 1;
        } else {
            return 0;
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    int currentPlayer{-1};

    Game game{};

    for (auto &line : lines) {
        InputStringStream iss{line};
        String str;

        iss >> str;

        if (str.find("Player") != std::string::npos) {
            ++currentPlayer;
        } else {
            if (str != "") {
                game.players[currentPlayer].cards.push_back(std::stoi(str));
            }
        }
    }

    auto game2 = game;

    game.process();

    { game2.process2(); }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
