#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
    auto r = 0;

    log << r << endl;
}

int main() {
    process("input.txt");
    return 0;
}
