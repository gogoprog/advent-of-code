#include "../../common.h"

struct Item {
    String name;
    int cost;
    int damage;
    int armor;
};

enum class Type { Weapon, Armor, Ring };

struct Config {
    int weaponIndex{0};
    int armorIndex{-1};
    int ring1Index{-1};
    int ring2Index{-1};
};

struct Shop {
    Map<Type, Vector<Item>> items;

    int cost(const Config &config) const {
        int r = items.at(Type::Weapon)[config.weaponIndex].cost;

        if (config.armorIndex != -1) {
            r += items.at(Type::Armor)[config.armorIndex].cost;
        }

        if (config.ring1Index != -1) {
            r += items.at(Type::Ring)[config.ring1Index].cost;
        }

        if (config.ring2Index != -1) {
            r += items.at(Type::Ring)[config.ring2Index].cost;
        }

        return r;
    }
};

struct Context {
    Shop shop;
};

struct Character {
    int hp;
    int damage{0};
    int armor{0};
    Vector<Item> items;

    int getTotalDamage() const {
        return std::accumulate(items.begin(), items.end(), damage, [](int c, const auto &a) { return c + a.damage; });
    }

    int getTotalArmor() const {
        return std::accumulate(items.begin(), items.end(), armor, [](int c, const auto &a) { return c + a.armor; });
    }

    void attack(Character &defender) {
        int finalDamage = getTotalDamage();

        finalDamage -= defender.getTotalArmor();
        finalDamage = std::max(finalDamage, 1);

        defender.hp -= finalDamage;
    }

    void apply(const Shop &shop, const Config &config) {
        items.clear();

        items.push_back(shop.items.at(Type::Weapon)[config.weaponIndex]);

        if (config.armorIndex != -1) {
            items.push_back(shop.items.at(Type::Armor)[config.armorIndex]);
        }

        if (config.ring1Index != -1) {
            items.push_back(shop.items.at(Type::Ring)[config.ring1Index]);
        }

        if (config.ring2Index != -1) {
            items.push_back(shop.items.at(Type::Ring)[config.ring2Index]);
        }
    }
};

bool battle(Character &a, Character &b) {
    while (true) {
        a.attack(b);
        if (b.hp <= 0) {
            return true;
        }

        b.attack(a);
        if (a.hp <= 0) {
            return false;
        }
    }
}

void process(Shop &shop, Character a, Character b) {
    Vector<Config> configs;

    for (int w{0}; w < shop.items[Type::Weapon].size(); ++w) {
        for (int a{-1}; a < (int)shop.items[Type::Armor].size(); ++a) {
            for (int r{-1}; r < (int)shop.items[Type::Ring].size(); ++r) {

                for (int r2{-1}; r2 < (int)shop.items[Type::Ring].size(); ++r2) {
                    if (r != r2 || r == -1) {
                        configs.push_back({w, a, r, r2});
                    }
                }
            }
        }
    }

    int bestCost{1000000};
    int worstCost{0};

    for (auto &config : configs) {
        auto clonea = a;
        auto cloneb = b;
        clonea.apply(shop, config);
        int cost = shop.cost(config);
        if (battle(clonea, cloneb)) {
            if (cost < bestCost) {
                bestCost = cost;
            }
        } else {
            if (cost > worstCost) {
                worstCost = cost;
            }
        }
    }

    log << "part1: " << bestCost << endl;
    log << "part2: " << worstCost << endl;
}

int main() {
    Vector<String> lines = getFileLines("shop.txt");
    Vector<Item> *targetList;
    String str;

    Shop shop;

    for (auto &line : lines) {
        InputStringStream iss{line};

        iss >> str;

        if (str == "Weapons:") {
            targetList = &shop.items[Type::Weapon];
        } else if (str == "Armor:") {
            targetList = &shop.items[Type::Armor];
        } else if (str == "Rings:") {
            targetList = &shop.items[Type::Ring];
        } else if (line.length() > 1) {
            targetList->push_back({str});
            auto &item = targetList->back();
            if (targetList == &shop.items[Type::Ring]) {
                iss >> str;
            }
            iss >> item.cost >> item.damage >> item.armor;
        }
    }

    Character player{100, 0, 0};
    Character boss{103, 9, 2};

    process(shop, player, boss);

    return 0;
}
