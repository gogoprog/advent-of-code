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

void increment(std::string &str) {

  for (int i = 7; i >= 0; --i) {
    char c = str[i];

    if (c < 'z') {
      str[i] = c + 1;
      return;
    } else {
      str[i] = 'a';
    }
  }
}


bool isValid(const std::string & str) {

  bool has_brelan{false};
  int pairs = 0;

  std::string pairs_chars;


  for(int i=0; i< 8; ++i){
    char c = str[i];

    if(i>=2) {
      if(str[i-1] == c - 1) {
        if(str[i-2] == c - 2){
          has_brelan = true;
        }
      }

    }

    if(i>0) {
      if(str[i -1] == c) {
        if(pairs_chars.find(c) == std::string::npos) {
          pairs_chars.push_back(c);
          ++pairs;
        }
      }
    }

    if(c == 'i' || c == 'o' || c == 'l') {
      return false;
    }

  }


  return pairs >=2 && has_brelan;
}

void process(const char *input) {
  std::cout << "Input " << input << std::endl;
  std::string current = input;

  do {
    increment(current);
  } while(!isValid(current));

  std::cout << "Result " << current << std::endl;
}

int main() {
  process("abcdefgh");
  process("vzbxkghb");
  process("vzbxxyzz");

  return 0;
}
