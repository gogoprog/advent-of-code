#include "../../common.h"

struct Room {
    void parse(const String str) {
        auto parts = splitString(str, '[');
        auto dashparts = splitString(parts[0], '-');
        String checksum = parts[1];
        checksum.erase(checksum.size() - 1);
        sectorId = std::stoi(dashparts.back());

        Map<char, int> charCountMap;
        Vector<char> allChars;

        first = parts[0];

        for (char c{'a'}; c <= 'z'; ++c) {
            allChars.push_back(c);
            charCountMap[c] = std::count(first.begin(), first.end(), c);
        }

        std::stable_sort(allChars.begin(), allChars.end(),
                         [&](auto a, auto b) { return charCountMap[a] > charCountMap[b]; });

        allChars.erase(allChars.begin() + 5, allChars.end());

        String computedChecksum{allChars.begin(), allChars.end()};

        valid = (checksum == computedChecksum);
    }

    bool isReal() const { return valid; }

    void decrypt() {
        auto dashparts = splitString(first, '-');
        dashparts.erase(dashparts.end() - 1);

        String name, output;

        for (auto part : dashparts) {
            name += part + ' ';
        }

        for (auto c : name) {
            if (c != ' ') {
                unsigned char n = c + (sectorId % 26);
                while (n > 'z')
                    n -= 26;
                output += n;
            } else {
                output += c;
            }
        }

        if(output.find("north") != String::npos) {
            log << "part2: " << sectorId << endl;
        }
    }

    int sectorId{0};
    bool valid{false};
    String first;
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    int sum{0};
    for (auto line : lines) {
        Room room;
        room.parse(line);

        if (room.isReal()) {
            sum += room.sectorId;

            room.decrypt();
        }
    }

    log << "part1: " << sum << endl;
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
