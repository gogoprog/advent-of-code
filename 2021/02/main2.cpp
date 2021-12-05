#include "../../common.h"
#include "raylib.h"
#include "raymath.h"
#include <unistd.h>

const int screenWidth = 1280;
const int screenHeight = 720;

struct Position {
    int depth;
    int horizontal;
};

enum class State { Idling, Moving, Completed };

Vector3 getVisualPosition(const Position position) {
    return {0.5f + position.horizontal, 0.5f, 0.5f + position.depth};
}

void process(const String filename) {
    auto lines = getFileLines(filename);

    InitWindow(screenWidth, screenHeight, "day02");
    Camera3D camera = {0};
    camera.position = {60.0f, 80.0f, 120.0f};
    camera.target = {60.0f, 0.0f, 50.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(144);

    Position position{0, 0};
    auto counter = 0;
    auto line_index = 0;
    auto frame = 0;

    Vector3 from_position, current_position, to_position;
    Vector3 camera_target;
    float advance;
    int from_time;
    int duration;

    current_position = getVisualPosition(position);
    camera_target = current_position;
    State state{State::Idling};

    Vector<Vector3> history;

    sleep(10);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (state != State::Completed) {
            Vector3 target_position = {current_position.x, camera.position.y, current_position.z};
            auto distance = Vector3Distance(camera.position, target_position);
            if (distance > 80) {
                Vector3 target_position = {current_position.x, camera.position.y, current_position.z};

                auto delta = Vector3Subtract(target_position, camera.position);

                camera.position = Vector3Lerp(camera.position, target_position, 0.1f);
            }
        }

        camera.target = Vector3Lerp(camera.target, current_position, 0.1f);

        BeginMode3D(camera);
        DrawCube(current_position, 1, 1, 1, {30, 40, 200, 255});
        DrawGrid(1000, 100.0f);

        if (history.size() > 1) {
            for (int h = 0; h < history.size() - 1; ++h) {
                DrawLine3D(history[h], history[h + 1], {30, 40, 100, 255});
            }
        }

        EndMode3D();
        EndDrawing();

        switch (state) {
            case State::Idling: {
                auto &line = lines[line_index];
                String word;
                int value;
                InputStringStream iss{line};
                iss >> word;
                iss >> value;

                if (word == "forward") {
                    position.horizontal += value;
                } else if (word == "down") {
                    position.depth += value;
                } else if (word == "up") {
                    position.depth -= value;
                }

                line_index++;

                from_position = current_position;
                to_position = getVisualPosition(position);
                state = State::Moving;
                from_time = frame;

                auto delta = abs(to_position.z - from_position.z) + abs(to_position.x - from_position.x);
                duration = 1 * delta;
            } break;

            case State::Moving: {
                auto advance = (frame - from_time) * 1.0f / duration;

                current_position.x = from_position.x + (to_position.x - from_position.x) * advance;
                current_position.z = from_position.z + (to_position.z - from_position.z) * advance;

                current_position = Vector3Lerp(from_position, to_position, advance);

                if (advance >= 1) {
                    current_position = to_position;
                    state = State::Idling;

                    history.push_back(to_position);

                    if (line_index >= lines.size()) {
                        state = State::Completed;
                    }
                }

            } break;

            case State::Completed: {
                if (camera.position.y < 1000.0f) {
                    camera.position.y += 1.0f;
                }

            } break;
        }

        frame++;
    }

    CloseWindow();

    {
        auto r = 0;

        auto depth = 0;
        auto horizontal = 0;

        for (auto &line : lines) {
            String word;
            int value;
            InputStringStream iss{line};
            iss >> word;
            iss >> value;

            if (word == "forward") {
                horizontal += value;
            } else if (word == "down") {
                depth += value;
            } else if (word == "up") {
                depth -= value;
            }
        }

        r = depth * horizontal;
        /* log << "Part1: " << r << endl; */
    }

    /* { */
    /*     auto r = 0; */

    /*     auto depth = 0; */
    /*     auto horizontal = 0; */
    /*     auto aim = 0; */

    /*     for (auto &line : lines) { */
    /*         String word; */
    /*         int value; */
    /*         InputStringStream iss{line}; */
    /*         iss >> word; */
    /*         iss >> value; */

    /*         if (word == "forward") { */
    /*             horizontal += value; */
    /*             depth += aim * value; */
    /*         } else if (word == "down") { */
    /*             aim += value; */
    /*         } else if (word == "up") { */
    /*             aim -= value; */
    /*         } */
    /*     } */

    /*     r = depth * horizontal; */
    /*     log << "Part2: " << r << endl; */
    /* } */
}

int main() {
    process("input.txt");
    return 0;
}
