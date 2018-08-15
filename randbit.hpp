/*

Randome Bit String Generator (RBS)

The MIT License (MIT)

Copyright (c) 2018 Hiroshi Watanabe, Satoshi Morita, Synge Todo, and Naoki Kawashima

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once
#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <cassert>
#include "walker.hpp"

namespace rbs {

const uint32_t default_seed = std::mt19937::default_seed;

template <class T> int popcount(T v);

int popcount(uint32_t v) {
  return __builtin_popcount(v);
}

int popcount(uint64_t x) {
  uint32_t *v = (uint32_t *)(&x);
  return __builtin_popcount(v[0]) + __builtin_popcount(v[1]);
}

double binom(int n, int k) {
  k = std::min(k, n - k);
  return k == 0 ? 1 : static_cast<double>(binom(n - 1, k - 1) * n / k);
}

template <class T>
std::vector<double> generate_binomial(double p) {
  std::vector<double> v;
  v.clear();
  const int n = sizeof(T) * 8;
  for (int i = 0; i < n; i++) {
    double x = binom(n, i) * pow(p, i) * pow(1.0 - p, n - i);
    v.push_back(x);
  }
  return v;
}

std::vector<double> generate_poisson(double la) {
  std::vector<double> v;
  v.clear();
  double x = exp(-la);
  int k = 0;
  v.push_back(x);
  while (k < la  || x > 1e-14) {
    k++;
    x = x * la / k;
    v.push_back(x);
  }
  return v;
}

template<class INT, class RNG_BASE = std::mt19937, class RNG = std::mt19937>
class rbsgen {
  protected:
    const int n;
    const int m;
    const int digit;
    const int numerator;
    const double pn;
    const double pp;
    const double lambda;
    walker_alias<> wa;
    bool positive;
    RNG r;
    RNG_BASE rb;
    std::string namestr;

  public:

    rbsgen(const double p, const int d, const int nu) :
      n(sizeof(INT) * 8),
      m(n - 1),
      digit(d),
      numerator(nu),
      pn(static_cast<double>(nu) / (1 << digit)),
      pp((p - pn) / (1 - pn)),
      lambda(-n * log(1.0 - pp)),
      wa(generate_poisson(lambda)) {
      std::stringstream ss;
      ss << "rbsgen" << d << "_" << nu << "p";
      namestr = ss.str();
    }

    rbsgen(const double p, const int d, const int nu, int dummy) :
      n(sizeof(INT) * 8),
      m(n - 1),
      digit(d),
      numerator(nu),
      pn(static_cast<double>(nu) / (1 << digit)),
      pp((pn - p) / pn),
      lambda(-n * log(1.0 - pp)),
      wa(generate_poisson(lambda)) {
      std::stringstream ss;
      ss << "rbsgen" << d << "_" << nu << "n";
      namestr = ss.str();
    }

    virtual INT gen() {
      INT y = 0;
      unsigned int k = wa.get_index(r);
      for (unsigned int i = 0; i < k; i++) {
        y |= (INT(1) << (r() & m));
      }
      return y;
    }

    double expected_cost() {
      return lambda + 1 + digit;
    }

    std::string name() {
      return namestr;
    }

    virtual
    ~rbsgen() {};
};

#define RBSGEN(DIGIT,NUM,S) \
template<class INT, class RNG_BASE = std::mt19937, class RNG = std::mt19937>\
class rbsgen ## DIGIT ##_ ## NUM ## n : public rbsgen<INT, RNG_BASE, RNG> { \
  public:\
    rbsgen ## DIGIT ##_ ## NUM ## n(const double p) : rbsgen<INT, RNG_BASE, RNG>(p, DIGIT, NUM, 0) {};\
    INT gen() {\
      INT z = 0;\
      unsigned int k = this->wa.get_index(this->r);\
      for (unsigned int i = 0; i < k; i++) {\
        z |= (INT(1) << (this->r() & this->m));\
      }\
      INT y = S;\
      return y & ~z;\
    }\
};\
template<class INT, class RNG_BASE = std::mt19937, class RNG = std::mt19937> \
class rbsgen ## DIGIT ## _ ## NUM ## p : public rbsgen<INT, RNG_BASE, RNG> {\
  public:\
    rbsgen ## DIGIT ## _ ## NUM ## p(const double p) : rbsgen<INT, RNG_BASE, RNG>(p, DIGIT, NUM) {}\
    INT gen() {\
      INT s = S;\
      unsigned int k = this->wa.get_index(this->r);\
      for (unsigned int i = 0; i < k; i++) {\
        s |= (INT(1) << (this->r() & this->m));\
      }\
      return s;\
    }\
};\

/*

RBSGEN(DIGIT,NUM,S)

DIGIT: number of digit for approximation in binary notation
NUM: numerator of approximated probability
S: expression for the initial bit string

The approximated probability pn is given by NUM/(1<<DIGIT)
S should be consistent with the approximated probability.

Example)

RBSGEN(4, 3,  this->rb() & this->rb() & (this->rb() | this->rb()));

The above macro will be expanded as follows.

For negative slope (p < pn)
template<class INT, class RNG_BASE = std::mt19937, class RNG = std::mt19937>
class rbsgen4_3n : public rbsgen<INT, RNG_BASE, RNG> {
  public:
    rbsgen4_3n(const double p) : rbsgen<INT, RNG_BASE, RNG>(p, 4, 3, 0) {};

    INT gen() {
      INT z = 0;
      unsigned int k = this->wa.get_index();
      for (unsigned int i = 0; i < k; i++) {
        z |= (INT(1) << (this->r() & this->m));
      }
      INT y = this->rb() & this->rb() & (this->rb() | this->rb());
      return y & ~z;
    }
};

For positive slope (pn > p)
template<class INT, class RNG_BASE = std::mt19937, class RNG = std::mt19937>
class rbsgen4_3p : public rbsgen<INT, RNG_BASE, RNG> {
  public:
    rbsgen4_3p(const double p) : rbsgen<INT, RNG_BASE, RNG>(p, 4, 3) {}

    INT gen() {
      INT s = this->rb() & this->rb() & (this->rb() | this->rb());
      unsigned int k = this->wa.get_index();
      for (unsigned int i = 0; i < k; i++) {
        s |= (INT(1) << (this->r() & this->m));
      }
      return s;
    }
};
*/

