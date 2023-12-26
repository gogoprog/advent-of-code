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
    int id;
    P position;
    P velocity;

    P getPosition(const Int64 time) const {
        return position + velocity * time;
    }
};

inline double dot(const P &u, const P &v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline double norm2(const P &v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline double norm(const P &v) {
    return sqrt(norm2(v));
}

inline P cross(const P &b, const P &c) // cross product
{
    return {b.y * c.z - c.y * b.z, b.z * c.x - c.z * b.x, b.x * c.y - c.x * b.y};
}

bool intersection(const P &a1, const P &a2, const P &b1, const P &b2, P &ip) {
    P da = a2 - a1;
    P db = b2 - b1;
    P dc = b1 - a1;

    if (dot(dc, cross(da, db)) != 0.0) // lines are not coplanar
        return false;

    auto s = dot(cross(dc, db), cross(da, db)) / norm2(cross(da, db));

    auto is = Int64(s);
    if (s == is) {

        if (b1 + is * db == a1 + is * da) {
            ip = a1 + is * da;
            return true;
        }
    }

    return false;

    /* if (s >= 0.0 && s <= 1.0) { */
    /* ip = a1 + da * P{s, s, s}; */
    /* return true; */
    /* } */

    /* return false; */
}

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

    log << lambda << " " << gamma << "" << beta << endl;

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

        int id = 0;
        for (auto &hs : hailstones) {
            hs.id = id;
            ++id;
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

        Set<P> velocities;

        for (auto &hs : hailstones) {

            for (auto &other_hs : hailstones) {
                if (&hs != &other_hs) {

                    Int64 time = 0;

                    while (true) {

                        for (auto time2 = 1; time2 < 100; ++time2) {

                            auto pos1 = hs.getPosition(time);
                            auto pos2 = other_hs.getPosition(time + time2);
                            auto delta = pos2 - pos1;

                            auto count = 0;

                            for (auto &hs2 : hailstones) {
                                /* Result3 iresult; */
                                P iresult;

                                if (intersection(pos1, pos1 + delta, hs2.getPosition(time), hs2.getPosition(time + 1),
                                                 iresult)) {

                                    ++count;
                                }
                            }

                            static int bestcount = 0;
                            if (count > bestcount) {
                                bestcount = count;
                                log << "bestcount: " << bestcount << endl;
                            }

                            if (count == hailstones.size()) {
                                log << "winrar : " << delta << " at time " << time << endl;

                                auto v = delta;
                                auto pos = pos1 - v * time;

                                result = pos.x + pos.y + pos.z;
                            }
                        }

                        ++time;
                        if (time > 100) {
                            break;
                        }
                    }
                }
            }
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
    process("input.txt");
    return 0;
}
