#include "../../common_fast.h"

enum Type { BROADCASTER, FLIPFLOP, CONJUNCTION };
enum PulseValue { LOW, HIGH };

struct Module {
    struct Pulse {
        Module *from;
        Module *to;
        int value;
    };

    int type{-1};
    StringView name;
    int state{0};
    Vector<Module *> destinations;
    Vector<Module *> inputs;
    Map<Module *, int> receivedPulse;

    void process(Module *source, int pulse, Queue<Pulse> &queue) {
        switch (type) {
            case BROADCASTER: {
                for (auto destination : destinations) {
                    queue.push(Pulse{this, destination, pulse});
                }
            } break;

            case FLIPFLOP: {
                switch (pulse) {
                    case LOW:
                        state ^= 1;

                        auto new_pulse = (state == 1) ? HIGH : LOW;
                        for (auto destination : destinations) {
                            queue.push(Pulse{this, destination, new_pulse});
                        }
                        break;
                }

            } break;

            case CONJUNCTION: {

                receivedPulse[source] = pulse;

                auto was_high = true;
                for (auto input : inputs) {
                    if (receivedPulse[input] != HIGH) {
                        was_high = false;
                        break;
                    }
                }

                auto new_pulse = (was_high) ? LOW : HIGH;
                for (auto destination : destinations) {
                    queue.push(Pulse{this, destination, new_pulse});
                }
            } break;
        }
    }
};

struct Context {

    Vector<Module *> modules;
    Map<StringView, Module *> modulesMap;

    Module *getModule(StringView name) {
        if (modulesMap.contains(name)) {
            return modulesMap[name];
        }

        auto module = new Module();

        module->name = name;

        modulesMap[name] = module;
        modules.push_back(module);

        return module;
    }

    void reset() {
        for (auto module : modules) {
            module->state = 0;
            module->receivedPulse.clear();
        }
    }

    void parse(auto lines) {

        for (auto line : lines) {

            auto parts = line | rv::split_sv(" -> ");
            auto left = parts | rv::get0;
            auto right = parts | rv::get1;

            auto first_char = left[0];

            Module *source;

            switch (first_char) {
                case '%':
                    source = getModule(left.substr(1));
                    source->type = FLIPFLOP;
                    break;
                case '&':
                    source = getModule(left.substr(1));
                    source->type = CONJUNCTION;
                    break;
                default:
                    source = getModule(left);
                    source->type = BROADCASTER;
                    break;
            }

            auto destinations = right | rv::split_sv(", ");

            for (auto destination : destinations) {

                auto module = getModule(destination);
                source->destinations.push_back(module);
                module->inputs.push_back(source);
            }
        }
    }

    void part1() {
        auto result{0};

        auto broadcaster = getModule("broadcaster");

        Queue<Module::Pulse> q;
        Array<int, 2> counter{0, 0};

        for (int i = 0; i < 1000; i++) {

            q.push({nullptr, broadcaster, LOW});

            while (!q.empty()) {
                auto pulse = q.front();
                q.pop();

                counter[pulse.value]++;

                /* if (pulse.from) { */
                /*     log << pulse.from->name << " -" << (pulse.value ? "high" : "low") << "-> " << pulse.to->name */
                /*         << endl; */
                /* } */

                pulse.to->process(pulse.from, pulse.value, q);
            }
        }

        result = counter[LOW] * counter[HIGH];

        log << "Part1: " << result << endl;
    }

    void part2() {

        if (!modulesMap.contains("rx")) {
            return;
        }

        auto result{0};

        reset();

        auto broadcaster = getModule("broadcaster");
        auto rx = getModule("rx");

        Queue<Module::Pulse> q;

        for (int i = 0; i < 1000000000; i++) {

            q.push({nullptr, broadcaster, LOW});

            while (!q.empty()) {
                auto pulse = q.front();
                q.pop();

                if(pulse.to == rx) {

                    result = i;
                    break;
                }

                /* if (pulse.from) { */
                /*     log << pulse.from->name << " -" << (pulse.value ? "high" : "low") << "-> " << pulse.to->name */
                /*         << endl; */
                /* } */

                pulse.to->process(pulse.from, pulse.value, q);
            }
        }



        log << "Part2: " << result << endl;
    }
};

void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto lines = rs::split_string_view(getFileContent(filename), '\n');
    {
        Context context;
        context.parse(lines);
        context.part1();
        context.part2();
    }
}

int main() {
    process("sample.txt");
    process("sample2.txt");
    process("input.txt");
    return 0;
}
