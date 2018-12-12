#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return ch != '.'; }).base(), s.end());
}

void process(const char *file, const long long gens) {
    std::ifstream f(file);
    std::string line, str, str2;
    char c;

    std::string initial;
    std::map<std::string, char> patterns;

    {
        std::getline(f, line);
        std::istringstream iss{line};

        iss.ignore(15);
        iss >> initial;
    }

    std::getline(f, line);

    while (std::getline(f, line)) {
        std::istringstream iss{line};

        iss >> str >> str2 >> c;

        patterns[str] = c;
    }

    f.close();

    std::string current{initial};
    long long offset{0};

    std::map<std::string, int> plants;

    for (long long i{0}; i < gens; ++i) {

        current = "....." + current + ".....";
        offset += 5;
        int len = current.length();
        std::string next(len, '.');
        for (int c{2}; c < len - 4; ++c) {
            std::string pattern = current.substr(c - 2, 5);

            for (auto &kv : patterns) {
                if (pattern == kv.first) {
                    next[c] = kv.second;
                }
            }
        }

        rtrim(next);

        while (next[0] == '.') {
            next.erase(0, 1);
            --offset;
        }

        current = next;

        if (plants.find(current) != plants.end()) {
            std::cout << plants.find(current)->second << std::endl;
            std::cout << i + 1 << std::endl;
            offset -= (gens - i - 1);
            break;
        } else {
            plants[current] = i + 1;
        }
    }

    std::cout << "offset: " << offset << std::endl;
    long long result{0};
    for (int c{0}; c < current.length(); ++c) {
        if (current[c] == '#') {
            result += (c - offset);
        }
    }
    std::cout << "total: " << result << std::endl;
}

int main() {
    process("input2.txt", 20);
    process("input.txt", 20);

    process("input.txt", 50000000000);

    return 0;
}
