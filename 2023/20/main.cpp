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
        const auto final = "js";
        /* const auto final = "output"; */

        if (!modulesMap.contains(final)) {
            return;
        }

        log << modules.size() << endl;

        int64_t result{0};

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

        auto compute = [&](auto from, auto to) {
            int64_t result = 0;
            int64_t i = 0;

            while (true) {
                Queue<Module::Pulse> q;

                q.push({from->id, to->id, LOW});

                while (!q.empty()) {
                    auto pulse = q.front();
                    q.pop();

                    if (pulse.to == rx_id && pulse.value == LOW) {
                        result = i;
                        break;
                    }

                    auto to = *(modules.begin() + pulse.to);
                    auto from = *(modules.begin() + pulse.from);
                    to->process(from, pulse.value, q);
                }

                /* print(); */

                ++i;

                if (i > 1000000) {
                    log << "whoops" << endl;

                    return int64_t(-1);
                }

                if (result)
                    break;
            }

            return result;
        };

        Vector<Module *> visitOrder;
        broadcaster->visit(visitOrder);

        Queue<Module *> q;

        Map<Module *, int64_t> cache;

        for (auto module : visitOrder) {
            q.push(module);
        }

        while (!q.empty()) {

            auto target = q.front();
            q.pop();

            bool has_cache = false;
            int64_t from_cache = 1;

            auto &inputs = target->inputs;

            if (inputs.size() == 1) {

                if (cache.contains(inputs[0])) {
                    switch (target->type) {
                        case FLIPFLOP:
                            has_cache = true;
                            from_cache = (cache[target->inputs[0]] + 1) / 2 - 1;
                            break;
                    }
                }
            } else if (inputs.size() > 1) {
                has_cache = true;

                for (auto input : inputs) {
                    if (cache.contains(input)) {
                        from_cache = std::lcm(from_cache, cache[input]);

                    } else {
                        has_cache = false;
                        break;
                    }
                }
            }

            log << button->name << " - " << target->name << " ..." << endl;
            if (has_cache) {
                log << "from cache : " << from_cache << endl;
                cache[target] = from_cache;
                continue;
            }
            auto r = compute(button, target);
            log << "from compu : " << r << endl;

            if (r == -1) {
                q.push(target);
            } else {
                cache[target] = r;
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
