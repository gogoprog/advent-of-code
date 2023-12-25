#include "../../common_fast.h"

using P = Point3<Int64>;

struct Result {
    double x;
    double y;
};

struct Result3 {
    double x;
    double y;
    double z;
};

struct Hailstone {
    Point3<Int64> position;
    Point3<Int64> velocity;
};

static bool intersectionXY(const P &from1, const P &to1, const P &from2, const P &to2, Result &result) {
    auto dX = to1.x - from1.x;
    auto dY = to1.y - from1.y;
    auto determinant = dX * (to2.y - from2.y) - (to2.x - from2.x) * dY;

    if (determinant <= 0) {
        return false;
    }

    auto lambda = ((to2.y - from2.y) * (to2.x - from1.x) + (from2.x - to2.x) * (to2.y - from1.y)) / (double)determinant;
    auto gamma = ((from1.y - to1.y) * (to2.x - from1.x) + dX * (to2.y - from1.y)) / (double)determinant;

    if (lambda < 0 || gamma > 0) {
        return false;
    }

    result.x = from1.x + lambda * dX;
    result.y = from1.y + lambda * dY;

    return true;
}

static bool intersectionXYZ(const P &from1, const P &to1, const P &from2, const P &to2, Result3 &result) {
    auto dX = to1.x - from1.x;
    auto dY = to1.y - from1.y;
    double dZ = to1.z - from1.z;
    double determinant = dX * (to2.y - from2.y) * (to2.z - from2.z) - dY * (to2.x - from2.x) * (to2.z - from2.z) +
                         dZ * (to2.x - from2.x) * (to2.y - from2.y);

    if (determinant <= 0) {
        return false;
    }

    double lambda = ((to2.y - from2.y) * (to2.z - from1.z) + (from2.x - to2.x) * (to2.y - from1.y) +
                     (from2.z - to2.z) * (to2.x - from1.x)) /
                    determinant;
    double gamma =
        ((from1.y - to1.y) * (to2.x - from1.x) + dX * (to2.y - from1.y) + dY * (to2.z - from1.z)) / determinant;
    double beta =
        ((from1.z - to1.z) * (to2.x - from1.x) + dX * (to2.z - from1.z) + dZ * (to2.y - from1.y)) / determinant;

    /* if (lambda < 0 || gamma > 0 || beta > 0) { */
    /*     return false; */
    /* } */

    result.x = from1.x + lambda * dX;
    result.y = from1.y + lambda * dY;
    result.z = from1.z + lambda * dZ;

    return true;
}

struct Context {

    Vector<Hailstone> hailstones;

    void parse(auto lines) {

        auto to_hailstones = rv::transform([](auto line) {
            Hailstone hs;

            auto parts = line | rv::split_sv(" @ ");

            auto left = parts | rv::get0 | rv::split_sv(", ") | rv::to_ints;
            auto right = parts | rv::get1 | rv::split_sv(", ") | rv::to_ints;

            hs.position.x = left | rv::get0;
            hs.position.y = left | rv::get1;
            hs.position.z = left | rv::get2;

            hs.velocity.x = right | rv::get0;
            hs.velocity.y = right | rv::get1;
            hs.velocity.z = right | rv::get2;

            return hs;
        });

        hailstones.resize(0);
        rs::copy(lines | to_hailstones, std::back_inserter(hailstones));

        for (auto hs : hailstones) {
            /* log << hs.position << " " << hs.velocity << "\n"; */
        }
    }

    void part1() {
        Int64 min, max;

        if (hailstones.size() < 10) {
            min = 7;
            max = 27;
        } else {
            min = 200000000000000;
            max = 400000000000000;
        }

        Int64 result{0};

        auto is_in_boundaries = [&](auto value) { return value >= min && value <= max; };

        for (auto &hs : hailstones) {

            for (auto &other_hs : hailstones) {
                if (&hs != &other_hs) {

                    auto from1 = hs.position;
                    auto to1 = hs.position + hs.velocity;

                    auto from2 = other_hs.position;
                    auto to2 = other_hs.position + other_hs.velocity;

                    Result iresult;

                    if (intersectionXY(from1, to1, from2, to2, iresult)) {

                        /* log << "inter result " << iresult.x << " " << iresult.y << "\n"; */

                        if (is_in_boundaries(iresult.x) && is_in_boundaries(iresult.y)) {
                            /* log << "result " << iresult.x << " " << iresult.y << "\n"; */
                            result++;
                        }
                    }
                }
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2() {

        Int64 result{0};

        P rock_position{0, 0, 0};
        P rock_velocity{0, 0, 0};

        for (auto &hs : hailstones) {

            for(int i = 0; i< 10; ++i) {

            }

            /*
                    Result3 iresult;

                    if (intersectionXYZ(from1, to1, from2, to2, iresult)) {

                        log << "inter result " << iresult.x << " " << iresult.y << " " << iresult.z  << "\n";
                    }
                }
            }
            */
        }

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
    /* process("input.txt"); */
    return 0;
}
