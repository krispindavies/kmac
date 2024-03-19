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
  config.n_inputs_ = 2;
  config.n_layers_ = 2;
  config.bounds_ = std::vector<Bounds<float>>{{0.0, 1.0}, {0.0, 2.0}};
  config.bin_sizes_ = std::vector<float>{0.5, 1.0};
  config.initial_value_ = [](const std::vector<float>& inputs) { return inputs[0] * inputs[1]; };
  Kmac<float, float> kmac(config);

  std::cout << "Done!" << std::endl;
}