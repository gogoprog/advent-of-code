#include "../../common.h"

using Visited = Map<Coord, bool>;

#ifdef RAYLIB
namespace renderer {
const int screenWidth = 1280;
const int screenHeight = 720;
Camera3D camera = {0};

void initialize() {
    InitWindow(screenWidth, screenHeight, "day02");
    camera.position = {60.0f, 150.0f, 120.0f};
    camera.target = {60.0f, 0.0f, 50.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(144);
}

template <int size> struct Context {
    Array<Coord, size> sources;
    Array<Coord, size> destinations;
    Array<Vector2, size> items;

    void init() {

        for (int i = 0; i < size; ++i) {
            destinations[i].x = 0;
            destinations[i].y = 0;
        }

        Visited v{};
        draw(v);

        sleep(3);
    }

    void step(Visited &visited, const Array<Coord, size> &targets) {
        static int skip = 0;

        if (skip++ % 10)
            return;

        sources = destinations;
        destinations = targets;

        for (int i = 0; i < size; ++i) {
            items[i].x = destinations[i].x;
            items[i].y = destinations[i].y;
        }

        draw(visited);

        /* sleep(1); */
    }

    void draw(Visited &visited) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        camera.target = Vector3Lerp(camera.target, {items[0].x, 0, items[0].y}, 0.01f);
        BeginMode3D(camera);

        DrawGrid(1000, 10.0f);

        for (auto &kv : visited) {
            if (kv.second) {
                DrawSphere({kv.first.x, 0, kv.first.y}, 0.3, {100, 5, 5, 255});
            }
        }

        for (auto &item : items) {
            DrawSphere({item.x, 0, item.y}, 1, {20, 20, 200, 255});
        }

        EndMode3D();
        EndDrawing();
    }
};

} // namespace renderer

#endif

Map<char, Coord> deltas{{'R', Coord{1, 0}}, {'L', {-1, 0}}, {'U', {0, -1}}, {'D', {0, 1}}};

template <int size> struct Context {
    Map<Coord, bool> visited;
    Array<Coord, size> knots;

#ifdef RAYLIB
    renderer::Context<size> renderer;
#endif

    void init() {
        visited[{0, 0}] = true;

        for (auto &knot : knots) {
            knot.set(0, 0);
        }

#ifdef RAYLIB
        renderer.init();
#endif
    }

    void follow(const int index, const Coord parent_move) {
        auto parent = knots[index - 1];
        auto &current = knots[index];

        auto diff = parent - current;

        Coord move{0, 0};

        /* log << index << " " << diff << endl; */

        if (std::abs(diff.x) > 1 || std::abs(diff.y) > 1) {
            if (parent.x == current.x || parent.y == current.y) {
                move = diff / diff.manhattan();
            } else {
                if (diff.manhattan() == 4) {
                    move = diff / 2;
                } else if (std::abs(diff.y) > 1) {
                    move.x = diff.x;
                    move.y = parent_move.y;
                } else if (std::abs(diff.x) > 1) {
                    move.x = parent_move.x;
                    move.y = diff.y;
                }
            }
        }

        if (move.manhattan() != 0) {
            current += move;

            /* draw(); */

            if (index < size - 1) {
                follow(index + 1, move);
            }
        }
    }

    void move(const char direction, const int length) {
        auto delta = deltas[direction];
        /* log << "== " << direction << " " << length << " == " << endl; */

        for (auto i : rs::iota_view(0, length)) {
            knots[0] += delta;

            follow(1, delta);

            visited[knots.back()] = true;

            /* draw(); */

#if RAYLIB
            renderer.step(visited, knots);
#endif
        }
    }

    void draw() {

        log << endl;
        auto [minPoint, maxPoint] = getMinMax(visited);

        for (int y = minPoint.y - size; y <= maxPoint.y + size; y++) {
            for (int x = minPoint.x - size; x <= maxPoint.x + size; ++x) {
                auto pos = Coord{x, y};
                bool found = false;

                for (auto i : rs::iota_view(0, size)) {
                    if (pos == knots[i]) {
                        log << i;
                        found = true;
                        break;
                    }
                }

                if (!found) {

                    if (pos == Coord{0, 0}) {
                        log << 's';
                    } else {
                        log << '.';
                    }
                }
            }

            log << endl;
        }
    }

    int countVisited() const {
        return std::count_if(visited.begin(), visited.end(), [](auto kv) { return kv.second; });
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    {
        Context<2> ctx;

        ctx.init();

        for (auto line : lines) {
            char d;
            int length;
            InputStringStream iss{line};
            iss >> d >> length;
            ctx.move(d, length);
        }

        log << "Part1: " << ctx.countVisited() << endl;
    }
    {
        Context<10> ctx;

        ctx.init();

        for (auto line : lines) {
            char d;
            int length;
            InputStringStream iss{line};
            iss >> d >> length;
            ctx.move(d, length);
        }

        log << "Part2: " << ctx.countVisited() << endl;
    }
}

int main() {
#ifdef RAYLIB
    renderer::initialize();
#endif
    process("sample.txt");
    process("sample2.txt");
    process("input.txt");
    return 0;
}
