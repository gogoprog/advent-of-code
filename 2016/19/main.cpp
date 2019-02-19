#include "../../common.h"

struct Context {
    int elfCount;

    void part1() {
        Vector<int> presents;
        presents.resize(elfCount);
        std::fill(presents.begin(), presents.end(), 1);

        int current = 0;

        while (true) {
            int &currentCount = presents[current];
            int next = current + 1;
            next %= elfCount;

            if (currentCount) {
                while (!presents[next]) {
                    next++;
                    next %= elfCount;
                }
                currentCount += presents[next];
                presents[next] = 0;

                if (currentCount == elfCount) {
                    log << "part1 winner is " << current + 1 << endl;
                    break;
                }
            }

            current = next;
        }
    }

    void part2() {
        Vector<int> presents;
        std::deque<int> elves;
        presents.resize(elfCount);
        elves.resize(elfCount);
        std::fill(presents.begin(), presents.end(), 1);
        std::generate(elves.begin(), elves.end(), [n = 0]() mutable { return n++; });

        int current = 0;

        while (true) {
            int &currentCount = presents[current];

            if (currentCount) {
                auto currentPos = std::lower_bound(elves.begin(), elves.end(), current);
                int index = std::distance(elves.begin(), currentPos);

                index += elves.size() / 2;
                index %= elves.size();

                int next = elves[index];

                currentCount += presents[next];
                presents[next] = 0;
                elves.erase(elves.begin() + index);

                if (currentCount == elfCount) {
                    log << "part2 winner is " << current + 1 << endl;
                    break;
                }
            }

            current++;
            current %= elfCount;
        }
    }
};

void process(const int input) {
    log << "Processing " << input << endl;

    Context ctx{input};

    ctx.part1();
    ctx.part2();
}

int main() {
    process(5);
    process(3001330);
    return 0;
}
