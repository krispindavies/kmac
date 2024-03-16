// Copyright (c) 2024 Krispin Davies
// SPDX-License-Identifier: BSD-3-Clause

#include "kmac/kmac.h"

#include <iostream>
#include <tuple>

int main(int argc, char** argv)
{
  std::ignore = argc;
  std::ignore = argv;

  Kmac<float, float>::Config config;
  config.n_inputs = 3;
  config.n_layers = 10;
  config.bounds = std::vector<Bounds<float>>{{0.0, 0.2}, {0.0, 0.2}, {0.0, 0.2}};
  config.bin_sizes = std::vector<float>{0.01, 0.01, 0.01};
  config.initial_value = 0.0;
  Kmac<float, float> kmac(config);

  std::cout << "Done!" << std::endl;
}