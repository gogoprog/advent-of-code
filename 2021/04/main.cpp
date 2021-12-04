#include "../../common.h"

struct Board {
    Map<Point, int> values;
    Map<Point, bool> marks;

    void init(const String &line, const int y) {
        InputStringStream iss{line};

        for (int x = 0; x < 5; ++x) {
            int v;
            iss >> v;

            values[{x, y}] = v;
        }
    }

    void print() {
        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                log << values[{x, y}] << " ";
            }

            log << endl;
        }
    }

    void mark(const int x, const int y) {
        marks[{x, y}] = true;
    }

    void mark(const int value) {
        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                if (values[{x, y}] == value) {
                    mark(x, y);
                }
            }
        }
    }

    bool hasWon() {
        for (int y = 0; y < 5; ++y) {
            bool result = true;
            for (int x = 0; x < 5; ++x) {
                if (!marks[{x, y}]) {

                    result = false;
                    break;
                }
            }

            if (result) {
                return true;
            }
        }

        for (int x = 0; x < 5; ++x) {
            bool result = true;
            for (int y = 0; y < 5; ++y) {
                if (!marks[{x, y}]) {

                    result = false;
                    break;
                }
            }

            if (result) {
                return true;
            }
        }

        return false;
    }

    int score() {
        auto result = 0;

        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                if (!marks[{x, y}]) {
                    result += values[{x, y}];
                }
            }
        }

        return result;
    }

    bool completed{false};
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    Vector<Board> boards;

    auto picks = lines[0];

    auto l = 2;
    while (l < lines.size()) {

        boards.push_back({});
        auto &board = boards.back();

        for (int i = 0; i < 5; ++i) {
            auto &line = lines[l];
            board.init(line, i);
            ++l;
        }

        ++l;
    }

    boards[0].print();

    auto ipicks = splitString<int>(picks, ',');
    int last_pick;

    Board *first{nullptr};
    Board *last{nullptr};

    for (auto pick : ipicks) {
        bool over = true;
        for (auto &board : boards) {
            board.mark(pick);

            if (board.hasWon()) {
                board.completed = true;
                if (!first) {
                    first = &board;
                    auto score = board.score();
                    log << "Part1: " << score * pick << endl;
                }
                if (std::all_of(boards.begin(), boards.end(), [](auto &b) { return b.completed; })) {
                    auto score = board.score();
                    log << "Part2: " << score * pick << endl;
                    return;
                }
            }
        }
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
