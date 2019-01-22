#include "../../common.h"

bool isAbba(const String in) {
    if (in[0] != in[1]) {
        if (in[0] == in[3] && in[1] == in[2]) {
            return true;
        }
    }

    return false;
}

bool isAba(const String in) { return in[0] != in[1] && in[0] == in[2]; }

bool supportsTLS(const String line) {
    String word;
    bool inbrackets{false};
    bool inside{false};
    bool outside{false};
    for (auto c : line) {

        if (c == '[') {
            inbrackets = true;
            word = "";
        } else if (c == ']') {
            inbrackets = false;
            word = "";
        } else {

            word += c;

            if (word.size() > 4) {
                word.erase(0, 1);
            }

            if (word.size() == 4) {
                if (isAbba(word)) {

                    if (inbrackets) {
                        inside = true;
                    } else {
                        outside = true;
                    }
                }
            }
        }
    }

    return outside && !inside;
}

bool supportsSSL(const String line) {
    String word;
    bool inbrackets{false};

    Vector<String> inAbas;
    Vector<String> outAbas;

    for (auto c : line) {

        if (c == '[') {
            inbrackets = true;
            word = "";
        } else if (c == ']') {
            inbrackets = false;
            word = "";
        } else {

            word += c;

            if (word.size() > 3) {
                word.erase(0, 1);
            }

            if (word.size() == 3) {
                if (isAba(word)) {

                    if (inbrackets) {
                        inAbas.push_back(word);
                    } else {
                        outAbas.push_back(word);
                    }
                }
            }
        }
    }

    for (auto inAba : inAbas) {
        for (auto outAba : outAbas) {
            if(inAba[0] == outAba[1]){
                if(inAba[1] == outAba[0]){
                    return true;
                }
            }
        }
    }

    return false;
}

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    int count{0};
    int count2{0};
    for (auto line : lines) {
        if (supportsTLS(line)) {
            count++;
        }
        if (supportsSSL(line)) {

            count2++;
        }
    }

    log << "part1: " << count << endl;
    log << "part2: " << count2 << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
