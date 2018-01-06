#include <algorithm>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

struct Ingredient {
  std::string name;
  int capacity;
  int durability;
  int flavor;
  int texture;
  int calories;

  void log() {
    std::cout << name << std::endl;
    std::cout << capacity << std::endl;
    std::cout << durability << std::endl;
    std::cout << flavor << std::endl;
    std::cout << texture << std::endl;
    std::cout << calories << std::endl;
  }
};

struct Recipe {
  std::vector<int> quantities;

  static void build(const std::vector<Ingredient> &ingredients,
                    const std::vector<int> &quantities, Recipe &best,
                    int &best_score) {
    int current = std::accumulate(quantities.begin(), quantities.end(), 0);

    if (current < 100) {
      if (quantities.size() < ingredients.size()) {
        for (int i = 0; i <= 100 - current; ++i) {
          auto new_quantities = quantities;
          new_quantities.push_back(i);
          build(ingredients, new_quantities, best, best_score);
        }
      }
    } else {
      int score = 0;
      int c{0}, d{0}, f{0}, t{0}, cal{0};

      for (size_t i = 0; i < quantities.size(); ++i) {
        int q = quantities[i];
        auto ing = ingredients[i];
        c += q * ing.capacity;
        d += q * ing.durability;
        f += q * ing.flavor;
        t += q * ing.texture;
        cal += q * ing.calories;
      }

      c = std::max(0, c);
      d = std::max(0, d);
      f = std::max(0, f);
      t = std::max(0, t);

      score = c * d * f * t;

      if (cal == 500 && score > best_score) {
        best.quantities = quantities;
        best_score = score;
      }
    }
  }
};

void processFile(const char *filename) {
  std::cout << "File " << filename << std::endl;
  std::ifstream f(filename);
  std::string line, str;

  std::vector<Ingredient> ingredients;

  while (std::getline(f, line)) {
    std::istringstream iss(line);
    Ingredient ingredient;

    iss >> ingredient.name >> str >> ingredient.capacity >> str;
    iss >> str >> ingredient.durability >> str;
    iss >> str >> ingredient.flavor >> str;
    iss >> str >> ingredient.texture >> str;
    iss >> str >> ingredient.calories;

    ingredients.push_back(ingredient);
  }

  f.close();

  Recipe best;
  int best_score = 0;
  Recipe::build(ingredients, {}, best, best_score);

  std::cout << "Best is " << best_score << std::endl;
}

int main() {

  processFile("input.txt");
  processFile("input2.txt");

  return 0;
}
