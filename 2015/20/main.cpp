#include <algorithm>
#include <array>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

int isPrime(int n) {
    bool flag = false;

    for (int i = 2; i <= n / 2; ++i) {
        if (n % i == 0) {
            flag = true;
            break;
        }
    }
    return flag;
}

int main() {
    int h{1};

    while (true) {
        int v{0};
        for (int i{1}; i <= h*50; ++i) {
            if ((h % i) == 0) {
                v += 11 * i;
            }
        }
        if (v >= 33100000) {
            std::cout << "house " << h << " = " << v << std::endl;
            exit(0);
        }
        ++h;
    }

    return 0;
}