RBSGEN(0, 0,  0);
RBSGEN(0, 1,  ~INT(0));
RBSGEN(1, 1,  this->rb());
RBSGEN(2, 1,  this->rb() & this->rb());
RBSGEN(2, 3,  this->rb() | this->rb());
RBSGEN(3, 1,  this->rb() & this->rb() & this->rb());
RBSGEN(3, 3,  this->rb() & (this->rb() | this->rb()));
RBSGEN(3, 5,  this->rb() | (this->rb() & this->rb()));
RBSGEN(3, 7,  this->rb() | this->rb() | this->rb());
RBSGEN(4, 3,  this->rb() & this->rb() & (this->rb() | this->rb()));
RBSGEN(4, 5,  this->rb() & (this->rb() | (this->rb() & this->rb())));
RBSGEN(4, 7,  this->rb() & (this->rb() | (this->rb() | this->rb())));
RBSGEN(4, 9,  this->rb() | (this->rb() & (this->rb() & this->rb())));
RBSGEN(4, 11, this->rb() | (this->rb() & (this->rb() | this->rb())));
RBSGEN(4, 13, this->rb() | this->rb() | (this->rb() & this->rb()));

class generator {
  private:
    rbsgen<uint32_t> *rbs;
  public:
    generator(const double p, const uint32_t seed = default_seed) {
      assert(p > 0.0 && p < 1.0);
      if (p <  0.12071342273525043) {
        rbs = new rbsgen0_0p<uint32_t>(p);
      } else if (p < 1.0 / (1 << 3)) {
        rbs = new rbsgen3_1n<uint32_t>(p);
      } else if (p < 0.18105811868061228) {
        rbs = new rbsgen3_1p<uint32_t>(p);
      } else if (p < 3.0 / (1 << 4)) {
        rbs = new rbsgen4_3n<uint32_t>(p);
      } else if (p < 0.22423500588948753) {
        rbs = new rbsgen4_3p<uint32_t>(p);
      } else if (p < 1.0 / (1 << 2)) {
        rbs = new rbsgen2_1n<uint32_t>(p);
      } else if (p < 0.30725823681945097) {
        rbs = new rbsgen2_1p<uint32_t>(p);
      } else if (p < 5.0 / (1 << 4)) {
        rbs = new rbsgen4_5n<uint32_t>(p);
      } else if (p < 0.345837720898041) {
        rbs = new rbsgen4_5p<uint32_t>(p);
      } else if (p < 3.0 / (1 << 3)) {
        rbs = new rbsgen3_3n<uint32_t>(p);
      } else if (p < 0.4193542193758186) {
        rbs = new rbsgen3_3p<uint32_t>(p);
      } else if (p < 7.0 / (1 << 4)) {
        rbs = new rbsgen4_7n<uint32_t>(p);
      } else if (p < 0.44731305654603837) {
        rbs = new rbsgen4_7p<uint32_t>(p);
      } else if (p < 1.0 / (1 << 1)) {
        rbs = new rbsgen1_1n<uint32_t>(p);
      } else if (p < 0.5526869434539616) {
        rbs = new rbsgen1_1p<uint32_t>(p);
      } else if (p < 9.0 / (1 << 4)) {
        rbs = new rbsgen4_9n<uint32_t>(p);
      } else if (p < 0.5806457806241814) {
        rbs = new rbsgen4_9p<uint32_t>(p);
      } else if (p < 5.0 / (1 << 3)) {
        rbs = new rbsgen3_5n<uint32_t>(p);
      } else if (p < 0.654162279101959) {
        rbs = new rbsgen3_5p<uint32_t>(p);
      } else if (p < 11.0 / (1 << 4)) {
        rbs = new rbsgen4_11n<uint32_t>(p);
      } else if (p < 0.692741763180549) {
        rbs = new rbsgen4_11p<uint32_t>(p);
      } else if (p < 3.0 / (1 << 2)) {
        rbs = new rbsgen2_3n<uint32_t>(p);
      } else if (p < 0.7757649941105125) {
        rbs = new rbsgen2_3p<uint32_t>(p);
      } else if (p < 13.0 / (1 << 4)) {
        rbs = new rbsgen4_13n<uint32_t>(p);
      } else if (p < 0.8189418813193877) {
        rbs = new rbsgen4_13p<uint32_t>(p);
      } else if (p < 7.0 / (1 << 3)) {
        rbs = new rbsgen3_7n<uint32_t>(p);
      } else if (p < 0.8792865772647496) {
        rbs = new rbsgen3_7p<uint32_t>(p);
      } else {
        rbs = new rbsgen0_1n<uint32_t>(p);
      }
    }

    ~generator() {
      delete rbs;
    }

    uint32_t operator()() {
      return rbs->gen();
    }

    double expected_cost() {
      return rbs->expected_cost();
    }

    std::string name() {
      return rbs->name();
    }
};

}// end of namespace rbs
