#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

struct Rect {
    int x, y, w, h;
};

int main() {

    std::ifstream f("input.txt");
    std::string line, str;
    std::vector<Rect> rects;
    Rect rect;
    char c;
    std::map<int, std::map<int, int>> claims;

    while (std::getline(f, line)) {
        std::istringstream iss(line);

        iss >> str >> c >> rect.x >> c >> rect.y >> c >> rect.w >> c >> rect.h;

        for (int x = rect.x; x < rect.x + rect.w; ++x) {
            for (int y = rect.y; y < rect.y + rect.h; ++y) {
                claims[x][y]++;
            }
        }

        rects.push_back(rect);
    }
    f.close();

    int r{0};

    for (auto x = claims.begin(); x != claims.end(); ++x) {
        auto &claims_line = x->second;
        for (auto y = claims_line.begin(); y != claims_line.end(); ++y) {
            if (y->second > 1) {
                ++r;
            }
        }
    }

    std::cout << r << std::endl;

    r = 1;

    for(auto & rect : rects) {
        bool good{true};
        for (int x = rect.x; x < rect.x + rect.w; ++x) {
            for (int y = rect.y; y < rect.y + rect.h; ++y) {
                if (claims[x][y] != 1 ) {
                    good = false;
                    break;
                }
            }
        }

        if(good) {
            std::cout << "Yes: " << r << std::endl;
        }
        ++r;
    }
}
