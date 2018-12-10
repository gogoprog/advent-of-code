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

struct Marble {};

void process(const int playerCount, const int last) {
    std::vector<int> marbles{0};
    std::vector<int> scores(playerCount);
    int currentIndex{0};
    int m{0};

    int t{0};

    while (m <= last) {
        for (int p{0}; p < playerCount; ++p) {
            ++m;

            if (m > last) {
                break;
            }

            if (m % 23 == 0) {
                scores[p] += m;
                int other = currentIndex - 7;
                while (other < 0) {
                    other += marbles.size();
                }

                /* std::cout << p << " wins " << marbles[other] << " at " << m << " t: " << t << std::endl; */

                scores[p] += marbles[other];
                marbles.erase(marbles.begin() + other);
                currentIndex = other;

                /* std::rotate(marbles.begin(), marbles.begin() + currentIndex, marbles.end()); */
                /* currentIndex = 0; */
                t++;

            } else {

                currentIndex += 2;

                if (currentIndex >= marbles.size()) {
                    currentIndex %= marbles.size();

                    /* if (marbles.size() > 8) { */
                        /* std::rotate(marbles.rbegin(), marbles.rbegin() + 8, marbles.rend()); */
                        /* currentIndex += 8; */
                    /* } */
                }

                marbles.insert(marbles.begin() + currentIndex, m);

                /* if (marbles.size() > 23 && currentIndex > 8) { */

                /*     std::rotate(marbles.begin(), marbles.begin() + currentIndex - 8, marbles.end()); */
                /*     currentIndex = 8; */
                /* } */
            }

#if 0
            std::cout << currentIndex << " " << marbles.size() << std::endl;
#endif

#if 0
            for (auto ma{0}; ma < marbles.size(); ++ma) {
                if (ma == currentIndex) {

                    std::cout << "(" << marbles[ma] << ")";
                } else {

                    std::cout << " " << marbles[ma] << " ";
                }
            }
            std::cout << std::endl;
#endif
        }
    }

    auto it = std::max_element(scores.begin(), scores.end());
    int maxp = it - scores.begin();

    std::cout << "best is " << maxp << " with " << scores[maxp] << std::endl;
}

int main() {
    process(9, 25);
    process(10, 1618);
    /* process(13, 7999); */
    /* process(17, 1104); */
    /* process(21, 6111); */
    /* process(30, 5807); */

    process(425, 70848);
    process(425, 7084800);

    return 0;
}
