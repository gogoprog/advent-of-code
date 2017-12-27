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
#include <vector>

struct Context {
  std::map<char, long> registers;
  int cursor{0};
  long lastFreq{0};
  std::queue<long> messages;
  int pid;
  bool waiting{false};
  int sent{0};
};

int run(const char *filename) {
  std::cout << "\n\nRun " << filename << std::endl;
  std::ifstream f(filename);
  std::vector<std::string> lines;
  std::string line, str;
  while (std::getline(f, line)) {
    lines.push_back(line);
  }
  f.close();

  std::vector<Context> contexts(2);

  auto get_value = [&](Context &ctx, std::istringstream &iss) -> long {
    iss >> str;
    if (str.length() == 1 && str[0] >= 'a' && str[0] <= 'z') {
      char c = str[0];
      if (ctx.registers.find(c) == ctx.registers.end()) {
        ctx.registers[c] = 0;
      }
      return ctx.registers[c];
    }
    return static_cast<long>(std::stoi(str));
  };

  auto get_register = [&](Context & ctx, std::istringstream & iss) -> auto & {
    char c;
    iss >> c;
    if (ctx.registers.find(c) == ctx.registers.end()) {
      ctx.registers[c] = 0;
    }
    return ctx.registers[c];
  };

  std::map<std::string,
           std::function<void(Context & ctx, std::istringstream &)>>
      funcs = {
          {"snd",
           [&](Context &ctx, std::istringstream &iss) {
             auto freq = get_value(ctx, iss);
             ctx.lastFreq = freq;
             contexts[1 - ctx.pid].messages.push(freq);
             ++ctx.sent;
           }},

          {"set",
           [&](Context &ctx, std::istringstream &iss) {
             auto &reg = get_register(ctx, iss);
             auto y = get_value(ctx, iss);
             reg = y;
           }},
          {"add",
           [&](Context &ctx, std::istringstream &iss) {
             auto &reg = get_register(ctx, iss);
             auto y = get_value(ctx, iss);
             reg += y;
           }},
          {"mul",
           [&](Context &ctx, std::istringstream &iss) {
             auto &reg = get_register(ctx, iss);
             auto y = get_value(ctx, iss);
             reg *= y;
           }},
          {"mod",
           [&](Context &ctx, std::istringstream &iss) {
             auto &reg = get_register(ctx, iss);
             auto y = get_value(ctx, iss);
             reg %= y;
           }},
          {"rcv",
           [&](Context &ctx, std::istringstream &iss) {
             auto &reg = get_register(ctx, iss);
             if (ctx.messages.size()) {
               ctx.waiting = false;
               reg = ctx.messages.front();
               ctx.messages.pop();
             }
             else {
               --ctx.cursor;
               ctx.waiting = true;

               if(contexts[1 - ctx.pid].waiting) {
                 std::cout << "Deadlock" << std::endl;
                 contexts[0].cursor = lines.size();
                 contexts[1].cursor = lines.size();
               }
             }
           }},
          {"jgz",
           [&](Context &ctx, std::istringstream &iss) {
             auto x = get_value(ctx, iss);
             auto y = get_value(ctx, iss);
             if (x > 0) {
               --ctx.cursor;
               ctx.cursor += y;
             }
           }},
      };

  contexts[0].registers['p'] = 0;
  contexts[0].pid = 0;
  contexts[1].registers['p'] = 1;
  contexts[1].pid = 1;

  int prg_size = lines.size();

  while (contexts[0].cursor < prg_size || contexts[1].cursor < prg_size) {
    for (int i = 0; i < 2; ++i) {
      auto &ctx = contexts[i];
      if (ctx.cursor < prg_size) {

        std::istringstream iss(lines[ctx.cursor]);
        iss >> str;
        ++ctx.cursor;
        funcs[str](ctx, iss);
      }
    }
  }

  std::cout << "Program 1 sent " << contexts[1].sent << " packets" << std::endl;
}

int main() {
  run("input.txt");
  run("input2.txt");
}
