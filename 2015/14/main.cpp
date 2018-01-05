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

struct Reindeer {
  std::string name;
  int speed;
  int moveDuration;
  int restDuration;
  int distance{0};
  int points{0};

  void computeDistance(const int duration) {
    int times = duration / (moveDuration + restDuration);
    int left = duration % (moveDuration + restDuration);
    int result = 0;

    result += speed * moveDuration * times;

    int leftMove = std::min(left, moveDuration);

    result += speed * leftMove;

    distance = result;
  }
};

void processFile(const char *filename, const int duration) {
  std::cout << "File " << filename << std::endl;
  std::ifstream f(filename);
  std::string line, str;

  std::vector<Reindeer> reindeers;

  while (std::getline(f, line)) {
    std::istringstream iss(line);
    Reindeer reindeer;
    iss >> reindeer.name >> str >> str >> reindeer.speed >> str >> str >>
        reindeer.moveDuration >> str;
    iss >> str >> str >> str >> str >> str >> reindeer.restDuration;
    reindeers.push_back(reindeer);
  }

  f.close();

  for (int i = 0; i < duration; ++i) {
    for (auto &r : reindeers) {
      r.computeDistance(i + 1);
    }

    std::sort(reindeers.begin(), reindeers.end(),
              [](auto &a, auto &b) { return a.distance > b.distance; });

    int best = reindeers[0].distance;
    for (auto &r : reindeers) {
      if (r.distance < best)
        break;

      ++r.points;
    }
  }

  std::sort(reindeers.begin(), reindeers.end(),
            [](auto &a, auto &b) { return a.points > b.points; });

  auto &winner = reindeers[0];

  std::cout << "Winner is " << winner.name << " with " << winner.points
            << std::endl;
}

int main() {

  processFile("input.txt", 1000);
  processFile("input2.txt", 2503);

  return 0;
}
