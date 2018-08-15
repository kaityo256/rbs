#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <chrono>
#include "../randbit.hpp"

std::vector<double> test_points = {
  0.0604, 0.1229, 0.1530, 0.1843, 0.2059, 0.2371, 0.2786, 0.3099, 0.3292, 0.3604, 0.3972, 0.4284, 0.4424, 0.4737, 0.5263, 0.5576, 0.5716, 0.6028, 0.6396, 0.6708, 0.6901, 0.7214, 0.7629, 0.7941, 0.8157, 0.8470, 0.8771, 0.9396
};

std::vector<std::string>test_names = {
  "rbsgen0_0p", "rbsgen3_1n", "rbsgen3_1p", "rbsgen4_3n", "rbsgen4_3p", "rbsgen2_1n", "rbsgen2_1p", "rbsgen4_5n", "rbsgen4_5p", "rbsgen3_3n", "rbsgen3_3p", "rbsgen4_7n", "rbsgen4_7p", "rbsgen1_1n", "rbsgen1_1p", "rbsgen4_9n", "rbsgen4_9p", "rbsgen3_5n", "rbsgen3_5p", "rbsgen4_11n", "rbsgen4_11p", "rbsgen2_3n", "rbsgen2_3p", "rbsgen4_13n", "rbsgen4_13p", "rbsgen3_7n", "rbsgen3_7p", "rbsgen0_1n"
};

// Check wether approriate generator is chosen
void test1() {
  std::cout << "Test1: Generator test" << std::endl;
  for (unsigned int i = 0; i < test_points.size(); i++) {
    const double p = test_points[i];
    rbs::generator g(p);
    std::cout << p << " " << g.name() << std::endl;
    assert(g.name() == test_names[i]);
  }
  std::cout << "Test1 OK" << std::endl;
}

// Check wthere the expected cost is at most 7
void test2() {
  std::cout << "Test3: Expected cost" << std::endl;
  std::ofstream ofs("expected.dat");
  const int D = 1000;
  const double pmax = 1.0;
  for (int i = 1; i < D; i++) {
    double p = static_cast<double>(i) / D * pmax;
    rbs::generator g(p);
    ofs << p << " " <<  g.expected_cost() << std::endl;
    assert(g.expected_cost() < 7.0);
  }
  std::cout << "Test3 OK" << std::endl;
}

double norm(std::vector<double> &v1, std::vector<double> &v2) {
  double sum = 0.0;
  for (unsigned int i = 0; i < v1.size(); i++) {
    sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
  }
  return sum;
}

void test3_sub(const double p) {
  rbs::generator g(p);
  const double epsilon = 1e-6;
  const int N = 4000000;
  const double inv = 1.0 / static_cast<double>(N);
  double ave = 0.0;
  std::vector<double> data(33, 0.0);
  std::vector<double> exact = rbs::generate_binomial<uint32_t>(p);
  const auto s = std::chrono::system_clock::now();
  for (int i = 0; i < N; i++) {
    const int n = rbs::popcount(g());
    ave += n * inv;
    data[n] += inv;
  }
  const auto e = std::chrono::system_clock::now();
  const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
  const double mbps = N * 32 / elapsed / 1000.0;
  const double norm_err = norm(exact, data);
  std::cout << std::setprecision(4);
  std::cout << std::fixed;
  std::cout << p << " ";
  std::cout << std::left << std::setw(11);
  std::cout << g.name();
  std::cout << std::right << std::setw(5) << elapsed << " [ms]";
  std::cout << std::setprecision(1) << std::fixed;
  std::cout << std::right << std::setw(7) << mbps <<  " [MBPS] ";
  std::cout << "ave = " << std::setw(4) << ave;
  std::cout << std::scientific;
  std::cout << " err = " << norm_err << std::endl;
  assert(norm_err < epsilon);
}

// Check whether the generated distribution is sufficiently close to the exact one
void test3() {
  std::cout << "Test2: Accuracy test" << std::endl;
  std::cout << "p      generator   time [ms]  speed [MBPS]   average  error of norm" << std::endl;
  for (auto p : test_points) {
    test3_sub(p);
  }
  std::cout << "Test3 OK" << std::endl;
}

int main(void) {
  test1();
  test2();
  test3();
}
