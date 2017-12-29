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
  int muls{0};
  void log() {
    std::cout << cursor << ": ";
    for (auto kv : registers) {
      std::cout << kv.first << "=" << kv.second << " | ";
    }
    std::cout << std::endl;
  }
};

void part2() {
  std::cout << "\nPart2... Not optimized" << std::endl;
  long a{0}, b{0}, c{0}, d{0}, e{0}, f{0}, g{0}, h{0};
  a = 1;

  auto log = [&]() {
    std::cout << "a=" << a << " b=" << b << " c=" << c << " d=" << d
              << " e=" << e << " f=" << f << " g=" << g << " h=" << h
              << std::endl;
  };

l1:
  b = 84; /* set b 84 */
l2:
  c = b; /* set c b */
l3:
  if (a != 0)
    goto l5; /* jnz a 2 */
l4:
  goto l9; /* jnz 1 5 */
l5:
  b *= 100; /* mul b 100 */
l6:
  b += 100000; /* sub b -100000 */
l7:
  c = b; /* set c b */
l8:
  c += 17000; /* sub c -17000 */
l9:
  f = 1; /* set f 1 */
l10:
  d = 2; /* set d 2 */
l11:
  e = 2; /* set e 2 */
l12:
  g = d; /* set g d */
l13:
  g *= e; /* mul g e */
l14:
  g -= b; /* sub g b */
l15:
  if (g != 0)
    goto l17; /* jnz g 2 */
l16:
  f = 0; /* set f 0 */
l17:
  e++; /* sub e -1 */
l18:
  g = e; /* set g e */
l19:
  g -= b; /* sub g b */
l20:
  if (g != 0)
    goto l12; /* jnz g -8 */
l21:
  d++; /* sub d -1 */
l22:
  g = d; /* set g d */
l23:
  g -= b; /* sub g b */
l24:
  if (g != 0)
    goto l11; /* jnz g -13 */
l25:
  if (f != 0)
    goto l27; /* jnz f 2 */
l26:
  h++; /* sub h -1 */
  log();
l27:
  g = b; /* set g b */
l28:
  g -= c; /* sub g c */
l29:
  if (g != 0)
    goto l31; /* jnz g 2 */
l30:
  goto end; /* jnz 1 3 */
l31:
  b += 17; /* sub b -17 */
l32:
  goto l9; /* jnz 1 -23 */

end:

  log();
}

int part1(const char *filename) {
  std::cout << "\n\nRun " << filename << std::endl;
  std::ifstream f(filename);
  std::vector<std::string> lines;
  std::string line, str;
  while (std::getline(f, line)) {
    lines.push_back(line);
  }
  f.close();

  auto get_value = [&](Context &ctx, std::istringstream &iss) -> long {
    iss >> str;
    if (str.length() == 1 && str[0] >= 'a' && str[0] <= 'z') {
      char c = str[0];
      return ctx.registers[c];
    }
    return static_cast<long>(std::stoi(str));
  };

  auto get_register = [&](Context & ctx, std::istringstream & iss) -> auto & {
    char c;
    iss >> c;
    return ctx.registers[c];
  };

  std::map<std::string,
           std::function<void(Context & ctx, std::istringstream &)>>
      funcs = {
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
          {"sub",
           [&](Context &ctx, std::istringstream &iss) {
             auto &reg = get_register(ctx, iss);
             auto y = get_value(ctx, iss);
             reg -= y;
           }},
          {"mul",
           [&](Context &ctx, std::istringstream &iss) {
             auto &reg = get_register(ctx, iss);
             auto y = get_value(ctx, iss);
             reg *= y;
             ctx.muls++;
           }},
          {"jnz",
           [&](Context &ctx, std::istringstream &iss) {
             auto x = get_value(ctx, iss);
             auto y = get_value(ctx, iss);
             if (x != 0) {
               --ctx.cursor;
               ctx.cursor += y;
             }
           }},
      };

  int prg_size = lines.size();
  {
    Context ctx;

    while (ctx.cursor < prg_size) {
      std::istringstream iss(lines[ctx.cursor]);
      iss >> str;
      ++ctx.cursor;
      funcs[str](ctx, iss);
    }

    std::cout << "Program muls " << ctx.muls << std::endl;
  }
}

int main() {
  part1("input.txt");
  part2();
}
