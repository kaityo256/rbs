#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>
#include <iomanip>
#include <randbit.hpp>

std::string b2s(uint32_t bs) {
  std::stringstream ss;
  for (int i = 0; i < 32; i++) {
    ss << ((bs & (1 << (32 - i - 1))) ? 1 : 0);
  }
  return ss.str();
}

void test(const double p) {
  std::cout << "Probability p = " << p << std::endl;
  rbs::generator g(p);
  for (int i = 0; i < 5; i++) {
    uint32_t r = g();
    int nb = rbs::popcount(r);
    std::cout << b2s(r) << " ";
    std::cout << std::setw(2) << nb << " bits" << std::endl;
  }
  std::cout << std::endl;
}

int main() {
  for (int i = 0; i < 4; i++) {
    test((i + 1) * 0.2);
  }
}
