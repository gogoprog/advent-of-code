#include <algorithm>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

using Links = std::map<std::pair<std::string, std::string>, int>;

struct Table {
  std::vector<std::string> seats;
  int happiness{0};

  static void build(const std::vector<std::string> &people,
                    const std::vector<std::string> &seats,
                    std::vector<Table> &result) {

    bool ended{true};

    for (auto &p : people) {
      if (std::find(seats.begin(), seats.end(), p) == seats.end()) {
        ended = false;
        auto seats_cpy = seats;
        seats_cpy.push_back(p);
        build(people, seats_cpy, result);
      }
    }

    if (ended) {
      Table t;
      t.seats = seats;
      result.push_back(t);
    }
  }

  void compute(const Links &links) {
    happiness = 0;

    for (size_t i = 0; i < seats.size(); ++i) {
      int h = i - 1;
      h = h < 0 ? h + seats.size() : h;

      auto &previous = seats[h];

      int j = i + 1;
      j %= seats.size();

      auto &next = seats[j];
      auto &current = seats[i];

      happiness += links.at({current, previous});
      happiness += links.at({current, next});
    }
  }
};

void processFile(const char *filename, const bool add_you = false) {
  std::cout << "File " << filename << std::endl;
  std::ifstream f(filename);
  std::string line, str;
  std::string name1, name2, verb;
  std::vector<std::string> people;
  int value;
  Links links;

  while (std::getline(f, line)) {
    std::istringstream iss(line);
    iss >> name1 >> str >> verb >> value >> str >> str >> str >> str >> str >>
        str >> name2;

    name2.pop_back();

    if (verb == "lose") {
      value = -value;
    }

    links[{name1, name2}] = value;

    people.push_back(name1);
    people.push_back(name2);
  }

  std::sort(people.begin(), people.end());
  auto last = std::unique(people.begin(), people.end());
  people.erase(last, people.end());

  if (add_you) {
    for (auto &p : people) {
      links[{"you", p}] = 0;
      links[{p, "you"}] = 0;
    }
    people.push_back("you");
  }

  std::vector<Table> tables;
  Table::build(people, {}, tables);

  for (auto &t : tables) {
    t.compute(links);
  }

  std::sort(tables.begin(), tables.end(),
            [](auto &a, auto &b) { return a.happiness > b.happiness; });

  std::cout << tables[0].happiness << std::endl;
}

int main() {

  processFile("input.txt");
  processFile("input2.txt");
  processFile("input2.txt", true);

  return 0;
}
