// Copyright (c) 2024 Krispin Davies
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <kmac/bounds.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>

template <typename InType, typename OutType>
class Kmac
{
public:
  using Inputs = std::vector<InType>;

  // Helper struct to carry the configuration for initializing a kmac network.
  struct Config
  {
    std::size_t n_inputs_;
    std::size_t n_layers_;
    std::vector<Bounds<OutType>> bounds_;
    std::vector<InType> bin_sizes_;
    std::function<OutType(const Inputs&)> initial_value_;
  };

  // Main constructor.
  Kmac(const Config& config)
  {
    // Check that the config is sensible.
    if (config.n_inputs_ < 1)
    {
      throw std::runtime_error("Kmac::Kmac: n_inputs must be >= 1.");
    }
    if (config.n_layers_ < 1)
    {
      throw std::runtime_error("Kmac::Kmac: n_layers must be >= 1.");
    }
    if (config.bounds_.size() != config.n_inputs_)
    {
      throw std::runtime_error("Kmac::Kmac: bounds must have [" + std::to_string(config.n_inputs_) + "] entries.");
    }
    for (const auto& entry : config.bounds_)
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
    if (config.bin_sizes_.size() != config.n_inputs_)
    {
      throw std::runtime_error("Kmac::Kmac: bin_sizes must have [" + std::to_string(config.n_inputs_) + "] entries.");
    }
    for (const auto& entry : config.bin_sizes_)
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

    std::cout << "Here 0." << std::endl;

    // Initialize the network.
    config_ = config;
    data_.clear();
    std::size_t n_cells = config.n_layers_;
    for (std::size_t index = 0; index < config.n_inputs_; index++)
    {
      n_cells *= std::max<InType>(std::ceil(config.bounds_[index].range() / config.bin_sizes_[index]), 1.0);
    }
    data_.reserve(n_cells);
    for (std::size_t index = 0; index < n_cells; index++)
    {
      const auto [inputs, layer] = inputsFromIndex(index);
      data_.push_back(config.initial_value_(inputs));
    }
  }

  std::size_t indexFromInputs(const Inputs& inputs, const std::size_t layer) const
  {
    double axial_index = (inputs.at(0) - minimums_.at(layer).at(0)) / config_.bin_sizes_.at(0);
    axial_index = std::clamp(std::round(axial_index), 0.0, double(bins_counts_.at(0)));
    std::size_t data_index = std::size_t(axial_index);
    for (std::size_t input_index = 1; input_index < config_.n_inputs_; input_index++)
    {
      axial_index = (inputs.at(input_index) - minimums_.at(layer).at(input_index)) / config_.bin_sizes_.at(input_index);
      axial_index = std::clamp(std::round(axial_index), 0.0, double(bins_counts_.at(input_index)));
      data_index = bins_counts_.at(input_index - 1) * data_index + std::size_t(axial_index);
    }
    return bins_counts_.back() * data_index + layer;
  }

  std::tuple<Inputs, std::size_t> inputsFromIndex(std::size_t data_index) const
  {
    // Retrieve the layer that the data index points to.
    const std::size_t layer = data_index % config_.n_layers_;
    data_index /= config_.n_layers_;

    // Retrieve the inputs that the data index points to.
    Inputs inputs;
    inputs.reserve(config_.n_inputs_);
    for (std::size_t input_index = config_.n_inputs_; input_index > 0; input_index--)
    {
      const std::size_t bin_index = data_index % bins_counts_.at(input_index - 1);
      inputs.push_back(bin_index * config_.bin_sizes_.at(input_index - 1) + minimums_.at(layer).at(input_index - 1));
      data_index /= bins_counts_.at(input_index - 1);
    }
    return std::tie(inputs, layer);
  }

private:
  Config config_;
  std::vector<std::vector<InType>> minimums_;
  std::vector<std::size_t> bins_counts_;
  std::vector<OutType> data_;
};
