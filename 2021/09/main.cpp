#include "../../common.h"

struct Context {
    static constexpr Array<Point, 4> directions = {Point{0, -1}, Point{1, 0}, Point{0, 1}, Point{-1, 0}};
    struct Data {
        Point point;
        int value;
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

    void run() {
        auto isLowPoint = [this](auto data) {
            return std::all_of(directions.begin(), directions.end(), [this, data](auto delta) {
                auto point = delta + data.point;
                return !exists(point) || data.value < get(point);
            });
        };

        auto computeRisk = [](auto data) { return data.value - '0' + 1; };

        auto view = dataAsVector | rv::filter(isLowPoint) | rv::transform(computeRisk);
        auto risk_level = std::reduce(view.begin(), view.end(), 0);

        log << "Part1: " << risk_level << endl;
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
