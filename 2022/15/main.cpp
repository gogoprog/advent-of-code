#include "../../common.h"

struct Sensor {
    Coord position;
    Coord closestBeacon;

    bool isCovered(const Coord pos) const {
        auto delta = pos - position;

        if (delta.manhattan() <= range()) {
            /* return pos != closestBeacon; */
            return true;
        }

        return false;
    }

    int range() const {
        auto delta = closestBeacon - position;
        return delta.manhattan();
    }
};

struct Context {

    Vector<Sensor> sensors;

    Coord min{10000, 10000};
    Coord max{-10000, -10000};

    void parse(const Strings &lines) {
        String str;
        char c;
        for (auto &line : lines) {
            sensors.push_back({});
            auto &sensor = sensors.back();
            InputStringStream iss{line};

            iss >> str >> str;
            iss.ignore(3);
            iss >> sensor.position.x;
            iss.ignore(4);
            iss >> sensor.position.y;

            iss >> c >> str >> str >> str >> str;

            iss.ignore(3);
            iss >> sensor.closestBeacon.x;
            iss.ignore(4);
            iss >> sensor.closestBeacon.y;

            /* log << sensor.position << " : " << sensor.closestBeacon << endl; */

            min.x = std::min(min.x, sensor.closestBeacon.x);
            min.y = std::min(min.y, sensor.closestBeacon.y);
            max.x = std::max(max.x, sensor.closestBeacon.x);
            max.y = std::max(max.y, sensor.closestBeacon.y);
        }
    }

    bool isCovered(const Coord pos) const {
        return std::any_of(sensors.begin(), sensors.end(), [=](auto &sensor) { return sensor.isCovered(pos); });
    }

    void part1(const int y) {
        auto result{0};
        for (int x = min.x - 2000000; x <= max.x + 2000000; x++) {
            auto pos = Coord{x, y};
            auto r = isCovered(pos);

            if (r) {

                if (std::all_of(sensors.begin(), sensors.end(),
                                [=](auto &sensor) { return sensor.closestBeacon != pos; })) {
                    result++;
                }
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2(const int maxv) {
        auto result{0ull};

        for (int y = 0; y <= maxv; y++) {
            for (int x = 0; x <= maxv; x++) {
                auto pos = Coord{x, y};
                auto possible{true};

                for (auto &sensor : sensors) {
                    if (sensor.isCovered(pos)) {
                        auto delta = pos - sensor.position;
                        auto offset = sensor.range() - std::abs(delta.y);

                        x = sensor.position.x + offset;
                        possible = false;
                        break;
                    }
                }

                if (possible && x <= maxv) {
                    if (std::all_of(sensors.begin(), sensors.end(),
                                    [=](auto &sensor) { return sensor.closestBeacon != pos; })) {
                        result = x * 4000000ull + y;
                        log << "Part2: " << result << endl;
                        return;
                    }
                }
            }
        }
    }
};

template <int y, int max> void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    {
        Context context;
        context.parse(lines);
        context.part1(y);
    }
    {
        Context context;
        context.parse(lines);
        context.part2(max);
    }
}

int main() {
    process<10, 20>("sample.txt");
    process<2000000, 4000000>("input.txt");
    return 0;
}
