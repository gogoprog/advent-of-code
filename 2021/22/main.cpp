#include "../../common.h"

struct Cube {
    Vector3 min;
    Vector3 max;

    ull size() const {
        return (max.x - min.x + 1ull) * (max.y - min.y + 1ull) * (max.z - min.z + 1ull);
    }

    static bool intersect(const Cube &a, const Cube &b) {
        return (a.min.x <= b.max.x && a.max.x >= b.min.x) && (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
               (a.min.z <= b.max.z && a.max.z >= b.min.z);
    }

    static Cube getIntersection(const Cube &a, const Cube &b) {
        Cube result;

        result.min.x = std::max(a.min.x, b.min.x);
        result.max.x = std::min(a.max.x, b.max.x);
        result.min.y = std::max(a.min.y, b.min.y);
        result.max.y = std::min(a.max.y, b.max.y);
        result.min.z = std::max(a.min.z, b.min.z);
        result.max.z = std::min(a.max.z, b.max.z);

        return result;
    }

    void split(SmallVector<Cube, 8> &result) const {
        auto midx = (max.x - min.x) / 2 + min.x;
        auto midy = (max.y - min.y) / 2 + min.y;
        auto midz = (max.z - min.z) / 2 + min.z;

        auto cen = Vector3{midx, midy, midz};

        auto s = size();

        add(result, {{min.x, min.y, min.z}, {cen.x, cen.y, cen.z}});
        add(result, {{cen.x + 1, cen.y + 1, cen.z + 1}, {max.x, max.y, max.z}});
        add(result, {{cen.x + 1, min.y, min.z}, {max.x, cen.y, cen.z}});
        add(result, {{min.x, cen.y + 1, min.z}, {cen.x, max.y, cen.z}});
        add(result, {{min.x, min.y, cen.z + 1}, {cen.x, cen.y, max.z}});
        add(result, {{cen.x + 1, min.y, cen.z + 1}, {max.x, cen.y, max.z}});
        add(result, {{cen.x + 1, cen.y + 1, min.z}, {max.x, max.y, cen.z}});
        add(result, {{min.x, cen.y + 1, cen.z + 1}, {cen.x, max.y, max.z}});
    }

    static void add(SmallVector<Cube, 8> &result, const Cube &cube) {

        if (cube.min.x > cube.max.x || cube.min.y > cube.max.y || cube.min.z > cube.max.z) {
            return;
        }

        if (std::find(result.begin(), result.end(), cube) == result.end()) {
            result.push_back(cube);
        }
    }

    bool contains(const Cube &other) {
        return min.x <= other.min.x && max.x >= other.max.x && min.y <= other.min.y && max.y >= other.max.y &&
               min.z <= other.min.z && max.z >= other.max.z;
    }

    bool operator!=(const Cube &other) const {
        return min != other.min || max != other.max;
    }

    bool operator==(const Cube &other) const {
        return min == other.min && max == other.max;
    }

    bool operator<(const Cube &other) const {

        if (max == other.max) {
            return min < other.min;
        }

        return max < other.max;
    }
};

struct Entry {
    Cube cube;
    int value;
};

struct Context {
    Vector<Entry> entries;

    void parse(const String &line) {
        String action;
        Vector3 from, to;
        char c;

        InputStringStream iss{line};

        iss >> action >> c >> c >> from.x >> c >> c >> to.x >> c >> c >> c >> from.y >> c >> c >> to.y >> c >> c >> c >>
            from.z >> c >> c >> to.z;

        auto value = (action == "on");
        auto cube = Cube{from, to};
        entries.push_back(Entry{cube, value});
    }

    void run() {
        {
            ull result{0};
            octree(result, {{-50, -50, -50}, {50, 50, 50}});

            log << "Part1: " << result << endl;
        }

        {
            ull result{0};
            auto size = 200000;

            Vector<Entry *> input_entries;

            for (auto &entry : entries) {
                input_entries.push_back(&entry);
            }
            octree2(result, {{-size, -size, -size}, {size, size, size}}, input_entries, false);

            log << "Part2: " << result << endl;
        }
    }

    void octree(ull &result, const Cube &zone) {
        bool all_contains = true;

        for (auto &entry : entries) {
            if (Cube::intersect(entry.cube, zone)) {

                if (!entry.cube.contains(zone)) {
                    all_contains = false;
                    break;
                }
            }
        }

        if (!all_contains) {
            SmallVector<Cube, 8> sub_zones;
            zone.split(sub_zones);

            for (auto &zone : sub_zones) {
                octree(result, zone);
            }
        } else {
            auto value = false;

            for (auto &entry : entries) {
                if (entry.cube.contains(zone)) {
                    value = entry.value;
                }
            }

            if (value) {
                result += zone.size();
            }
        }
    }

    void octree2(ull &result, const Cube &zone, const Vector<Entry *> &input_entries, bool init_value) {
        auto copy_entries = Vector<Entry *>();

        for (auto &entry : input_entries) {
            if (Cube::intersect(entry->cube, zone)) {

                if (!entry->cube.contains(zone)) {
                    copy_entries.push_back(entry);
                } else {
                    copy_entries.resize(0);
                    init_value = entry->value;
                }
            }
        }

        auto len = copy_entries.size();

        if (len > 1) {
            SmallVector<Cube, 8> sub_zones;
            zone.split(sub_zones);

            for (auto &zone : sub_zones) {
                octree2(result, zone, copy_entries, init_value);
            }
        } else if (len == 1) {
            auto entry = copy_entries[0];
            auto intersection = Cube::getIntersection(entry->cube, zone);

            if (entry->value && !init_value) {
                result += intersection.size();
            } else if (!entry->value && init_value) {
                result += zone.size() - intersection.size();
            } else {
                if (init_value) {
                    result += zone.size();
                }
            }

        } else {
            if (init_value) {
                result += zone.size();
            }
        }
    }

    void check() {
        Vector3 min{0, 0, 0};
        Vector3 max{0, 0, 0};

        for (auto &entry : entries) {
            min.x = std::min(entry.cube.min.x, min.x);
            min.y = std::min(entry.cube.min.y, min.y);
            min.z = std::min(entry.cube.min.z, min.z);
            max.x = std::max(entry.cube.max.x, max.x);
            max.y = std::max(entry.cube.max.y, max.y);
            max.z = std::max(entry.cube.max.z, max.z);
        }

        log << min << endl;
        log << max << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx;

    for (auto &line : lines) {
        ctx.parse(line);
    }

    ctx.check();
    ctx.run();
}

int main() {
    process("sample.txt");
    process("sample2.txt");
    process("sample3.txt");
    process("input.txt");
    return 0;
}
