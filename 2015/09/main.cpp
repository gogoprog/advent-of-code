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

void travel(const std::vector<std::string> &route,
            const std::vector<std::string> &cities,
            const std::map<std::pair<std::string, std::string>, int> &distances,
            const int current, std::vector<int> &results) {
  bool ended{true};
  for (auto &city : cities) {
    if (std::find(route.begin(), route.end(), city) == route.end()) {
      auto new_route = route;
      new_route.push_back(city);
      int new_distance = current;
      if (route.size()) {
        new_distance += distances.at({route.back(), city});
      }
      travel(new_route, cities, distances, new_distance, results);
      ended = false;
    }
  }

  if (ended) {
    results.push_back(current);
  }
}

void processFile(const char *filename) {
  std::cout << "File " << filename << std::endl;
  std::ifstream f(filename);
  std::string line;

  std::map<std::pair<std::string, std::string>, int> distances;
  std::vector<std::string> cities;

  while (std::getline(f, line)) {
    std::string a, b;
    std::string str;
    int value;

    std::istringstream iss(line);

    iss >> a >> str >> b >> str >> value;

    distances[{a, b}] = value;
    distances[{b, a}] = value;
    cities.push_back(a);
    cities.push_back(b);
  }

  std::vector<int> results;
  std::sort(cities.begin(), cities.end());
  cities.erase(std::unique(cities.begin(), cities.end()), cities.end());

  travel({}, cities, distances, 0, results);

  std::sort(results.begin(), results.end());
  std::cout << "Shortest: " << results[0] << std::endl;
  std::cout << "Longest: " << results.back() << std::endl;
}

int main() {

  processFile("input.txt");
  processFile("input2.txt");

  return 0;
}
