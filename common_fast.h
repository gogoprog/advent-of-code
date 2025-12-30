
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <charconv>
#include <cmath>
#include <deque>
#include <functional>
#include <initializer_list>
#include <inttypes.h>
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

#include <ranges>
namespace rs = std::ranges;
namespace rv = std::ranges::views;

template <class K, class V> using Map = std::map<K, V>;
template <class A, class B> using Pair = std::pair<A, B>;
template <class... Types> using Tuple = std::tuple<Types...>;
template <class K, class V> using UnorderedMap = std::unordered_map<K, V>;
template <class V> using Vector = std::vector<V>;
template <class V, int size> using Array = std::array<V, size>;
template <class V> using Set = std::set<V>;
template <class V> using UnorderedSet = std::unordered_set<V>;
template <class T> using Grid2 = Map<int, Map<int, T>>;
template <class V> using Queue = std::queue<V>;
template <class V> using Dequeue = std::deque<V>;
template <class V> using PriorityQueue = std::priority_queue<V>;
template <class V> using List = std::list<V>;
using String = std::string;
using StringView = std::string_view;
using Strings = Vector<String>;
using StringViews = Vector<StringView>;
using ull = unsigned long long;
using lli = long long int;
template <class S> using Function = std::function<S>;
template <class V> using UniquePtr = std::unique_ptr<V>;

using Int8 = int8_t;
using Uint8 = uint8_t;
using Int64 = int64_t;
using Uint64 = uint64_t;

constexpr std::int64_t operator""_int64(unsigned long long v) {
    return static_cast<std::int64_t>(v);
}

template <typename INT = int> struct Point {
    union {
        struct {
            INT x;
            INT y;
        };
        struct {
            INT coords[2];
        };
        struct {
            INT first;
            INT second;
        };
    };

    inline INT &operator[](const INT c) {
        return coords[c];
    }

    inline bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }

    inline bool operator!=(const Point &other) const {
        return !operator==(other);
    }

    inline bool operator<(const Point &other) const {
        if (x == other.x) {
            return y < other.y;
        }
        return x < other.x;
    }

    inline void set(const INT x, const INT y) {
        this->x = x;
        this->y = y;
    }

    inline Point &operator+=(const Point &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline INT manhattan() const {
        return std::abs(x) + std::abs(y);
    }
};

template <typename INT> Point<INT> operator+(const Point<INT> &a, const Point<INT> &b) {
    return {a.x + b.x, a.y + b.y};
}

template <typename INT> Point<INT> operator-(const Point<INT> &a, const Point<INT> &b) {
    return {a.x - b.x, a.y - b.y};
}

template <typename INT> Point<INT> operator*(const Point<INT> &a, const Point<INT> b) {
    return {a.x * b.x, a.y * b.y};
}

template <typename INT> Point<INT> operator*(const Point<INT> &a, const int b) {
    return {a.x * b, a.y * b};
}

template <typename INT> Point<INT> operator/(const Point<INT> &a, const int b) {
    return {a.x / b, a.y / b};
}

using Line = Pair<Point<int>, Point<int>>;

