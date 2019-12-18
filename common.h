#include <algorithm>
#include <array>
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
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define log std::cout
#define endl std::endl

template <class K, class V> using Map = std::map<K, V>;
template <class A, class B> using Pair = std::pair<A, B>;
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
using Point = Pair<int, int>;
using Line = Pair<Point, Point>;

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
};

Point operator+(const Point &a, const Point &b) {
    return {a.first + b.first, a.second + b.second};
}

template <typename T> std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
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

Vector<String> getFileLines(const String &filename, const char delim = '\n') {
    Vector<String> result;
    std::ifstream f(filename);
    String line;

    while (std::getline(f, line, delim)) {
        result.push_back(line);
    }

    f.close();

    return result;
}

template <typename A, typename B> B convert(const A a) {
}

template <> int convert(const String a) {
    return std::stoi(a);
}

template <> String convert(const String a) {
    return a;
}

template <typename R = String> Vector<R> splitString(const String &input, const char delim = '\n') {
    Vector<String> result;
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
        minPoint.first = std::min(pos.first, minPoint.first);
        minPoint.second = std::min(pos.second, minPoint.second);
        maxPoint.first = std::max(pos.first, maxPoint.first);
        maxPoint.second = std::max(pos.second, maxPoint.second);
    }

    return {minPoint, maxPoint};
}
