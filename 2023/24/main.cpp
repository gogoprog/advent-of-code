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

template <typename T> inline Point3<T> cross(const Point3<T> &b, const Point3<T> &c) {
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

double shortestDistance(const P &line1_start, const P &line1_direction, const P &line2_start,
                        const P &line2_direction) {
    P p1_to_p2 = (line2_start - line1_start);
    double d1 = dot(line1_direction, line1_direction);
    double d2 = dot(line1_direction, line2_direction);
    double d3 = dot(line2_direction, line2_direction);
    double d4 = dot(line1_direction, p1_to_p2);
    double d5 = dot(line2_direction, p1_to_p2);

    double t = (d4 * d2 - d5 * d1) / (d1 * d3 - d2 * d2);

    Point3<double> closestPoint1{line1_start.x + t * line1_direction.x, line1_start.y + t * line1_direction.y,
                                 line1_start.z + t * line1_direction.z};

    Point3<double> closestPoint2{line2_start.x + (d5 + t * d2) * line2_direction.x,
                                 line2_start.y + (d5 + t * d2) * line2_direction.y,
                                 line2_start.z + (d5 + t * d2) * line2_direction.z};

    auto delta = closestPoint2 - closestPoint1;
    return delta.squareLength();
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

        auto len = hailstones.size();

        Array<Map<Int64, Int64>, 3> possib;

        P maxdeltas{100000, 100000, 10000};

        for (auto &hs : hailstones) {

            for (auto &other_hs : hailstones) {
                if (&hs != &other_hs) {

                    for (int i = 0; i < 3; ++i) {

                        if (hs.velocity[i] == other_hs.velocity[i]) {
                            auto delta = hs.position[i] - other_hs.position[i];
                            log << "yep " << i << " : " << hs.velocity[i] << " -> " << delta << endl;

                            delta = std::abs(delta);

                            if (possib[i][hs.velocity[i]] == 0) {

                                possib[i][hs.velocity[i]] = delta;
                            }

                            possib[i][hs.velocity[i]] = std::min(possib[i][hs.velocity[i]], delta);
                            /* maxdeltas[i] = std::min(maxdeltas[i], delta); */
                        }
                    }
                }
            }

            /* break; */
        }

        auto find_v = [](Map<Int64, Int64> &values) {

            log << values << endl;
            for (auto &kv : values) {

                auto iniv = kv.first;

                for (int i = 1; i <= kv.second; ++i) {
                    auto newv = iniv + i;

                    auto count = 0;

                    for (auto &kv2 : values) {
                        auto delta = newv - kv2.first;

                        if (delta > 0 && (kv.second % delta) == 0) {
                            count++;
                        }
                    }

                    log << count << endl;

                    if (count == values.size()) {
                        log << "yep " << newv << endl;
                        break;
                    }
                }
            }
        };

        /* log << factors << endl; */
        log << possib << endl;

        find_v(possib[0]);
        find_v(possib[1]);
        find_v(possib[2]);

        return;

        auto test = [&](auto &velo) {
            for (auto &hs : hailstones) {
                for (auto &other_hs : hailstones) {
                    auto pos = hs.position;

                    Int64 time = 0;

                    while (true) {

                        for (auto time2 = 1; time2 < 100; ++time2) {

                            auto pos1 = hs.getPosition(time);
                            auto count = 0;

                            for (auto &hs2 : hailstones) {
                                /* Result3 iresult; */
                                P iresult;

                                if (intersection(pos1, pos1 + velo, hs2.getPosition(time), hs2.getPosition(time + 1),
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
                                log << "winrar : " << velo << " at time " << time << endl;

                                auto pos = pos1 - velo * time;

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
        };

        for (int x = -maxdeltas.x; x <= maxdeltas.x; ++x) {

            if (x && ((maxdeltas.x % x) == 0)) {

                for (int y = -maxdeltas.y; y <= maxdeltas.y; ++y) {

                    if (y && (maxdeltas.y % y) == 0) {

                        for (int z = -maxdeltas.z; z <= maxdeltas.z; ++z) {

                            if (z && (maxdeltas.z % z) == 0) {
                                log << x << " " << y << " " << z << endl;
                                result++;
                            }
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
    /* process("input.txt"); */
    return 0;
}
