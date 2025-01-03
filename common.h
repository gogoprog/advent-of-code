#ifdef RAYLIB
#include "raylib.h"
#include "raymath.h"
#endif

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <charconv>
#include <cmath>
#include <deque>
#include <fstream>
#include <functional>
#include <initializer_list>
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
#include <source_location>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#if __cplusplus >= 202002L
#include <ranges>
namespace rs = std::ranges;
namespace rv = std::ranges::views;
#endif

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
template <class V> using Deque = std::deque<V>;
template <class V> using PriorityQueue = std::priority_queue<V>;
template <class V> using List = std::list<V>;
using String = std::string;
using StringView = std::string_view;
using Strings = Vector<String>;
using InputStringStream = std::istringstream;
using ull = unsigned long long;
using lli = long long int;
template <class S> using Function = std::function<S>;
template <class V> using UniquePtr = std::unique_ptr<V>;

struct Point {
    struct _xy { int x, y; }l;

    union {
        struct {
            int x;
            int y;
        };
        _xy xy;
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

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point &other) const {
        return !operator==(other);
    }

    bool operator<(const Point &other) const {
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }

    void set(const int x, const int y) {
        this->x = x;
        this->y = y;
    }

    Point &operator+=(const Point &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    int manhattan() const {
        return std::abs(x) + std::abs(y);
    }
};

Point operator+(const Point &a, const Point &b) {
    return {a.x + b.x, a.y + b.y};
}

Point operator-(const Point &a, const Point &b) {
    return {a.x - b.x, a.y - b.y};
}

Point operator*(const Point &a, const int b) {
    return {a.x * b, a.y * b};
}

Point operator/(const Point &a, const int b) {
    return {a.x / b, a.y / b};
}

using Line = Pair<Point, Point>;

Point parsePoint(const String &input) {
    Point result;
    InputStringStream iss{input};
    char c;
    iss >> result.x >> c >> result.y;

    return result;
}

struct Point3 {
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

    const int &operator[](const int c) const {
        return coords[c];
    }

    bool operator==(const Point3 &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Point3 &other) const {
        return !operator==(other);
    }

    bool operator<(const Point3 &other) const {
        if (x == other.x) {
            if (y == other.y) {
                return z < other.z;
            }

            return y < other.y;
        }
        return x < other.x;
    }

