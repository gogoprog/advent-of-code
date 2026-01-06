#include "../../common_fast.h"

struct Context {

    struct Shape {
        Vector<StringView> content;
        int size;
    };

    Vector<Shape> shapes;

    struct Region {
        int width;
        int height;
        Vector<int> list;
    };

    Vector<Region> regions;

    void parse(auto content) {
        auto blocks = content | rv::split_sv("\n\n");

        Vector<StringView> vblocks;
        rs::copy(blocks, std::back_inserter(vblocks));

        auto shapes_v = vblocks | rv::reverse | rv::drop(1) | rv::reverse;
        auto regions_v = vblocks | rv::reverse | rv::get0 | rv::split_sv('\n');

        for (auto shape_v : shapes_v) {
            shapes.push_back({});
            auto &shape = shapes.back();
            shape.size = 0;

            for (auto c : shape_v) {
                if (c == '#')
                    shape.size++;
            }

            auto lines = shape_v | rv::split_sv('\n') | rv::drop(1);
            rs::copy(lines, std::back_inserter(shape.content));
        }

        for (auto region_v : regions_v) {
            regions.push_back({});
            auto &region = regions.back();

            auto parts = region_v | rv::split_sv(':');
            auto size = parts | rv::get0 | rv::split_sv('x') | rv::to_ints;

            region.width = size | rv::get0;
            region.height = size | rv::get1;

            auto list_v = parts | rv::get1 | rv::split_sv(' ') | rv::filter_empty | rv::to_ints;

            rs::copy(list_v, std::back_inserter(region.list));
        }
    }

    void part1() {
        auto result{0_int64};

        for (const auto &region : regions) {
            int available = region.width * region.height;

            int required{0};

            for (int i{0}; i < region.list.size(); ++i) {
                required += shapes[i].size * region.list[i];
            }

            if (available >= required) {
                result++;
            }
        }

        log << "Part1: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context context;
        context.parse(content);
        context.part1();
    }
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
