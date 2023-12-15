
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <charconv>
#include <cmath>
#include <deque>
#include <functional>
#include <initializer_list>
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
template <class T> using Grid = Map<int, Map<int, T>>;
template <class V> using Queue = std::queue<V>;
template <class V> using Dequeu = std::deque<V>;
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

struct Point {
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

    return StringView(buffer, length);
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
            throw "No key found";
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

    Logger &operator<<(uint64_t i) {
        printf("%ld", i);
        return *this;
    }

    Logger &operator<<(long i) {
        printf("%ld", i);
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

    template <typename A,typename B> Logger &operator<<(Map<A, B> map) {
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

template <input_range _Range, int index> auto operator|(_Range &&__r, __get<index> s) {
    auto it = (__r.begin());

    for (int i = 0; i < index; i++) {
        it = std::next(it);
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
