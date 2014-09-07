#pragma once

#include <cassert>
#include <vector>
#include <numeric>
#include <boost/math/constants/constants.hpp>

namespace otl
{
namespace optimizer
{
namespace cdas
{
template <typename _TReal>
void ConvertObjective(const std::vector<_TReal> &angle, const std::vector<_TReal> &objective, std::vector<_TReal> &convertedObjective)
{
	assert(objective.size() == angle.size());
	convertedObjective.resize(objective.size());
	const _TReal radius = sqrt(std::inner_product(objective.begin(), objective.end(), objective.begin(), (_TReal)0));
	for (size_t i = 0; i < objective.size(); ++i)
	{
		assert(0 <= angle[i] && angle[i] <= boost::math::constants::pi<_TReal>());
		convertedObjective[i] = radius * sin(acos(objective[i] / radius) + angle[i]) / sin(angle[i]);
	}
}
}
}
}
