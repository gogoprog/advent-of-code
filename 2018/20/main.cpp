#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

template <class K, class V> using Map = std::map<K, V>;
template <class V> using Vector = std::vector<V>;
template <class T> using Grid = Map<int, Map<int, T>>;

void process(const char *file) {
    std::ifstream f(file);
    std::string line;
    std::string str;

    while (std::getline(f, line)) {
        std::istringstream iss{line};
    }

    f.close();

    vm.run(ip, 1);
}

int main(int argc, char *argv[]) {
    process(argv[1]);

    return 0;
}
