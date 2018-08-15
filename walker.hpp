/*
The MIT License (MIT)

Copyright (c) 2018 Hiroshi Watanabe

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <vector>
#include <numeric>
#include <cstdint>
#include <random>

template <class RNG = std::mt19937>
class walker_alias {
  private:
    unsigned int n;
    unsigned int nb;
    std::vector<int> index;
    std::vector<double> prob;
    std::vector<uint32_t> probi;
    uint32_t int_max = ~static_cast<uint32_t>(0);
  public:
    walker_alias(std::vector<double> a) {
      nb = 1;
      while ((unsigned int)(1 << nb) <= a.size()) {
        nb++;
      }
      n = (1 << nb);
      index.resize(n, 0.0);
      prob.resize(n, 0.0);
      probi.resize(n, 0);
      for (unsigned int i = 0; i < a.size(); i++) {
        prob[i] = a[i];
      }
      double ave = std::accumulate(prob.begin(), prob.end(), 0.0) / prob.size();
      for (auto &v : prob) {
        v /= ave;
      }
      std::vector<int> small, large;
      for (unsigned int i = 0; i < n; i++) {
        if (prob[i] < 1.0) {
          small.push_back(i);
        } else {
          large.push_back(i);
        }
        index[i] = i;
      }
      while (small.size() && large.size()) {
        const int j = small.back();
        small.pop_back();
        const int k = large.back();
        index[j] = k;
        prob[k] = prob[k] - 1.0 + prob[j];
        if (prob[k] < 1.0) {
          small.push_back(k);
          large.pop_back();
        }
      }
      const int imax = int_max >> nb;
      for (unsigned int i = 0; i < n; i++) {
        probi[i] = static_cast<uint32_t>(prob[i] * imax);
      }
    }
    int get_index(RNG &mt) {
      uint32_t r = mt();
      int k = r & (n - 1);
      r = r >> nb;
      if (r > probi[k]) {
        return index[k];
      } else {
        return k;
      }
    }
};
