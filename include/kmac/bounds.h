// Copyright (c) 2024 Krispin Davies
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <algorithm>
#include <cmath>

template <typename DataType>
class Bounds
{
public:
	Bounds(const DataType& lower_bound, const DataType& upper_bound)
	{
		lower_bound_ = lower_bound;
		upper_bound_ = upper_bound;
	}

	[[nodiscard]] DataType clamp(const DataType& value) const
	{
		return std::clamp(value, lower_bound_, upper_bound_);
	}

	[[nodiscard]] DataType range() const
	{
		return upper_bound_ - lower_bound_;
	}

	[[nodiscard]] bool isfinite() const
	{
		return std::isfinite(lower_bound_) && std::isfinite(upper_bound_);
	}

private:
	DataType lower_bound_;
	DataType upper_bound_;
};
