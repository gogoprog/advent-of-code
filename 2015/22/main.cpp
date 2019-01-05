#include "../../common.h"

struct Effect {
    int id{0};
    int time{0};
    int armor{0};
    int damage{0};
    int mana{0};
};

struct Spell {
    String name;
    int cost;
    int damage;
    int hp;

    Effect effect;
};

struct Character {
    String name;
    int hp;
    int damage{0};
    int armor{0};
    int mana{0};
    int totalManaCost{0};

    Vector<Effect> effects;

    int getTotalArmor() const { return armor; }

    void attack(int finalDamage, Character &defender) {
        finalDamage -= defender.getTotalArmor();
        finalDamage = std::max(finalDamage, 1);

        defender.hp -= finalDamage;

        /* log << name << " does " << finalDamage << endl; */
    }

    bool hasEffect(int id) const {
        for (auto &effect : effects) {
            if (effect.id == id) {
                return true;
            }
        }

        return false;
    }

    void applyEffects(Character &other) {
        armor = 0;

        for (int i{(int)effects.size() - 1}; i >= 0; --i) {
            auto &effect = effects[i];

            if (effect.damage) {
                attack(effect.damage, other);
            }

            if (effect.mana) {
                mana += effect.mana;
            }

            if (effect.armor) {
                armor += effect.armor;
            }

            effect.time -= 1;

            if (effect.time <= 0) {
                effects.erase(effects.begin() + i);
            }
        }
    }

    bool cast(const Spell &spell, Character &other) {
        if (mana < spell.cost) {
            return false;
        }

        mana -= spell.cost;
        totalManaCost += spell.cost;

        if (spell.effect.id) {
            if (hasEffect(spell.effect.id)) {
                return false;
            } else {
                /* log << "Added effect " << spell.name << endl; */
                effects.push_back(spell.effect);
            }
        } else {
            hp += spell.hp;
            attack(spell.damage, other);
        }

        return true;
    }

    bool checkWin(Character &other, int &minCost) {
        if (other.hp <= 0) {
            /* log << "won" << endl; */
            if (totalManaCost < minCost) {
                minCost = totalManaCost;
            }
            return true;
        }
        return false;
    }
};

void turn(const Vector<Spell> &spells, Character initialA, Character initialB, int &minCost, const bool hard) {

    if (hard) {
        --initialA.hp;

        if (initialA.hp <= 0) {
            return;
        }
    }

    initialA.applyEffects(initialB);

    if (initialA.checkWin(initialB, minCost)) {
        return;
    }

    for (const auto &spell : spells) {
        Character a{initialA};
        Character b{initialB};

        if (!a.cast(spell, b)) {
            continue;
        } else {
            if (a.totalManaCost >= minCost) {
                continue;
            }
        }

        if (a.checkWin(b, minCost)) {
            continue;
        }

        if (hard) {
            --a.hp;

            if (a.hp <= 0) {
                continue;
            }
        }

        a.applyEffects(b);

        if (a.checkWin(b, minCost)) {
            continue;
        }

        b.attack(b.damage, a);

        if (a.hp <= 0) {
            continue;
        }

        turn(spells, a, b, minCost, hard);
    }
}

int main() {
    Character player{"Player", 50, 0, 0, 500};
    Character boss{"Boss", 58, 9, 0, 0};

    Vector<Spell> spells;

    spells.push_back({"Magic Missile", 53, 4, 0});
    spells.push_back({"Drain", 73, 2, 2});
    spells.push_back({"Shield", 113, 0, 0, {1, 6, 7, 0, 0}});
    spells.push_back({"Poison", 173, 0, 0, {2, 6, 0, 3, 0}});
    spells.push_back({"Recharge", 229, 0, 0, {3, 5, 0, 0, 101}});

    {
        int minCost{10000000};
        turn(spells, player, boss, minCost, false);

        log << "part1: " << minCost << endl;
    }
    {

        int minCost{10000000};
        turn(spells, player, boss, minCost, true);

        log << "part2: " << minCost << endl;
    }

    return 0;
}
