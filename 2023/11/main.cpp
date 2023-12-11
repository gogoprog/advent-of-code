#include "../../common_fast.h"

struct Context {
    Vector<String> lines;
    int width, height;
    Vector<Coord> galaxies;

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

        for (int y = height - 1; y >= 0; y--) {
            bool empty = std::all_of(lines[y].begin(), lines[y].end(), [](char c) { return c == '.'; });

            if (empty) {
                String newline;
                newline.resize(width);
                std::fill(newline.begin(), newline.end(), '.');
                lines.insert(lines.begin() + y, newline);
            }
        }

        height = lines.size();

        auto empty_col = [&](auto col) {
            for (int y = 0; y < height; y++) {
                char ch = lines[y][col];

                if (ch != '.')
                    return false;
            }

            return true;
        };

        for (int x = width - 1; x >= 0; x--) {
            bool empty = empty_col(x);

            if (empty) {
                for (int y = 0; y < height; y++) {
                    auto &line = lines[y];
                    line.insert(line.begin() + x, '.');
                }
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

    void part1() {
        auto result{0};

        for (int g = 0; g < galaxies.size(); g++) {
            for (int g2 = 0; g2 < galaxies.size(); g2++) {
                if(g != g2) {
                    auto r = (galaxies[g2] - galaxies[g]).manhattan();
                    result += r;
                }
            }
        }

        log << "Part1: " << result/2 << endl;
    }

    void part2() {
        auto result{0};

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = rs::split_string_view(getFileContent(filename), '\n');
    {
        Context context;
        context.parse(lines);
        context.part1();
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
