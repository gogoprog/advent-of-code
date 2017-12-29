#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <limits>
#include <map>
#include <openssl/md5.h>
#include <sstream>
#include <vector>

std::string md5(const std::string &input) {
  unsigned char digest[MD5_DIGEST_LENGTH];
  MD5((unsigned char *)input.c_str(), input.length(), (unsigned char *)&digest);
  char mdString[33];
  for (int i = 0; i < 16; i++)
    sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);

  return mdString;
}

void process(const std::string &input) {
  std::cout << input << std::endl;

  int i = 0;
  std::string str;

  while (true) {
    str = input + std::to_string(i);
    str = md5(str);

    if (str.substr(0, 6) == "000000") {
      std::cout << "First: " << str << " = " << i << std::endl;
      break;
    }

    ++i;
  }
}

int main() {
  process("abcdef");
  process("ckczppom");

  return 0;
}
