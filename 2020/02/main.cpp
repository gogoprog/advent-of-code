#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    int count{0};
    int count2{0};

    for(const auto & line : lines) {
        InputStringStream iss{line};

        int a, b;
        char t, c;
        iss >> a >> t >> b >> c >> t;

        String str;
        iss >> str;


        int found = std::count(str.begin(), str.end(), c);

        if(found >= a && found <= b) {
            ++count;
        }

        int check = (str[a-1] == c ? 1 : 0) + (str[b-1] == c ? 1 : 0);

        if(check == 1) {
            ++count2;
        }

    }

    log << "Part1: " << count << endl;
    log << "Part2: " << count2 << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
