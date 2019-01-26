#include "../../common.h"

enum State { REGULAR, MARKER, DATA_SECTION };

String uncompress(const String line, const bool part2) {
    int p{0};
    String result;
    String marker;
    String data_section;
    int marker_length;
    int marker_repeats;

    State state{REGULAR};

    while (p < line.length()) {
        char c = line[p];

        switch (state) {
            case REGULAR: {
                switch (c) {
                    case '(': {
                        state = MARKER;
                        marker = "";
                        break;
                    }
                    default: {
                        result += c;
                        break;
                    }
                }
            } break;

            case MARKER: {
                switch (c) {
                    case ')': {
                        state = DATA_SECTION;
                        data_section = "";
                        InputStringStream iss{marker};
                        char _;
                        iss >> marker_length >> _ >> marker_repeats;
                        break;
                    }
                    default: {
                        marker += c;
                        break;
                    }
                }
                break;
            }

            case DATA_SECTION: {

                data_section += c;
                --marker_length;
                if (marker_length == 0) {
                    if (part2) {
                        data_section = uncompress(data_section, true);
                    }
                    for (int i{0}; i < marker_repeats; ++i) {
                        result += data_section;
                    }

                    state = REGULAR;
                }
                break;
            }
        }

        ++p;
    }

    return result;
}

void process(const String filename, const bool part2) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    for (auto line : lines) {
        auto result = uncompress(line, part2);
        log << "length: " << result.length() << endl;
    }
}

int main() {
    process("sample.txt", false);
    process("input.txt", false);
    process("sample2.txt", true);
    process("input.txt", true);
    return 0;
}
