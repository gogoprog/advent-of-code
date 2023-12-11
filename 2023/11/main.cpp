#include "../../common_fast.h"

struct Context {
    Vector<String> lines;
    int width, height;
    Vector<Coord> galaxies;

    Vector<int> expRows;
    Vector<int> expCols;

    static void draw(Vector<String> &lines) {
        auto width = lines[0].size();
        auto height = lines.size();
        log << "drawing " << width << "x" << height << "\n";
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                log << lines[y][x];
            }
            log << endl;
        }
    }

    void parse(auto str) {
        StringViews svs;

        rs::copy(str, std::back_inserter(svs));

        for (auto line : svs) {
            lines.push_back(String(line));
        }

        /* draw(lines); */

        width = lines[0].size();
        height = lines.size();

        auto empty_col = [&](auto col) {
            for (int y = 0; y < height; y++) {
                char ch = lines[y][col];

                if (ch != '.')
                    return false;
            }

            return true;
        };

        for (int y = height - 1; y >= 0; y--) {
            bool empty = std::all_of(lines[y].begin(), lines[y].end(), [](char c) { return c == '.'; });

            if (empty) {
                expRows.push_back(y);
            }
        }

        for (int x = width - 1; x >= 0; x--) {
            bool empty = empty_col(x);

            if (empty) {
                expCols.push_back(x);
            }
        }

        /* draw(lines); */

        width = lines[0].size();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (lines[y][x] == '#') {
                    galaxies.push_back(Coord{x, y});
                }
            }
        }

        log << galaxies.size() << " galaxies\n";
    }

    int64_t compute(Coord start, Coord end, int64_t expand) {
        int64_t result;

        result = (end - start).manhattan();

        auto mincol = std::min(start.x, end.x);
        auto maxcol = std::max(start.x, end.x);
        auto minrow = std::min(start.y, end.y);
        auto maxrow = std::max(start.y, end.y);

        for (auto c : expCols) {
            if (c >= mincol && c <= maxcol) {
                result += expand;
            }
        }

        for (auto r : expRows) {
            if (r >= minrow && r <= maxrow) {
                result += expand;
            }
        }

        return result;
    }

    void execute(int expand) {
        int64_t result{0};

        for (int g = 0; g < galaxies.size(); g++) {
            for (int g2 = 0; g2 < galaxies.size(); g2++) {
                if (g != g2) {
                    auto r = compute(galaxies[g], galaxies[g2], expand - 1);
                    result += r;
                }
            }
        }

        log << "Execute expand x " << expand << " : " << result / 2 << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = rs::split_string_view(getFileContent(filename), '\n');
    {
        Context context;
        context.parse(lines);
        context.execute(2);
        context.execute(10);
        context.execute(100);
        context.execute(1000000);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
