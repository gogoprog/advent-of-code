#include "../../common.h"

template <lli multiplier> struct Context {

    Vector<lli> numbers;
    Vector<lli> initialNumbers;
    Vector<int> positions;
    lli size;

    void parse(const Strings &lines) {
        size = lines.size();

        for (auto &line : lines) {
            lli number = convert<String, int>(line) * multiplier;
            initialNumbers.push_back(number);

            number %= size - 1;

            numbers.push_back(number);
            positions.push_back(positions.size());
        }
    }

    void apply(int index) {
        index = index % size;

        auto it = std::find(positions.begin(), positions.end(), index);
        auto current_index = it - positions.begin();
        lli value = numbers[index];
        lli new_index = current_index + value;

        while (new_index < 0) {
            new_index += size - 1;
        }

        while (new_index >= size) {
            new_index -= size - 1;
        }

        if (new_index >= current_index) {
            positions.erase(it);
            positions.insert(positions.begin() + new_index, index);
        } else {
            positions.erase(it);
            positions.insert(positions.begin() + new_index, index);
        }
    }

    void print() {
        for (auto pos : positions) {
            log << numbers[pos] << ", ";
        }

        log << endl;
    }

    lli getNumber(const int index) {
        return initialNumbers[positions[index % size]];
    }

    void part1() {
        auto result{0};

        for (int i = 0; i < size; i++) {
            apply(i);
        }

        auto zit = std::find(numbers.begin(), numbers.end(), 0);
        auto zindex = zit - numbers.begin();

        auto it = std::find(positions.begin(), positions.end(), zindex);
        auto index = it - positions.begin();

        auto a = getNumber(index + 1000);
        auto b = getNumber(index + 2000);
        auto c = getNumber(index + 3000);

        result = a + b + c;

        log << "Part1: " << result << endl;
    }

    void part2() {
        lli result{0};

        for (int i = 0; i < size * 10; i++) {
            apply(i);
        }

        auto zit = std::find(numbers.begin(), numbers.end(), 0);
        auto zindex = zit - numbers.begin();

        auto it = std::find(positions.begin(), positions.end(), zindex);
        auto index = it - positions.begin();

        auto a = getNumber(index + 1000);
        auto b = getNumber(index + 2000);
        auto c = getNumber(index + 3000);

        result = a + b + c;

        log << "Part2: " << result << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    {
        Context<1> context;
        context.parse(lines);
        context.part1();
    }
    {
        Context<811589153> context;
        context.parse(lines);
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
