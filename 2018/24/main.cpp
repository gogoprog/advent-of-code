#include <algorithm>
#include <cstring>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <vector>

template <class K, class V> using Map = std::map<K, V>;
template <class V> using Vector = std::vector<V>;
template <class V> using Set = std::set<V>;
template <class T> using Grid = Map<int, Map<int, T>>;
using ull = unsigned long long;

enum class Team { ImmuneSystem, Infection, None };

std::ostream &operator<<(std::ostream &out, const Team team) {
    switch (team) {
    case Team::ImmuneSystem:
        out << "[ImmuneSystem]";
        break;

    case Team::Infection:
        out << "[Infection]";
        break;
    }
    return out;
}

struct Group {
    Team team;
    int units;
    int initialUnits;
    int unitHp;
    int attackDamage;
    int initialAttackDamage;
    char attackType;
    int initiative;
    Set<char> immunities;
    Set<char> weaknesses;

    Group *target{nullptr};
    bool chosen;

    int effectivePower() const { return units * attackDamage; }

    int attack() {
        if (target != nullptr) {
            int totalDamage = computeDamage(*target);

            int losses = totalDamage / target->unitHp;

            target->units -= losses;
            /* std::cout << team << " attacks " << target->team << " with " << totalDamage << '(' << losses << ')' */
            /*           << std::endl; */

            return losses;
        }
        return 0;
    }

    int computeDamage(const Group &other) const {
        if (other.immunities.find(attackType) != other.immunities.end()) {
            return 0;
        }

        int factor{1};
        if (other.weaknesses.find(attackType) != other.weaknesses.end()) {
            factor = 2;
        }

        int totalDamage = effectivePower() * factor;
        return totalDamage;
    }

    void parse(std::istringstream &iss) {
        std::string str;
        iss >> units >> str >> str >> str >> unitHp >> str >> str;

        initialUnits = units;

        if (iss.str().find('(') != std::string::npos) {

            std::getline(iss, str, '(');
            std::getline(iss, str, ')');
            std::istringstream iss2{str};

            std::string str2;
            while (std::getline(iss2, str2, ';')) {
                std::istringstream iss3{str2};
                std::string type;
                iss3 >> type >> str;
                Set<char> *list;
                if (type[0] == 'w') {
                    list = &weaknesses;
                } else {
                    list = &immunities;
                }
                std::istringstream iss4{str2.substr(iss3.tellg())};
                while (std::getline(iss4, str, ',')) {
                    list->insert(str[1]);
                }
            }
        }

        iss >> str >> str >> str >> str >> str >> attackDamage >> str;
        initialAttackDamage = attackDamage;
        attackType = str[0];
        iss >> str >> str >> str >> initiative;
    }

    void setTarget(Vector<Group> &groups) {
        int bestDamage{0};
        Group *best{nullptr};

        for (auto &other : groups) {
            if (other.team != team && !other.chosen && other.units > 0) {
                int ad = computeDamage(other);

                if (ad > bestDamage) {
                    bestDamage = ad;
                    best = &other;
                } else if (best && ad == bestDamage) {
                    int bep = best->effectivePower();
                    int oep = other.effectivePower();
                    if (oep > bep) {
                        best = &other;
                    } else if (oep == bep) {
                        if (other.initiative > best->initiative) {
                            best = &other;
                        }
                    }
                }
            }
        }

        target = best;
        if (target) {
            target->chosen = true;
        }
    }

    void prepare() {
        chosen = false;
        target = nullptr;
    }

    void reset(const int boost) {
        units = initialUnits;
        attackDamage = initialAttackDamage + boost;
    }
};

struct Context {
    Set<Team> aliveTeams;
    Vector<Group> groups;
    Team lastWinner;

    void battle() {
        bool ended{false};
        Vector<int> orderedIndexes(groups.size());
        std::iota(orderedIndexes.begin(), orderedIndexes.end(), 0);
        while (!ended) {
            for (auto &group : groups) {
                group.prepare();
            }

            std::sort(orderedIndexes.begin(), orderedIndexes.end(), [&](int a, int b) {
                auto &ga = groups[a];
                auto &gb = groups[b];
                int ea = ga.effectivePower();
                int eb = gb.effectivePower();

                if (ea > eb) {
                    return true;
                } else if (ea < eb) {
                    return false;
                } else {
                    return ga.initiative > gb.initiative;
                }
            });

            for (auto i : orderedIndexes) {
                auto &group = groups[i];
                if (group.units > 0) {
                    group.setTarget(groups);
                }
            }

            std::sort(orderedIndexes.begin(), orderedIndexes.end(), [&](int a, int b) {
                auto &ga = groups[a];
                auto &gb = groups[b];
                return ga.initiative > gb.initiative;
            });

            int losses{0};
            for (auto i : orderedIndexes) {
                auto &group = groups[i];
                if (group.units > 0) {
                    losses += group.attack();
                }
            }

            if(losses == 0) {
                lastWinner = Team::None;
                return;
            }

            aliveTeams.clear();
            for (auto &group : groups) {
                if (group.units > 0) {
                    aliveTeams.insert(group.team);
                }
            }

            ended = aliveTeams.size() < 2;

            /* log(); */
        }

        lastWinner = *aliveTeams.begin();
    }

    int unitsLeft() const {

        int r{0};
        for (auto &group : groups) {
            if (group.units > 0) {
                r += group.units;
            }
        }

        return r;
    }

    void log() {
        std::cout << "---" << std::endl;
        for (auto &group : groups) {
            std::cout << group.team << " has " << group.units << std::endl;
        }
    }

    void reset(const int boost) {
        lastWinner = Team::None;
        for (auto &group : groups) {
            group.reset(group.team == Team::ImmuneSystem ? boost : 0);
        }
    }
};

void process(const char *file) {
    std::ifstream f(file);
    std::string line;
    Team currentTeam;
    Context ctx;

    while (std::getline(f, line)) {
        std::istringstream iss{line};

        if (line == "Immune System:") {
            currentTeam = Team::ImmuneSystem;
        } else if (line == "Infection:") {
            currentTeam = Team::Infection;
        } else {
            if (line.length() > 3) {
                ctx.groups.push_back({currentTeam});
                auto &group = ctx.groups.back();
                group.parse(iss);
            }
        }
    }

    f.close();

    ctx.battle();

    std::cout << "part1: " << ctx.unitsLeft() << std::endl;

    int boost{0};
    Team winner;

    while (ctx.lastWinner != Team::ImmuneSystem) {
        ctx.reset(boost);
        ++boost;
        ctx.battle();
    }

    std::cout << "part2: " << ctx.unitsLeft() << " with boost=" << boost << std::endl;
}

int main(int argc, char *argv[]) {
    process(argv[1]);

    return 0;
}
