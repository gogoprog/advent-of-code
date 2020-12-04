#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Map<String, std::function<bool(String)>> required = {
        {"byr",
         [](auto value) {
             int v = std::stoi(value);
             return v >= 1920 && v <= 2002;
         }},
        {"iyr",
         [](auto value) {
             int v = std::stoi(value);
             return v >= 2010 && v <= 2020;
         }},
        {"eyr",
         [](auto value) {
             int v = std::stoi(value);
             return v >= 2020 && v <= 2030;
         }},
        {"hgt",
         [](auto value) {
             InputStringStream iss{value};
             int v;
             String units;
             iss >> v >> units;

             if (units == "cm") {
                 return v >= 150 && v <= 193;
             } else {
                 return v >= 59 && v <= 76;
             }
         }},
        {"hcl",
         [](auto value) {
             InputStringStream iss{value};
             char c;
             String v;
             iss >> c;

             if (c == '#') {

                 iss >> v;
                 if (v.length() == 6) {
                     for (int i{0}; i < 6; ++i) {
                         if (!std::isxdigit(v[i])) {
                             return false;
                         }
                     }

                     return true;
                 }
             }

             return false;
         }},

        {"ecl",
         [](auto value) {
             return (value == "amb" || value == "blu" || value == "brn" || value == "gry" || value == "grn" ||
                     value == "hzl" || value == "oth");
         }},
        {"pid",
         [](auto value) {
             if (value.length() == 9) {

                 for (int i{0}; i < 9; ++i) {
                     if (!(value[i] >= '0' && value[i] <= '9')) {
                         return false;
                     }
                 }

                 return true;
             }

             return false;
         }},

    };

    int count1{0};
    int count2{0};
    int valid1{0};
    int valid2{0};

    auto validate = [&]() {
        if (count1 == required.size()) {
            valid1++;
        }
        if (count2 == required.size()) {
            valid2++;
        }
        count1 = 0;
        count2 = 0;
    };

    for (auto &line : lines) {

        InputStringStream iss{line};

        while (!iss.eof()) {
            String input;
            iss >> input;
            auto parts = splitString(input, ':');

            if (parts.size() == 2) {
                if (required.find(parts[0]) != required.end()) {
                    if (required[parts[0]](parts[1])) {
                        count2++;
                    }
                    count1++;
                }
            }
        }

        if (line == "") {
            validate();
        }
    }

    validate();

    log << "part1 : " << valid1 << endl;
    log << "part2 : " << valid2 << endl;
}

int main() {
    /* process("sample.txt"); */
    /* process("valids.txt"); */
    /* process("invalids.txt"); */
    process("input.txt");
    return 0;
}
