#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using Time = std::tm;

struct Guard {
    Guard() { std::fill(minutes.begin(), minutes.end(), 0); }

    void begin(const Time &time) {
        currentTime = time;
        sleeping = false;
    }

    void end(const Time &time) {}

    void sleep(const Time &time) {
        currentTime = time;
        sleeping = true;
    }

    void wakeUp(const Time &time) {
        auto diff = time.tm_min - currentTime.tm_min;
        sleeping = false;
        sleepTime += diff;
        for (int m = currentTime.tm_min; m < time.tm_min; ++m) {
            minutes[m] = minutes[m] + 1;
        }
        currentTime = time;
    }

    int getMostSleepMinute() const {
        return std::distance(minutes.begin(), std::max_element(minutes.begin(), minutes.end()));
    }

    int getMostSleepMinuteDuration() const { return *std::max_element(minutes.begin(), minutes.end()); }

    Time currentTime{};
    bool sleeping{false};
    int sleepTime{0};
    std::array<int, 60> minutes;
};

int main() {

    std::ifstream f("input.txt");
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(f, line)) {
        lines.push_back(line);
    }
    f.close();

    std::sort(lines.begin(), lines.end());

    int current{0};
    std::map<int, Guard> guards;

    for (const auto &line : lines) {
        std::string str, str2;
        char c;
        std::tm tm{};
        std::istringstream iss{line};

        iss >> std::get_time(&tm, "[%Y-%m-%d %H:%M]") >> str >> str2;
        auto t = tm;

        if (str == "Guard") {
            if (current) {
                guards[current].end(t);
            }

            std::istringstream iss2{str2};
            iss2 >> c >> current;

            guards[current].begin(t);

        } else if (str == "falls") {
            guards[current].sleep(t);
        } else if (str == "wakes") {
            guards[current].wakeUp(t);
        }
    }

    {
        auto it = std::max_element(guards.begin(), guards.end(), [](const auto &a, const auto &b) {
            return a.second.sleepTime < b.second.sleepTime;
        });

        auto &bestSleeper = (*it).second;
        auto id = (*it).first;
        auto m = (*it).second.getMostSleepMinute();

        std::cout << "part1: " << id * m << std::endl;
    }
    {
        auto it = std::max_element(guards.begin(), guards.end(), [](const auto &a, const auto &b) {
            return a.second.getMostSleepMinuteDuration() < b.second.getMostSleepMinuteDuration();
        });

        auto &bestSleeper = (*it).second;
        auto id = (*it).first;
        auto m = (*it).second.getMostSleepMinute();

        std::cout << "part2: " << id * m << std::endl;
    }
}
