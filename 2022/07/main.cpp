#include "../../common.h"

enum class Type {
    Directory,
    File,
};

constexpr int PART1MAX = 100000;
constexpr int DISK_SIZE = 70000000;
constexpr int TARGET_FREE_SPACE = 30000000;

struct Entry {
    String name;
    String fullPath;
    Type type;
    int size;
    Vector<Entry *> children;
    Entry *parent{nullptr};

    int computeSize() const {
        auto result = size;

        for (auto child : children) {
            result += child->computeSize();
        }

        return result;
    }

    void addChild(Entry *entry) {
        if (std::find(children.begin(), children.end(), entry) == children.end()) {
            children.push_back(entry);
            entry->parent = this;
        }
    }

    void print(const int depth = 0) {
        for (int i = 0; i < depth; ++i) {
            log << "    ";
        }
        log << name << " (" << (int)type << ") " << size << " - " << endl;

        for (auto child : children) {
            child->print(depth + 1);
        }
    }
};

struct Context {
    Map<String, UniquePtr<Entry>> entries;

    void parse(const Vector<String> &lines) {
        Entry *current_dir{nullptr};

        auto change_dir = [&](const String full_path) {
            if (entries.find(full_path) == entries.end()) {
                auto pos = full_path.find_last_of('/');
                auto name = full_path.substr(pos);
                auto entry = std::make_unique<Entry>(name, full_path, Type::Directory, 0);
                entries[full_path] = std::move(entry);
            }

            current_dir = entries[full_path].get();
        };

        auto goto_dir = [&](const auto dir) {
            if (dir == "/") {
                change_dir("/");
            } else if (dir == "..") {
                current_dir = current_dir->parent;
            } else {
                auto previous_dir = current_dir;
                change_dir(current_dir->fullPath + "/" + dir);
                previous_dir->addChild(current_dir);
            }
        };

        for (auto line : lines) {
            InputStringStream iss{line};
            String str;

            if (line[0] == '$') {
                iss >> str >> str;

                if (str == "cd") {
                    iss >> str;
                    goto_dir(str);
                } else if (str == "ls") {
                    // noop
                }

            } else {
                String name;
                iss >> str >> name;

                auto full_path = current_dir->fullPath + "/" + name;

                if (entries.find(full_path) == entries.end()) {
                    if (str == "dir") {
                        auto entry = std::make_unique<Entry>(name, full_path, Type::Directory, 0);
                        current_dir->addChild(entry.get());
                        entries[full_path] = std::move(entry);

                    } else {
                        auto size = convert<String, int>(str);

                        auto entry = std::make_unique<Entry>(name, full_path, Type::File, size);
                        current_dir->addChild(entry.get());
                        entries[full_path] = std::move(entry);
                    }
                }
            }
        }
    }

    void print() {
        entries["/"]->print();
    }

    void part1() {
        auto compute_size = [](auto &entry) { return entry->computeSize(); };

        auto below_max = [](auto value) { return value <= PART1MAX; };

        auto r =
            entries | rv::values | rv::transform(compute_size) | rv::filter(below_max) | ra::reduce(0, std::plus());

        log << "Part1: " << r << endl;
    }

    void part2() {
        auto root = entries["/"].get();
        auto used_size = root->computeSize();
        auto current_free_space = DISK_SIZE - used_size;

        auto compute_size = [](auto &entry) { return entry->computeSize(); };
        auto condition = [=](auto size) { return current_free_space + size >= TARGET_FREE_SPACE; };
        auto r = entries | rv::values | rv::transform(compute_size) | rv::filter(condition) | ra::sort | rv::take(1);

        log << "Part2: " << *std::begin(r) << endl;
    }
};

void process(const String filename) {
    log << "Processing " << filename << endl;
    auto lines = getFileLines(filename);

    Context ctx;
    ctx.parse(lines);

    ctx.part1();
    ctx.part2();
}

int main() {
    process("sample.txt");
    process("input.txt");
    return 0;
}