template <typename INT = int> struct Point3 {
    union {
        struct {
            INT x;
            INT y;
            INT z;
        };
        struct {
            INT coords[3];
        };
    };

    INT &operator[](const int c) {
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

    Uint64 squareLength() const {
        return x * x + y * y + z * z;
    }

    INT length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    void normalize() {
        auto len = length();
        x = INT(x / len);
        y = INT(y / len);
        z = INT(z / len);
    }

    template <typename O> Point3<INT> &operator=(const Point3<O> &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

template <typename INT> Point3<INT> operator-(const Point3<INT> &a, const Point3<INT> &b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

template <typename INT> Point3<INT> operator+(const Point3<INT> &a, const Point3<INT> &b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

template <typename INT, typename O> Point3<INT> operator+(const Point3<INT> &a, const Point3<O> &b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

template <typename INT, typename O> Point3<INT> operator*(const Point3<INT> &a, const O b) {
    return {a.x * b, a.y * b, a.z * b};
}

template <typename INT, typename O> Point3<INT> operator/(const Point3<INT> &a, const O b) {
    return {a.x / b, a.y / b, a.z / b};
}

template <typename INT> Point3<INT> operator*(const INT b, const Point3<INT> &a) {
    return {a.x * b, a.y * b, a.z * b};
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

using Coord = Point<int>;
using Coord2 = Point<int>;
using Coord3 = Point3<int>;
using Coord4 = Point4;
using Coords = Vector<Coord>;

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

StringView getFileContent(const char *filename, const char delim = '\n') {

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

    return StringView(buffer, length - 1);
}

template <typename T> std::tuple<Coord, Coord> getMinMax(const Map<Coord, T> &map) {
    Coord minPoint{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
    Coord maxPoint{std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};

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

inline StringView getStringView(auto range) {
    return StringView(&*range.begin(), rs::distance(range));
}

inline int64_t parseInt(StringView line) {
    int64_t result;
    char c = *line.data();

    if (c == '-') {
        std::from_chars(line.data() + 1, line.data() + line.size(), result);
        return -result;
    }

    std::from_chars(line.data(), line.data() + line.size(), result);
    return result;
}

inline uint64_t parseUint64(StringView line) {
    uint64_t result;
    char c = *line.data();
    std::from_chars(line.data(), line.data() + line.size(), result);
    return result;
}

inline int32_t parseInt32(StringView line) {
    int32_t result;
    char c = *line.data();

    if (c == '-') {
        std::from_chars(line.data() + 1, line.data() + line.size(), result);
        return -result;
    }

    std::from_chars(line.data(), line.data() + line.size(), result);
    return result;
}

inline int64_t parsePositiveInt(StringView line) {
    int64_t result;
    std::from_chars(line.data(), line.data() + line.size(), result);
    return result;
}

class Logger {
  public:
    Logger &operator<<(StringView str) {
        printf("%.*s", int(str.size()), str.data());
        return *this;
    }

    Logger &operator<<(char c) {
        printf("%c", c);
        return *this;
    }

    Logger &operator<<(int i) {
        printf("%d", i);
        return *this;
    }

    Logger &operator<<(uint16_t i) {
        printf("%d", i);
        return *this;
    }

    Logger &operator<<(int64_t i) {
        printf("%" PRId64, i);
        return *this;
    }

    Logger &operator<<(uint64_t i) {
        printf("%" PRIu64, i);
        return *this;
    }

    Logger &operator<<(ull i) {
        printf("%lld", i);
        return *this;
    }

    Logger &operator<<(float v) {
        printf("%.6f", v);
        return *this;
    }

    Logger &operator<<(double v) {
        printf("%.6f", v);
        return *this;
    }

    template <typename T, long unsigned int S> Logger &operator<<(const Array<T, S> &vs) {
        printf("[");
        auto first = true;
        for (auto v : vs) {
            if (!first) {
                printf(", ");
            } else {
                first = false;
            }
            *this << v;
        }
        printf("]");
        return *this;
    }

    template <typename T> Logger &operator<<(Vector<T> vs) {
        printf("[");
        auto first = true;
        for (auto v : vs) {
            if (!first) {
                printf(", ");
            } else {
                first = false;
            }
            *this << v;
        }
        printf("]");
        return *this;
    }

    template <typename A, typename B> Logger &operator<<(Map<A, B> map) {
        printf("[");
        auto first = true;
        for (auto kv : map) {
            if (!first) {
                printf(", ");
            } else {
                first = false;
            }
            *this << kv.first << " => " << kv.second;
        }
        printf("]");
        return *this;
    }

    Logger &operator<<(Coord c) {
        printf("(%d, %d)", c.x, c.y);
        return *this;
    }

    Logger &operator<<(Point3<int> c) {
        printf("(%d, %d, %d)", c.x, c.y, c.z);
        return *this;
    }

    template <typename T> Logger &operator<<(Point3<T> c) {
        printf("(");
        *this << c.x;
        printf(", ");
        *this << c.y;
        printf(", ");
        *this << c.z;
        printf(")");
        return *this;
    }

    template <typename A, typename B> Logger &operator<<(Pair<A, B> c) {
        printf("(");
        *this << c.first;
        printf(", ");
        *this << c.second;
        printf(")");
        return *this;
    }

    template <typename T> Logger &operator<<(Set<T> vs) {
        printf("[");
        auto first = true;
        for (auto v : vs) {
            if (!first) {
                printf(", ");
            } else {
                first = false;
            }
            *this << v;
        }
        printf("]");
        return *this;
    }

    template <typename T> Logger &operator<<(List<T> vs) {
        printf("[");
        auto first = true;
        for (auto v : vs) {
            if (!first) {
                printf(", ");
            } else {
                first = false;
            }
            *this << v;
        }
        printf("]");
        return *this;
    }

    static inline Logger &getInstance() {
        static Logger logger;
        return logger;
    }
};

#define log Logger::getInstance()
#define endl '\n'

namespace std::ranges {

namespace views {
auto to_string_views = rv::transform([](auto range) { return getStringView(range); });

auto filter_empty = rv::filter([](auto range) {
    auto line = getStringView(range);
    return line.size() > 0;
});

auto to_ints = rv::transform([](auto range) {
    auto line = getStringView(range);
    return parseInt(line);
});

auto to_int32s = rv::transform([](auto range) {
    auto line = getStringView(range);
    return parseInt32(line);
});

auto split_string_view = [](auto delim) { return rv::split(delim) | to_string_views; };

template <typename T> auto __convert(T v) {
    return StringView(v);
}
template <> auto __convert(char v) {
    return v;
}

auto split_sv = [](auto delim) { return rv::split(__convert(delim)) | to_string_views; };

template <int index> struct __get {};
inline constexpr __get<0> get0{};
inline constexpr __get<1> get1{};
inline constexpr __get<2> get2{};
inline constexpr __get<3> get3{};
inline constexpr __get<4> get4{};
inline constexpr __get<5> get5{};

template <input_range _Range, int index> auto operator|(_Range &&__r, __get<index> s) {
    auto it = (__r.begin());

    if constexpr (index > 0) {
        it = std::next(it, index);
    }

    return *it;
}

struct __to_string {};
inline constexpr __to_string to_string{};
template <input_range _Range> auto operator|(_Range &&__r, __to_string s) {
    String str;

#ifdef __circle_lang__
    puts("circle does not support std::ranges::views::to_string");
    throw "unsupported";
#else
    std::ranges::copy(__r, std::back_inserter(str));
#endif

    return str;
}

struct __to_int {};
inline constexpr __to_int to_int{};
template <input_range _Range> auto operator|(_Range &&__r, __to_int s) {
    auto line = getStringView(__r);
    return parseInt(line);
}

} // namespace views

auto const split_string_view = [](auto view, char delim) { return rs::split_view(view, delim) | rv::to_string_views; };

} // namespace std::ranges

struct Grid {
    StringViews lines;
    int width, height;

    Grid() {
    }

    Grid(auto _lines) {
        parse(_lines);
    }

    inline bool isValid(const Coord coord) const {
        return coord.x >= 0 && coord.x < width && coord.y >= 0 && coord.y < height;
    }

    inline char get(const Coord coord) const {
        return lines[coord.y][coord.x];
    }

    void parse(auto _lines) {
        lines = {};
        rs::copy(_lines, std::back_inserter(lines));
        width = lines[0].size();
        height = lines.size();
    }

    void for_each(auto func) const {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Coord coord{x, y};
                char c = lines[y][x];

                if constexpr (std::is_same_v<decltype(func(coord, c)), bool>) {
                    if (!func(coord, c)) {
                        return;
                    }
                } else {
                    func(coord, c);
                }
            }
        }
    }

    Vector<Coord> genCoords() const {
        Vector<Coord> result;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                result.push_back({x, y});
            }
        }
        return result;
    }
};
