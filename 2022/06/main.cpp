#include "../../common.h"

bool allDifferent(const List<char> &list) {
    Set<char> temp;

    for (auto c : list) {
        if (temp.find(c) != temp.end()) {
            return false;
        }

        temp.insert(c);
    }

    return true;
}

template <int count> void detect(const String line) {
    List<char> chars;

    for (int i = 0; i < count - 1; ++i) {
        chars.push_back(line[i]);
    }

    int r = -1;

    for (int i = count - 1; i < line.length(); ++i) {
        chars.push_back(line[i]);

        if (allDifferent(chars)) {
            r = i;
            break;
        }

        chars.pop_front();
    }

    log << "Result: " << r + 1 << endl;
}

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    for (auto line : lines) {
        detect<4>(line);
        detect<14>(line);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
