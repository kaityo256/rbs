#include <iostream>
#include <fstream>
#include <cstdint>
#include <chrono>
#include <iomanip>
#include <cassert>
#include <string>
#include <sstream>
#include <algorithm>
#include <randbit.hpp>

double norm(std::vector<double> &v1, std::vector<double> &v2) {
  double sum = 0.0;
  for (unsigned int i = 0; i < v1.size(); i++) {
    sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
  }
  return sum;
}

template<class RNG>
void test_sub(const double p, std::ostream &os) {
  const double epsilon = 1e-4;
  const int N = 10000000;
  const double inv = 1.0 / static_cast<double>(N);
  RNG g(p);
  double ave = 0.0;
  std::vector<double> data(33, 0.0);
  std::vector<double> exact = rbs::generate_binomial<uint32_t>(p);
  const auto s = std::chrono::system_clock::now();
  for (int i = 0; i < N; i++) {
    const int n = rbs::popcount(g.gen());
    ave += n * inv;
    data[n] += inv;
  }
  const auto e = std::chrono::system_clock::now();
  const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
  const double mbps = N * 32 / elapsed * 1e-3;
  const double norm_err = norm(exact, data);
  os << p << " " << elapsed << " " << mbps << " " << norm_err << std::endl;
  std::cout << p << " " << elapsed << " " << mbps << " error= " << norm_err <<  std::endl;
  assert(norm_err < epsilon);
}

template <class RNG>
void test(const double ps, const double pe) {
  const int ND = 20;
  std::stringstream ss;
  RNG g((ps + pe) * 0.5);
  ss << g.name() << ".dat";
  std::cout << g.name() << std::endl;
  std::ofstream ofs(ss.str());
  for (int i = 0; i <= ND; i++) {
    const double p = (pe - ps) / static_cast<double>(ND) * i + ps;
    test_sub<RNG>(p, ofs);
  }
}

int main(void) {
  test<rbs::rbsgen0_0p<uint32_t>>(0.01, 0.125);
  test<rbs::rbsgen3_1n<uint32_t>>(0.10, 0.125);
  test<rbs::rbsgen3_1p<uint32_t>>(0.125, 0.1875);
  test<rbs::rbsgen4_3n<uint32_t>>(0.16, 0.1875);
  test<rbs::rbsgen4_3p<uint32_t>>(0.1875, 0.25);
  test<rbs::rbsgen2_1n<uint32_t>>(0.2, 0.25);
  test<rbs::rbsgen2_1p<uint32_t>>(0.25, 0.3125);
  test<rbs::rbsgen4_5n<uint32_t>>(0.29, 0.3125);
  test<rbs::rbsgen4_5p<uint32_t>>(0.3125, 0.375);
  test<rbs::rbsgen3_3n<uint32_t>>(0.32, 0.375);
  test<rbs::rbsgen3_3p<uint32_t>>(0.375, 0.4375);
  test<rbs::rbsgen4_7n<uint32_t>>(0.40, 0.4375);
  test<rbs::rbsgen4_7p<uint32_t>>(0.4375, 0.48);
  test<rbs::rbsgen1_1n<uint32_t>>(0.435, 0.499);
}
