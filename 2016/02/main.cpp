#include "../../common.h"

struct Context {
    int x{0}, y{2};
    String code;

    Grid<bool> valids;
    Grid<char> digits;

    Context() { 
        set(2, 0, '1');

        set(1, 1, '2');
        set(2, 1, '3');
        set(3, 1, '4');

        set(0, 2, '5');
        set(1, 2, '6');
        set(2, 2, '7');
        set(3, 2, '8');
        set(4, 2, '9');

        set(1, 3, 'A');
        set(2, 3, 'B');
        set(3, 3, 'C');

        set(2, 4, 'D');
    }

    void set(int i, int j, char v) {
        valids[i][j] = true;
        digits[i][j] = v;
    }

    void move(char letter) {

        switch (letter) {
        case 'U':
            if (valids[x][y-1])
                y--;
            break;
        case 'D':
            if (valids[x][y+1])
                y++;
            break;
        case 'L':
            if (valids[x-1][y])
                x--;
            break;
        case 'R':
            if (valids[x+1][y])
                x++;
            break;
        }

    }

    void press() {
        code += digits[x][y];
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    Context ctx{};

    for (auto line : lines) {
        for (auto c : line) {
            ctx.move(c);
        }
        ctx.press();
    }

    log << "code: " << ctx.code << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
