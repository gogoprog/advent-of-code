#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

void process(const char *file, const int workersCount, const int extraTime = 0) {
    std::ifstream f(file);
    std::string line, str;
    std::map<char, std::vector<char>> requirements{};
    std::set<char> steps;

    while (std::getline(f, line)) {
        std::istringstream iss{line};
        char a, b;

        iss >> str >> a >> str >> str >> str >> str >> str >> b;

        requirements[b].push_back(a);

        steps.insert(a);
        steps.insert(b);
    }
    f.close();

    std::vector<char> finishedSteps;

    std::cout << "part1: ";

    while (finishedSteps.size() < steps.size()) {
        for (auto s : steps) {
            if (std::find(finishedSteps.begin(), finishedSteps.end(), s) == finishedSteps.end()) {

                auto &reqs = requirements[s];

                bool r = std::all_of(reqs.begin(), reqs.end(), [&](char c) {
                    return std::find(finishedSteps.begin(), finishedSteps.end(), c) != finishedSteps.end();
                });

                if (r) {
                    finishedSteps.push_back(s);
                    std::cout << s;
                    break;
                }
            }
        }
    }

    std::cout << std::endl;

    finishedSteps.clear();

    struct Worker {
        char target;
        int timeLeft{0};
    };
    std::vector<Worker> workers(workersCount);
    std::set<char> busySteps;

    int totalTime{0};

    std::cout << "part2: ";

    while (finishedSteps.size() < steps.size()) {
        for (auto &w : workers) {
            if (w.timeLeft == 0) {
                for (auto s : steps) {
                    if (std::find(finishedSteps.begin(), finishedSteps.end(), s) == finishedSteps.end()) {
                        if (std::find(busySteps.begin(), busySteps.end(), s) == busySteps.end()) {

                            const auto &reqs = requirements[s];

                            bool r = std::all_of(reqs.begin(), reqs.end(), [&](char c) {
                                return std::find(finishedSteps.begin(), finishedSteps.end(), c) != finishedSteps.end();
                            });

                            if (r) {
                                busySteps.insert(s);
                                w.timeLeft = s - 'A' + 1 + extraTime;
                                w.target = s;
                                break;
                            }
                        }
                    }
                }
            }
        }
        for (auto &w : workers) {
            if (w.timeLeft > 0) {
                --w.timeLeft;
                if (w.timeLeft == 0) {
                    finishedSteps.push_back(w.target);
                    busySteps.erase(w.target);
                }
            }
        }

        ++totalTime;
    }

    for (auto i : finishedSteps)
        std::cout << i;
    std::cout << std::endl;

    std::cout << totalTime << std::endl;
}

int main() {
    process("input2.txt", 2);
    process("input.txt", 5, 60);

    return 0;
}
