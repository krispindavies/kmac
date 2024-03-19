// Copyright (c) 2024 Krispin Davies
// SPDX-License-Identifier: BSD-3-Clause

#include "kmac/kmac.h"

#include <gtest/gtest.h>

#include <iostream>
#include <tuple>

TEST(test_kmac, initialization)
{
  using KmacFF = Kmac<float, float>;
  KmacFF::Config config;
  config.n_inputs_ = 2;
  config.n_layers_ = 2;
  config.bounds_ = std::vector<Bounds<float>>{{0.0, 1.0}, {0.0, 2.0}};
  config.bin_sizes_ = std::vector<float>{0.5, 1.0};
  config.initial_value_ = [](const std::vector<float>& inputs) { return inputs[0] * inputs[1]; };
  EXPECT_NO_THROW(KmacFF kmac(config));
}

TEST(test_kmac, indexing)
{
  Kmac<float, float>::Config config;
  config.n_inputs_ = 2;
  config.n_layers_ = 2;
  config.bounds_ = std::vector<Bounds<float>>{{0.0, 1.0}, {0.0, 2.0}};
  config.bin_sizes_ = std::vector<float>{0.5, 1.0};
  config.initial_value_ = [](const std::vector<float>& inputs) { return inputs[0] * inputs[1]; };
  Kmac<float, float> kmac(config);

  EXPECT_EQ(0, kmac.indexFromInputs(std::vector<float>{0.0, 0.0}, 0));

  auto expected_result = std::make_tuple(std::vector<float>{0.0, 0.0}, 0);
  EXPECT_EQ(expected_result, kmac.inputsFromIndex(0));
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}