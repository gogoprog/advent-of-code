#include "../../common_fast.h"

enum Type { HighCard, OnePair, TwoPair, ThreeOfAKind, FullHouse, Kind4, Kind5 };

struct Hand {
    StringView cards;
    int bid;

    int getType() const {
        Map<char, int> cardcounts;
        Map<int, int> counts;

        for (auto c : cards) {
            cardcounts[c]++;
        }

        for (auto kv : cardcounts) {
            counts[kv.second]++;
        }

        if (counts[5])
            return Kind5;
        if (counts[4])
            return Kind4;
        if (counts[3] && counts[2])
            return FullHouse;
        if (counts[3])
            return ThreeOfAKind;
        if (counts[2] == 2)
            return TwoPair;
        if (counts[2] == 1)
            return OnePair;

        return HighCard;
    }

    int getType2() const {
        Map<char, int> cardcounts;
        Map<int, int> counts;

        for (auto c : cards) {
            cardcounts[c]++;
        }

        int jokers = cardcounts['J'];

        if (jokers == 5)
            return Kind5;

        cardcounts['J'] = 0;

        for (int i = 4; i >= 1; --i) {
            for (auto kv : cardcounts) {
                if (kv.second == i) {
                    cardcounts[kv.first] += jokers;
                    i = 0;
                    break;
                }
            }
        }

        for (auto kv : cardcounts) {
            counts[kv.second]++;
        }

        if (counts[5])
            return Kind5;
        if (counts[4])
            return Kind4;
        if (counts[3] && counts[2])
            return FullHouse;
        if (counts[3])
            return ThreeOfAKind;
        if (counts[2] == 2)
            return TwoPair;
        if (counts[2] == 1)
            return OnePair;

        return HighCard;
    }
};

struct Context {

    void part1(auto lines) {
        ull result{0};

        auto to_hands = rv::transform([](auto line) {
            Hand hand;
            hand.cards = line | rv::split_sv(' ') | rv::get0;
            hand.bid = line | rv::split_sv(' ') | rv::get1 | rv::to_int;

            return hand;
        });

        auto view = lines | to_hands;

        Vector<Hand> ranked_hands;

        rs::copy(view, std::back_inserter(ranked_hands));

        std::sort(ranked_hands.begin(), ranked_hands.end(), [](auto a, auto b) {
            static String strengths = "23456789TJQKA";
            auto typea = a.getType();
            auto typeb = b.getType();

            if (typea == typeb) {
                for (auto c = 0; c < 5; c++) {
                    if (a.cards[c] != b.cards[c]) {
                        auto astrength = strengths.find(a.cards[c]);
                        auto bstrength = strengths.find(b.cards[c]);
                        return astrength < bstrength;
                    }
                }
            }

            return typea < typeb;
        });

        for (int i = 0; i < ranked_hands.size(); i++) {
            result += ranked_hands[i].bid * (i + 1);
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        ull result{0};

        auto to_hands = rv::transform([](auto line) {
            Hand hand;
            hand.cards = line | rv::split_sv(' ') | rv::get0;
            hand.bid = line | rv::split_sv(' ') | rv::get1 | rv::to_int;

            return hand;
        });

        auto view = lines | to_hands;

        Vector<Hand> ranked_hands;

        rs::copy(view, std::back_inserter(ranked_hands));

        std::sort(ranked_hands.begin(), ranked_hands.end(), [](auto a, auto b) {
            static String strengths = "J23456789TQKA";
            auto typea = a.getType2();
            auto typeb = b.getType2();

            if (typea == typeb) {
                for (auto c = 0; c < 5; c++) {
                    if (a.cards[c] != b.cards[c]) {
                        auto astrength = strengths.find(a.cards[c]);
                        auto bstrength = strengths.find(b.cards[c]);
                        return astrength < bstrength;
                    }
                }
            }

            return typea < typeb;
        });

        for (int i = 0; i < ranked_hands.size(); i++) {
            result += ranked_hands[i].bid * (i + 1);
        }

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = rs::split_string_view(getFileContent(filename), '\n');
    {
        Context context;
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
