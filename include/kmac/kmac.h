// Copyright (c) 2024 Krispin Davies
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <kmac/bounds.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

template <typename InType, typename OutType>
class Kmac
{
public:
  // Helper struct to carry the configuration for initializing a kmac network.
  struct Config
  {
    std::size_t n_inputs;
    std::size_t n_layers;
    std::vector<Bounds<OutType>> bounds;
    std::vector<InType> bin_sizes;
    OutType initial_value;
  };

  // Main constructor.
  Kmac(const Config& config)
  {
    // Check that the config is sensible.
    if (config.n_inputs < 1)
    {
      throw std::runtime_error("Kmac::Kmac: n_inputs must be >= 1.");
    }
    if (config.n_layers < 1)
    {
      throw std::runtime_error("Kmac::Kmac: n_layers must be >= 1.");
    }
    if (config.bounds.size() != config.n_inputs)
    {
      throw std::runtime_error("Kmac::Kmac: bounds must have [" + std::to_string(config.n_inputs) + "] entries.");
    }
    for (const auto& entry : config.bounds)
    {
      if (entry.range() <= 0)
      {
        throw std::runtime_error("Kmac::Kmac: bounds entries must have a range > 0.");
      }
      if (!entry.isfinite())
      {
        throw std::runtime_error("Kmac::Kmac: bounds entries must all be finite.");
      }
    }
    if (config.bin_sizes.size() != config.n_inputs)
    {
      throw std::runtime_error("Kmac::Kmac: bin_sizes must have [" + std::to_string(config.n_inputs) + "] entries.");
    }
    for (const auto& entry : config.bin_sizes)
    {
      if (entry <= 0)
      {
        throw std::runtime_error("Kmac::Kmac: bin_sizes entries must be > 0.");
      }
      if (!std::isfinite(entry))
      {
        throw std::runtime_error("Kmac::Kmac: bin_sizes entries must all be finite.");
      }
    }

    // Initialize the network.
    config_ = config;
    std::size_t n_cells = config.n_layers;
    for (std::size_t index = 0; index < config.n_inputs; index++)
    {
      n_cells *= std::max<InType>(std::ceil(config.bounds[index].range() / config.bin_sizes[index]), 1.0);
    }
    data_ = std::vector<OutType>(n_cells, config.initial_value);
  }

private:
  Config config_;
  std::vector<OutType> data_;
};
