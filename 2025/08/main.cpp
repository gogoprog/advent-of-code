#include "../../common_fast.h"

struct Context {

    using Box = Point3<Int64>;
    using Circuit = Set<Box>;
    Vector<Box> boxes;

    void parse(auto lines) {
        for (auto line : lines) {

            boxes.push_back({});
            auto &box = boxes.back();

            box.x = line | rv::split_sv(',') | rv::get0 | rv::to_int;
            box.y = line | rv::split_sv(',') | rv::get1 | rv::to_int;
            box.z = line | rv::split_sv(',') | rv::get2 | rv::to_int;
        }
    }

    template <int size> void part1() {
        auto result{0_int64};

        auto boxes = this->boxes;

        Vector<Circuit> circuits;

        auto distance = [](Box a, Box b) { return (b - a).squareLength(); };

        Uint64 previous = 0;

        for (int i = 0; i < size; ++i) {
            Uint64 min = 1_int64 << 63;
            Box *besta = nullptr;
            Box *bestb = nullptr;

            for (auto &a : boxes) {
                for (auto &b : boxes) {
                    if (&a != &b) {
                        auto d = distance(a, b);

                        if (d < min && d > previous) {
                            min = d;
                            besta = &a;
                            bestb = &b;
                        }
                    }
                }
            }

            if (besta && bestb) {
                previous = min;
                bool connected{false};

                for (auto &circuit : circuits) {
                    if (circuit.contains(*besta) || circuit.contains(*bestb)) {
                        circuit.insert(*besta);
                        circuit.insert(*bestb);
                        connected = true;
                        break;
                    }
                }

                if (!connected) {
                    circuits.push_back({*besta, *bestb});
                }
            } else {
                break;
            }
        }

        bool work{true};

        while (work) {
            work = false;

            for (auto &circuit_a : circuits) {
                for (auto &circuit_b : circuits) {

                    if (&circuit_a != &circuit_b) {

                        for (auto &b : circuit_b) {
                            if (circuit_a.contains(b)) {
                                circuit_a.insert(circuit_b.begin(), circuit_b.end());
                                circuits.erase(std::find(circuits.begin(), circuits.end(), circuit_b));
                                work = true;
                                break;
                            }
                        }
                    }
                }
                if (work) {
                    break;
                }
            }
        }

        std::sort(circuits.begin(), circuits.end(), [](auto a, auto b) { return b.size() < a.size(); });

        result = 1;
        for (int i = 0; i < 3; ++i) {
            result *= circuits[i].size();
        }

        log << "Part1: " << result << endl;
    }

    void part2(auto lines) {
        auto result{0_int64};

        auto boxes = this->boxes;

        Vector<Circuit> circuits;

        auto distance = [](Box a, Box b) { return (b - a).squareLength(); };

        Uint64 previous = 0;

        while (true) {
            Uint64 min = 1_int64 << 63;
            Box *besta = nullptr;
            Box *bestb = nullptr;

            for (auto &a : boxes) {
                for (auto &b : boxes) {
                    if (&a != &b) {
                        auto d = distance(a, b);

                        if (d < min && d > previous) {
                            min = d;
                            besta = &a;
                            bestb = &b;
                        }
                    }
                }
            }

            if (besta && bestb) {
                previous = min;
                bool connected{false};

                for (auto &circuit : circuits) {
                    if (circuit.contains(*besta) || circuit.contains(*bestb)) {
                        circuit.insert(*besta);
                        circuit.insert(*bestb);
                        connected = true;
                        break;
                    }
                }

                if (!connected) {
                    circuits.push_back({*besta, *bestb});
                }

                bool work{true};

                while (work) {
                    work = false;

                    for (auto &circuit_a : circuits) {
                        for (auto &circuit_b : circuits) {

                            if (&circuit_a != &circuit_b) {

                                for (auto &b : circuit_b) {
                                    if (circuit_a.contains(b)) {
                                        circuit_a.insert(circuit_b.begin(), circuit_b.end());
                                        circuits.erase(std::find(circuits.begin(), circuits.end(), circuit_b));
                                        work = true;
                                        break;
                                    }
                                }
                            }
                        }
                        if (work) {
                            break;
                        }
                    }
                }

                if (circuits[0].size() == boxes.size()) {
                    result = (*besta).x * (*bestb).x;
                    break;
                }
            } else {
                break;
            }
        }

        log << "Part2: " << result << endl;
    }
};

template <int size> void process(const char *filename) {
    log << "Processing " << filename << endl;
    auto content = getFileContent(filename);
    auto lines = rs::split_string_view(content, '\n');
    {
        Context context;
        context.parse(lines);
        context.part1<size>();
        context.part2(lines);
    }
}

int main() {
    process<10>("sample.txt");
    process<1000>("input.txt");
    return 0;
}
