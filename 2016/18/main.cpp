#include "../../common.h"

struct Context {
    int totalRows;
    Vector<String> rows;
    int size;

    void parse(String line) {
        rows.push_back(line);
        size = line.length();
    }

    void addLine() {
        String last = rows.back();
        String newRow = last;

        for (int i{0}; i < size; ++i) {
            char left = i > 0 ? last[i - 1] : '.';
            char center = last[i];
            char right = i < size - 1 ? last[i + 1] : '.';

            if ((left == '^' && center == '^' && right == '.') or (center == '^' && right == '^' && left == '.') or
                (left == '^' && right == '.' && center == '.') or (right == '^' && left == '.' && center == '.')) {
                newRow[i] = '^';
            } else {
                newRow[i] = '.';
            }
        }

        rows.push_back(newRow);
    }

    void run() {
        while (rows.size() < totalRows) {
            addLine();
        }

        int r{0};
        for (auto &row : rows) {
            /* log << row << endl; */
            r += std::count(row.begin(), row.end(), '.');
        }

        log << "safe tiles: " << r << endl;
    }
};

void process(const String filename, const int rows) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx{rows};
    ctx.parse(lines[0]);

    ctx.run();
}

int main() {
    process("sample.txt", 3);
    process("sample2.txt", 10);
    process("input.txt", 40);
    process("input.txt", 400000);
    return 0;
}
