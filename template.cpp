#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);
}

int main() {
    process("input.txt");
    return 0;
}
