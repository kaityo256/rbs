#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <randbit.hpp>

//const int L = 1 << 15;
const int L = 1 << 12;
const int T = L;
const int SAMPLE = 10;
const double inv = 1.0 / static_cast<double>(L *SAMPLE);

struct DP_Serial {

  void
  calculate(std::vector<double> &n) {
    uint32_t *s1 = new uint32_t[L];
    uint32_t *s2 = new uint32_t[L];
    const double p = 0.6447;
    const uint32_t pi = static_cast<uint32_t>(p * 4294967295);
    n.resize(T, 0.0);
    std::mt19937 mt(1);
    for (int s = 0; s < SAMPLE; s++) {
      std::fill(s1, s1 + L, 1);
      for (int t = 0; t < T; t++) {
        int nt = 0;
        std::fill(s2, s2 + L, 0);
        for (int i = 0; i < L - 1; i++) {
          if (s1[i]) {
            nt++;
            if (mt() < pi) s2[i] = 1;
            if (mt() < pi) s2[i + 1] = 1;
          }
        }
        if (s1[L - 1]) {
          nt++;
          if (mt() < pi) s2[L - 1] = 1;
          if (mt() < pi) s2[0] = 1;
        }
        std::swap(s1, s2);
        n[t] += nt * inv;
      }
    }
    delete [] s1;
    delete [] s2;
  }
};


template <class INT>
struct DP_Bit {
  void
  calculate(std::vector<double> &n) {
    const int size = sizeof(INT) * 8;
    const int LS = L / size;
    INT *s1 = new INT[LS];
    INT *s2 = new INT[LS];
    const int shift = size - 1;
    const double p = 0.6447;
    rbs::generator rg(p);
    n.resize(T, 0.0);
    for (int s = 0; s < SAMPLE; s++) {
      std::fill(s1, s1 + LS, ~INT(0));
      for (int t = 0; t < T; t++) {
        int nt = 0;
        std::fill(s2, s2 + LS, 0);
        for (int i = 0; i < LS; i++) {
          nt += rbs::popcount(s1[i]);
        }
        for (int i = 0; i < LS - 1; i++) {
          s2[i] |= s1[i] & rg();
          INT x = s1[i] & rg();
          s2[i] |= (x << 1);
          s2[i + 1] |= (x >> shift);
        }
        s2[LS - 1] |= s1[LS - 1] & rg();
        INT x = s1[LS - 1] & rg();
        s2[LS - 1] |= (x << 1);
        s2[0] |= (x >> shift);

        std::swap(s1, s2);
        n[t] += nt * inv;
      }
    }
    delete [] s1;
    delete [] s2;
  }
};

template <class C>
double measure(const char *name, C f) {
  const auto s = std::chrono::system_clock::now();
  std::vector<double> n;
  f.calculate(n);
  const auto e = std::chrono::system_clock::now();
  const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(e   - s).count();
  std::cerr << std::left << std::setw(12) << name << " ";
  std::cerr << std::right << std::setw(5) << elapsed << " [ms]";
  std::cerr << std::endl;
  std::stringstream ss;
  ss << name << ".dat";
  std::ofstream ofs(ss.str());
  for (int t = 0; t < T; t++) {
    ofs << t << " " << n[t] << std::endl;
  }
  return static_cast<double>(elapsed);
}

int main(void) {
  double ts = measure("serial", DP_Serial());
  double tm = measure("msc", DP_Bit<uint32_t>());
  std::cout << "Speed up by " << ts / tm << std::endl;
}
