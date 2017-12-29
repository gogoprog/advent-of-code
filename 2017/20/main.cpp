#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <map>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>

struct Vec3 {
  int x, y, z;
  Vec3 &operator+=(const Vec3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
  }

  bool operator==(const Vec3 &other) const {
    return x == other.x && y == other.y && z == other.z;
  }

  bool operator<(const Vec3 &other) const {
    if (x != other.x)
      return (x < other.x);

    if (y != other.y)
      return (y < other.y);

    return (z < other.z);
  }

  long manhattan() const { return std::abs(x) + std::abs(y) + std::abs(z); }
};

void log(const Vec3 &v) { std::cout << v.x << ", " << v.y << ", " << v.z; }

struct Particle {
  Vec3 p, v, a;
  bool destroyed{false};
  void logMe() {
    std::cout << "p=";
    log(p);
    std::cout << " v=";
    log(v);
    std::cout << " a=";
    log(a);
    std::cout << std::endl;
  }
};

int run(const char *filename) {
  std::cout << "\n\nRun " << filename << std::endl;
  std::ifstream f(filename);
  std::string line, str;
  char c;

  std::vector<Particle> particles;

  while (std::getline(f, line)) {
    std::istringstream iss(line);
    Particle particle;

    iss >> c >> c >> c >> particle.p.x >> c >> particle.p.y >> c >>
        particle.p.z >> str;
    iss >> c >> c >> c >> particle.v.x >> c >> particle.v.y >> c >>
        particle.v.z >> str;
    iss >> c >> c >> c >> particle.a.x >> c >> particle.a.y >> c >>
        particle.a.z >> str;

    /* particle.logMe(); */

    particles.push_back(particle);
  }
  f.close();

  std::map<Vec3, Particle *> space;
  for (int i = 0; i < 5000; ++i) {
    space.clear();
    for (auto &part : particles) {
      if (!part.destroyed) {
        part.v += part.a;
        part.p += part.v;

        if (space.find(part.p) != space.end()) {
          space[part.p]->destroyed = true;
          part.destroyed = true;
        } else {
          space[part.p] = &part;
        }
      }
    }
  }

  /* long closest = std::numeric_limits<long>::max(); */
  /* int closestIndex; */
  /* for (int i = 0; i < particles.size(); ++i) { */
  /*   auto &part = particles[i]; */
  /*   long distance = part.p.manhattan(); */
  /*   if (distance < closest) { */
  /*     closest = distance; */
  /*     closestIndex = i; */
  /*   } */
  /* } */
  /* std::cout << "Closest: " << closestIndex << std::endl; */

  int count = 0;
  for(auto & part : particles){

    if(!part.destroyed) {
      ++count;
    }
  }
  std::cout << "Particles left: " << count << std::endl;
}

int main() {
  run("input.txt");
  run("input3.txt");
  run("input2.txt");
}
