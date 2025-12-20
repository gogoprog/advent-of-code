#include "../../common_fast.h"

struct Context {

    static bool isValid(const StringView str) {
        int len = str.length();

        if (len % 2)
            return true;

        for (int i = 0; i < len / 2; ++i) {
            if (str[i] != str[i + len / 2])
                return true;
        }

        return false;
    }

    static bool isValid2(const StringView str) {
        int len = str.length();
        int half = len / 2;

        for (int l = 1; l <= half; ++l) {

            if ((len % l) != 0)
                continue;

            auto pattern = str.substr(0, l);

            bool good = true;

            for (int r = 1; r < len / l; r++) {
                auto other = str.substr(r * l, l);

                if (other != pattern) {
                    good = false;
                    break;
                }
            }

            if (good) {
                return false;
            }
        }

        return true;
    }

    void parse(auto lines) {
    }

    void part1(auto lines) {
        auto result{0_int64};

        auto id_ranges = lines | rv::get0 | rv::split_sv(',');

        for (auto id_range : id_ranges) {
            auto parts = id_range | rv::split_sv('-');
            auto first = parts | rv::get0 | rv::to_int;
            auto last = parts | rv::get1 | rv::to_int;

            for (int64_t i = first; i <= last; i++) {
                if (!isValid(std::to_string(i))) {
                    result += i;
                }
            }
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0_int64};

        auto id_ranges = lines | rv::get0 | rv::split_sv(',');

        for (auto id_range : id_ranges) {
            auto parts = id_range | rv::split_sv('-');
            auto first = parts | rv::get0 | rv::to_int;
            auto last = parts | rv::get1 | rv::to_int;

            for (int64_t i = first; i <= last; i++) {
                if (!isValid2(std::to_string(i))) {
                    result += i;
                }
            }
        }

        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context context;
        context.parse(lines);
        context.part1(lines);
        context.part2(lines);
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
