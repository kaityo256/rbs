#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <randbit.hpp>

//const int L = 1 << 15;
const int L = 1 << 12;
const int T = L;
const int SAMPLE = 1000;
const double sinv = 1.0 / static_cast<double>(SAMPLE);

template <class INT = uint32_t>
struct DP_Bit {

  void calculate(std::vector<double> &n) {
    const int size = sizeof(INT) * 8;
    const int shift = size - 1;
    const int LS = L / size;
    n.resize(T, 0);
    const double p = 0.6447;
    rbs::generator rg(p);
    INT *s1 = new INT[LS + 1];
    INT *s2 = new INT[LS + 1];
    for (int s = 0; s < SAMPLE; s++) {
      std::fill(s1, s1 + LS, 0);
      s1[0] = 1;
      int max = 0;
      for (int t = 0; t < T; t++) {
        int nt = 0;
        std::fill(s2, s2 + LS, 0);
        for (int i = 0; i < max + 1; i++) {
          int ni = rbs::popcount(s1[i]);
          if (ni != 0) {
            nt += ni;
            INT x1 = s1[i] & rg();
            INT x2 = s1[i] & rg();
            s2[i] |= x1 | (x2 << 1);
            s2[i + 1] = (x2 >> shift);
            if ((x2 >> shift) && i >= max) {
              max = (i + 1) % L;
            }
          }
        }
        std::swap(s1, s2);
        n[t] += nt * sinv;
        if (nt == 0)break;
      }
    }
    delete [] s1;
    delete [] s2;
  }

};

struct DP_Serial {

  void calculate(std::vector<double> &n) {
    std::mt19937 mt(1);
    const double p = 0.6447;
    const uint32_t pi = static_cast<uint32_t>(p * 4294967295);
    n.resize(T, 0);
    int *s1 = new int[L];
    int *s2 = new int[L];
    for (int s = 0; s < SAMPLE; s++) {
      std::fill(s1, s1 + L, 0);
      s1[0] = 1;
      int max = 0;
      for (int t = 0; t < T; t++) {
        int nt = 0;
        std::fill(s2, s2 + L, 0);
        for (int i = 0; i < max + 1; i++) {
          if (s1[i]) {
            nt++;
            if (mt() < pi) {
              s2[i] = 1;
            }
            if (mt() < pi) {
              if (i + 1 > max) max = i + 1;
              s2[i + 1] = 1;
            }
          }
        }
        std::swap(s1, s2);
        n[t] += nt * sinv;
        if (nt == 0)break;
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
