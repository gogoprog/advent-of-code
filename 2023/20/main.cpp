#include "../../common_fast.h"

enum Type { BROADCASTER, FLIPFLOP, CONJUNCTION };
enum PulseValue { LOW, HIGH };

using int8 = int8_t;

struct Module {
    struct Pulse {
        int8 from;
        int8 to;
        int8 value;
    };

    int8 id;
    int8 type{-1};
    StringView name;
    int8 state{0};
    Vector<Module *> destinations;
    Vector<Module *> inputs;
    Map<Module *, int> receivedPulse;

    void process(Module *source, int8 pulse, Queue<Pulse> &queue) {
        switch (type) {
            case BROADCASTER: {
                for (auto destination : destinations) {
                    queue.push(Pulse{this->id, destination->id, pulse});
                }
            } break;

            case FLIPFLOP: {
                switch (pulse) {
                    case LOW:
                        state ^= 1;

                        int8 new_pulse = (state == 1) ? HIGH : LOW;
                        for (auto destination : destinations) {
                            queue.push(Pulse{this->id, destination->id, new_pulse});
                        }
                        break;
                }

            } break;

            case CONJUNCTION: {

                receivedPulse[source] = pulse;

                auto all_high = true;
                for (auto input : inputs) {
                    if (receivedPulse[input] != HIGH) {
                        all_high = false;
                        break;
                    }
                }

                int8 new_pulse = (all_high) ? LOW : HIGH;
                for (auto destination : destinations) {
                    queue.push(Pulse{this->id, destination->id, new_pulse});
                }
            } break;
        }
    }

    void visit(Vector<Module *> &visitOrder) {

        log << "visiting " << name << endl;

        Vector<Module *> follow_up;

        for (auto destination : destinations) {

            if (std::find(visitOrder.begin(), visitOrder.end(), destination) == visitOrder.end()) {
                visitOrder.push_back(destination);
                follow_up.push_back(destination);
            }
        }

        for (auto destination : follow_up) {
            destination->visit(visitOrder);
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
        module->id = modules.size() - 1;

        return module;
    }

    void reset() {
        for (auto module : modules) {
            module->state = 0;
            module->receivedPulse.clear();
        }
    }

    void parse(auto lines) {

        getModule("button")->name = "button";

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

            q.push({0, broadcaster->id, LOW});

            while (!q.empty()) {
                auto pulse = q.front();
                q.pop();

                counter[pulse.value]++;

                /* if (pulse.from) { */
                /*     log << pulse.from->name << " -" << (pulse.value ? "high" : "low") << "-> " << pulse.to->name */
                /*         << endl; */
                /* } */

                modules[pulse.to]->process(modules[pulse.from], pulse.value, q);
            }
        }

        result = counter[LOW] * counter[HIGH];

        log << "Part1: " << result << endl;
    }

    void part2() {
        /* const auto final = "js"; */
        const auto final = "rx";

        if (!modulesMap.contains(final)) {
            return;
        }

        log << modules.size() << endl;

        reset();

        auto broadcaster = getModule("broadcaster");
        auto button = getModule("button");
        auto rx = getModule(final);
        auto rx_id = getModule(final)->id;

        for (auto module : modules) {
            auto input_size = module->inputs.size();
            auto dest_size = module->destinations.size();

            if (input_size == 1 && dest_size == 1) {
                log << module->inputs[0]->name << " - " << module->name << " - " << module->destinations[0]->name
                    << endl;
            }
        }

        using Pair64 = Pair<int64_t, int64_t>;

        auto compute = [&](auto from, auto to, int8 target_pulse, int pulse_from) {
            int result = -1;
            int64_t offset = 0;
            int64_t interval = 0;
            int i = 1;

            int step = 0;

            int last = 0;
            int lastinterval = 0;

            reset();

            while (true) {
                Queue<Module::Pulse> q;

                q.push({from->id, broadcaster->id, LOW});

                while (!q.empty()) {
                    auto pulse = q.front();
                    q.pop();

                    if (pulse.to == to->id && pulse.value == target_pulse) {

                        if (pulse.from == pulse_from) {
                            /* log << i << endl; */
                            return i;
                        }
                    }

                    auto to = *(modules.begin() + pulse.to);
                    auto from = *(modules.begin() + pulse.from);
                    to->process(from, pulse.value, q);
                }

                /* print(); */

                ++i;

                if (i > 100000) {
                    log << "whoops" << endl;

                    return -1;
                }

                /* if (result) */
                /* break; */
            }

            return result;
        };

        {
            /* auto r = compute(button, rx, LOW); */
            /* log << rx->name << " = " << r << endl; */
        }

        int64_t result = 1;

        for (auto input : rx->inputs[0]->inputs) {

            int64_t r = compute(button, rx->inputs[0], HIGH, input->id);

            log << input->id << " = " << r << endl;

            result = std::lcm(result, r);

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
