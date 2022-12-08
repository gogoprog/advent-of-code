#include "../../common.h"

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto data = getFileLines(filename);

    Vector<Point> positions;
    auto width = data[0].size();
    auto height = data.size();

    for (int y{0}; y < height; ++y) {
        for (int x{0}; x < width; ++x) {
            positions.push_back({x, y});
        }
    }

    auto is_valid = [&](const auto pos) { return pos.x >= 0 and pos.x < width && pos.y >= 0 and pos.y < height; };

    auto get_value = [&](const auto pos) { return data[pos.y][pos.x] - '0'; };

    {
        auto is_visible = [&](auto pos) {
            auto this_height = get_value(pos);
            auto is_right = [&](auto pos2) { return pos2.x > pos.x && pos2.y == pos.y; };
            auto is_left = [&](auto pos2) { return pos2.x < pos.x && pos2.y == pos.y; };
            auto is_top = [&](auto pos2) { return pos2.x == pos.x && pos2.y < pos.y; };
            auto is_down = [&](auto pos2) { return pos2.x == pos.x && pos2.y > pos.y; };

            auto is_lower = [&](auto pos2) {
                auto height = get_value(pos2);
                return height < this_height;
            };

            auto v_right = positions | rv::filter(is_right);
            auto v_left = positions | rv::filter(is_left);
            auto v_top = positions | rv::filter(is_top);
            auto v_down = positions | rv::filter(is_down);

            return rs::all_of(v_right, is_lower) || rs::all_of(v_left, is_lower) || rs::all_of(v_top, is_lower) ||
                   rs::all_of(v_down, is_lower);
        };

        auto view = positions | rv::filter(is_visible);
        auto count = rs::distance(view);

        log << "Part1: " << count << endl;
    }

    {

        auto compute_scenic = [&](const auto &pos) {
            constexpr Vector2 directions[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
            auto this_value = get_value(pos);

            int result{1};

            for (auto dir : directions) {
                int score{0};
                auto next_pos = pos + dir;

                while (is_valid(next_pos)) {
                    score++;

                    if (get_value(next_pos) >= this_value) {
                        break;
                    }

                    next_pos += dir;
                }

                result *= score;
            }

            return result;
        };

        auto view = positions | rv::transform(compute_scenic) | ra::sort | rv::reverse | rv::take(1);
        auto value = *std::begin(view);

        log << "Part2: " << value << endl;
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
