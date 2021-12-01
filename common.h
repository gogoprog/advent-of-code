#include <algorithm>
#include <array>
#include <bitset>
#include <charconv>
#include <cmath>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define log std::cout
#define endl std::endl

template <class K, class V> using Map = std::map<K, V>;
template <class A, class B> using Pair = std::pair<A, B>;
template <class... Types> using Tuple = std::tuple<Types...>;
template <class K, class V> using UnorderedMap = std::unordered_map<K, V>;
template <class V> using Vector = std::vector<V>;
template <class V, int size> using Array = std::array<V, size>;
template <class V> using Set = std::set<V>;
template <class V> using UnorderedSet = std::unordered_set<V>;
template <class T> using Grid = Map<int, Map<int, T>>;
template <class V> using Queue = std::queue<V>;
template <class V> using PriorityQueue = std::priority_queue<V>;
template <class V> using List = std::list<V>;
using String = std::string;
using InputStringStream = std::istringstream;
using ull = unsigned long long;
using lli = long long int;
template <class S> using Function = std::function<S>;

struct Vector2 {
    union {
        struct {
            int x;
            int y;
        };
        struct {
            int coords[2];
        };
        struct {
            int first;
            int second;
        };
    };

    int &operator[](const int c) {
        return coords[c];
    }

    bool operator==(const Vector2 &other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2 &other) const {
        return !operator==(other);
    }

    bool operator<(const Vector2 &other) const {
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }

    Vector2 &operator+=(const Vector2 &other) {
        x += other.x;
        y += other.y;
        return *this;
    }
};

Vector2 operator*(const Vector2 &a, const int b) {
    return {a.x * b, a.y * b};
}

Vector2 operator/(const Vector2 &a, const int b) {
    return {a.x / b, a.y / b};
}

using Point = Vector2;
using Line = Pair<Point, Point>;

Point operator+(const Point &a, const Point &b) {
    return {a.x + b.x, a.y + b.y};
}

struct Vector3 {
    union {
        struct {
            int x;
            int y;
            int z;
        };
        struct {
            int coords[3];
        };
    };

    int &operator[](const int c) {
        return coords[c];
    }

    bool operator==(const Vector3 &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vector3 &other) const {
        return !operator==(other);
    }

    bool operator<(const Vector3 &other) const {
        if (x == other.x) {
            if (y == other.y) {
                return z < other.z;
            }

            return y < other.y;
        }
        return x < other.x;
    }

    Vector3 &operator+=(const Vector3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
};

struct Vector4 {
    union {
        struct {
            int x;
            int y;
            int z;
            int w;
        };
        struct {
            int coords[4];
        };
    };

    int &operator[](const int c) {
        return coords[c];
    }

    bool operator==(const Vector4 &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vector4 &other) const {
        return !operator==(other);
    }

    bool operator<(const Vector4 &other) const {
        if (x == other.x) {
            if (y == other.y) {
                return z < other.z;
            }

            return y < other.y;
        }
        return x < other.x;
    }

    Vector4 &operator+=(const Vector4 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
};

template <typename T> std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
    if (!v.empty()) {
        out << '[';
        std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
        out << "\b\b]";
    }
    return out;
}

template <typename T> std::ostream &operator<<(std::ostream &out, const std::list<T> &v) {
    if (!v.empty()) {
        out << '[';
        std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
        out << "\b\b]";
    }
    return out;
}

template <typename T, std::size_t S> std::ostream &operator<<(std::ostream &out, const std::array<T, S> &v) {
    if (!v.empty()) {
        out << '[';
        std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
        out << "\b\b]";
    }
    return out;
}

template <typename T> std::ostream &operator<<(std::ostream &out, const std::set<T> &v) {
    if (!v.empty()) {
        out << '[';
        std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
        out << "\b\b]";
    }
    return out;
}

template <typename T, typename U> std::ostream &operator<<(std::ostream &out, const std::map<T, U> &v) {
    if (!v.empty()) {
        out << '[';
        bool first{true};
        for (auto &kv : v) {
            if (!first)
                out << ", ";
            out << kv;
            first = false;
        }
        out << "\b\b]";
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const Vector2 &v) {
    out << '[';
    out << v.x;
    out << ", ";
    out << v.y;
    out << "]";
    return out;
}

template <typename A, typename B> std::ostream &operator<<(std::ostream &out, const std::pair<A, B> &v) {
    out << '[';
    out << v.first;
    out << ", ";
    out << v.second;
    out << "]";
    return out;
}

std::ostream &operator<<(std::ostream &out, const Vector3 &v) {
    out << '[';
    out << v.x;
    out << ", ";
    out << v.y;
    out << ", ";
    out << v.z;
    out << "]";
    return out;
}

std::ostream &operator<<(std::ostream &out, const std::vector<int8_t> &v) {
    if (!v.empty()) {
        for (auto vv : v) {
            out << int(vv) << ' ';
        }
    }
    return out;
}

template <typename A, typename B> B convert(const A a) {
}

template <> int convert(const String a) {
    return std::stoi(a);
}

template <> String convert(const String a) {
    return a;
}

template <typename T = String> Vector<T> getFileLines(const String &filename, const char delim = '\n') {
    Vector<T> result;
    std::ifstream f(filename);
    String line;

    while (std::getline(f, line, delim)) {
        result.push_back(convert<String, T>(line));
    }

    f.close();

    return result;
}

template <typename R = String> Vector<R> splitString(const String &input, const char delim = '\n') {
    Vector<R> result;
    InputStringStream iss(input);
    String line;

    while (std::getline(iss, line, delim)) {
        result.push_back(convert<String, R>(line));
    }

    return result;
}

template <typename R, int size> Array<R, size> splitNString(const String &input, const char delim = '\n') {
    Array<R, size> result;
    InputStringStream iss(input);
    String line;

    int i{0};

    while (std::getline(iss, line, delim) && i < size) {
        result[i] = convert<String, R>(line);
        ++i;
    }

    return result;
}

template <typename T> std::tuple<Point, Point> getMinMax(const Map<Point, T> &map) {
    Point minPoint{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
    Point maxPoint{std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};

    for (auto &kv : map) {
        auto &pos = kv.first;
        minPoint.x = std::min(pos.x, minPoint.x);
        minPoint.y = std::min(pos.y, minPoint.y);
        maxPoint.x = std::max(pos.x, maxPoint.x);
        maxPoint.y = std::max(pos.y, maxPoint.y);
    }

    return {minPoint, maxPoint};
}
