#include <algorithm>
#include <array>
#include <cmath>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
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
template <class K, class V> using UnorderedMap = std::unordered_map<K, V>;
template <class V> using Vector = std::vector<V>;
template <class V, int size> using Array = std::array<V, size>;
template <class V> using Set = std::set<V>;
template <class V> using UnorderedSet = std::unordered_set<V>;
template <class T> using Grid = Map<int, Map<int, T>>;
template <class V> using Queue = std::queue<V>;
using String = std::string;
using InputStringStream = std::istringstream;
using ull = unsigned long long;
template <class S> using Function = std::function<S>;

template <typename T> std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
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

Vector<String> splitString(const String &input, const char delim = '\n') {
    Vector<String> result;
    std::istringstream iss(input);
    String line;

    while (std::getline(iss, line, delim)) {
        result.push_back(line);
    }

    return result;
}
