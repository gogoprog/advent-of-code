#include "../../common.h"

struct Context {
    Set<Point> points;

    void foldX(const int x) {
        Set<Point> new_points;

        for (auto point : points) {
            if (point.x > x) {
                point.x = x - (point.x - x);
                new_points.insert(point);
            } else {
                new_points.insert(point);
            }
        }

        points = new_points;
    }

    void foldY(const int y) {
        Set<Point> new_points;

        for (auto point : points) {
            if (point.y > y) {
                point.y = y - (point.y - y);
                new_points.insert(point);
            } else {
                new_points.insert(point);
            }
        }

        points = new_points;
    }

    void draw() {
        Map<Point, bool> grid;

        for (auto &point : points) {
            grid[point] = true;
        }

        auto [min, max] = getMinMax(grid);

        for (int y = min.y; y <= max.y; y++) {
            for (int x = min.x; x <= max.x; x++) {
                if (grid[{x, y}]) {
                    log << "#";
                } else {
                    log << " ";
                }
            }

            log << endl;
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    Context ctx;

    bool part1_done{false};

    for (auto &line : lines) {
        if (line.find(',') != String::npos) {
            ctx.points.insert(parsePoint(line));
        }

        if (line.find("fold") != String::npos) {
            InputStringStream iss{line};
            String temp;
            iss >> temp >> temp >> temp;
            auto [axis, value_str] = splitNString<String, 2>(temp, '=');
            auto value = std::stoi(value_str);

            if (axis == "x") {
                ctx.foldX(value);

            } else {
                ctx.foldY(value);
            }

            if (!part1_done) {
                log << "Part1: " << ctx.points.size() << endl;
                part1_done = true;
            }
        }
    }

    ctx.draw();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
