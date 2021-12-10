#include "../../common.h"

struct Context {
    static constexpr Array<Point, 4> directions = {Point{0, -1}, Point{1, 0}, Point{0, 1}, Point{-1, 0}};
    struct Data {
        Point point;
        int value;
    };
    struct Basin {
        Vector<Point> points;
    };
    Vector<String> data;
    Vector<Data> dataAsVector;
    int width;
    int height;

    void init() {
        width = data[0].size();
        height = data.size();

        for (int y{0}; y < height; ++y) {
            for (int x{0}; x < width; ++x) {
                dataAsVector.push_back({{x, y}, data[y][x]});
            }
        }
    }

    bool exists(const Point &p) {
        auto x = p.x;
        auto y = p.y;
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    int get(const Point &p) {
        return data[p.y][p.x];
    }

    void expandBasin(Basin &basin, const Point &point) {
        for (auto &delta : directions) {
            auto next_point = point + delta;

            if (exists(next_point)) {
                auto current_value = get(point);
                auto next_value = get(next_point);

                if (next_value != '9' && next_value > current_value) {
                    if (rs::find(basin.points, next_point) == basin.points.end()) {
                        basin.points.push_back(next_point);
                        expandBasin(basin, next_point);
                    }
                }
            }
        }
    }

    void run() {
        auto isLowPoint = [this](auto data) {
            return std::all_of(directions.begin(), directions.end(), [this, data](auto delta) {
                auto point = delta + data.point;
                return !exists(point) || data.value < get(point);
            });
        };

        auto computeRisk = [](auto data) { return data.value - '0' + 1; };

        {
            auto risk_level =
                dataAsVector | rv::filter(isLowPoint) | rv::transform(computeRisk) | ra::reduce(0, std::plus());

            log << "Part1: " << risk_level << endl;
        }
        {
            auto findBasin = [this](auto data) {
                Basin basin{};
                basin.points.push_back(data.point);
                expandBasin(basin, data.point);
                return basin;
            };

            auto getSize = [](auto basin) { return basin.points.size(); };

            auto result = dataAsVector | rv::filter(isLowPoint) | rv::transform(findBasin) | rv::transform(getSize) |
                          ra::sort | rv::reverse | rv::take(3) | ra::reduce(1ull, std::multiplies<ull>());

            log << "Part2: " << result << endl;
        }
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context context{lines};
    context.init();
    context.run();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