    Point3 &operator+=(const Point3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Point3 &operator-=(const Point3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    int manhattan() const {
        return std::abs(x) + std::abs(y) + std::abs(z);
    }
};

Point3 operator-(const Point3 &a, const Point3 &b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Point3 operator+(const Point3 &a, const Point3 &b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Point3 parsePoint3(const String &input) {
    Point3 result;
    InputStringStream iss{input};
    char c;
    iss >> result.x >> c >> result.y >> c >> result.z;

    return result;
}

struct Point4 {
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

    bool operator==(const Point4 &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Point4 &other) const {
        return !operator==(other);
    }

    bool operator<(const Point4 &other) const {
        if (x == other.x) {
            if (y == other.y) {
                return z < other.z;
            }

            return y < other.y;
        }
        return x < other.x;
    }

    Point4 &operator+=(const Point4 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
};

using Coord = Point;
using Coord2 = Point;
using Coord3 = Point3;
using Coord4 = Point4;

#ifndef RAYLIB
using Vector2 = Point;
using Vector3 = Point3;
using Vector4 = Point4;
#endif

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

std::ostream &operator<<(std::ostream &out, const Point &v) {
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

std::ostream &operator<<(std::ostream &out, const Point3 &v) {
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

template <> ull convert(const String a) {
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

template <typename R = String>
Vector<R> splitString(const String &input, const char delim = '\n', const bool skip_empty = true) {
    Vector<R> result;
    InputStringStream iss(input);
    String line;

    result.reserve(1024);

    while (std::getline(iss, line, delim)) {
        if (!skip_empty || !line.empty()) {
            result.push_back(convert<String, R>(line));
        }
    }

    return result;
}

StringView getFileContent(const String &filename, const char delim = '\n') {
    std::ifstream f(filename);
    static std::string contents((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    f.close();
    contents.pop_back();
    return contents;
}

StringView getFileContentFast(const char *filename, const char delim = '\n') {

    static char buffer[32 * 1024];
    long length;
    FILE *f = fopen(filename, "rb"); // was "rb"

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        fread(buffer, sizeof(char), length, f);
        fclose(f);
    }
    buffer[length - 1] = '\0';

    return StringView(buffer, length);
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

template <class T, int max_size> class SmallVector {
    Array<T, max_size> data;
    int dataCount{0};

  public:
    SmallVector() = default;

    SmallVector(std::initializer_list<T> il) {
        for (auto value : il) {
            push_back(value);
        }
    }

    void push_back(const T value) {
        data[dataCount] = value;
        ++dataCount;
    }

    inline int size() const {
        return dataCount;
    }

    T &back() {
        return data[dataCount - 1];
    }

    const T &back() const {
        return data[dataCount - 1];
    }

    auto begin() {
        return data.begin();
    }

    auto end() {
        return data.begin() + dataCount;
    }

    auto begin() const {
        return data.begin();
    }

    auto end() const {
        return data.begin() + dataCount;
    }
};

template <class K, class V> class Dictionary {
  private:
    Vector<K> keys;
    Vector<V> values;

    int getIndex(const K &key) const {
        auto it = std::find(keys.begin(), keys.end(), key);
        return it - keys.begin();
    }

  public:
    V &operator[](const K &key) {
        auto it = std::find(keys.begin(), keys.end(), key);
        auto index = 0;
        if (it == keys.end()) {
            keys.push_back(key);
            values.push_back({});
            index = keys.size() - 1;
        } else {
            index = it - keys.begin();
        }

        return values[index];
    }

    const V &operator[](const K &key) const {
        auto it = std::find(keys.begin(), keys.end(), key);
        auto index = 0;
        if (it == keys.end()) {
            /* throw "No key found"; */
            assert(false);
        } else {
            index = it - keys.begin();
        }

        return values[index];
    }

    void set(const K &key, const V &value) {
        auto it = std::find(keys.begin(), keys.end(), key);
        auto index = 0;
        if (it == keys.end()) {
            keys.push_back(key);
            values.push_back({});
            index = keys.size() - 1;
        } else {
            index = it - keys.begin();
        }
        values[index] = value;
    }

    bool has(const K &key) const {
        auto it = std::find(keys.begin(), keys.end(), key);
        if (it == keys.end()) {
            return false;
        } else {
            return true;
        }
    }

    int getSize() const {
        return keys.size();
    }

    K &getKeyFromIndex(const int index) {
        return keys[index];
    }

    V &getValueFromIndex(const int index) {
        return values[index];
    }

    void clear() {
        keys.resize(0);
        values.resize(0);
    }

    void reserve(const int value) {
        keys.reserve(value);
        values.reserve(value);
    }
};

#if __cplusplus >= 202002L
namespace std::ranges {

struct __accumulate {
    template <input_range _Range, class T, class BinaryOperation>
        requires indirectly_copyable_storable<iterator_t<_Range>, range_value_t<_Range> *>
    constexpr T operator()(_Range &&__r, T init, BinaryOperation op) const {
        auto __first = ranges::begin(__r);
        auto __last = ranges::end(__r);
        return std::accumulate(__first, __last + 1, init, op);
    }
};

inline constexpr __accumulate accumulate{};

namespace actions {

struct __sort {};
inline constexpr __sort sort{};

// :WARNING: `sort` is leaking.
template <input_range _Range> auto &&operator|(_Range &&__r, __sort s) {
    auto *new_vector = new std::vector(ranges::begin(__r), ranges::end(__r));
    std::ranges::sort(*new_vector);
    return *new_vector;
}

template <class T, class BinaryOperation> struct __reduce_view {
    T init;
    BinaryOperation op;
};

struct __reduce {
    template <class T, class BinaryOperation> auto operator()(T init, BinaryOperation op) const {
        return __reduce_view{init, op};
    }
};

inline constexpr __reduce reduce{};

template <input_range _Range, class T, class BinaryOperation>
auto operator|(_Range &&__r, __reduce_view<T, BinaryOperation> _rv) {
    auto result = std::reduce(ranges::begin(__r), ranges::end(__r), _rv.init, _rv.op);
    return result;
}

} // namespace actions

} // namespace std::ranges

namespace ra = std::ranges::actions;

template <typename T> void myassert(T a, T b, const std::source_location location = std::source_location::current()) {
#ifndef NDEBUG
    if (a != b) {
        std::cerr << "file: " << location.file_name() << "(" << location.line() << ":" << location.column() << ") `"
                  << location.function_name() << "`: " << endl;
        std::cerr << "Assertion failed: " << endl << a << " == " << b << endl;
        assert(0);
    }
#endif
}

inline StringView getStringView(auto range) {
    return StringView(&*range.begin(), rs::distance(range));
}

#endif
