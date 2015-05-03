/*!
Copyright (C) 2014, 申瑞珉 (Ruimin Shen)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <cassert>
#include <vector>
#include <random>

namespace otl
{
namespace optimizer
{
namespace sms_emoa
{
template <typename _TRandom, typename _TReal>
std::vector<_TReal> GeneratePoint(_TRandom &random, const std::vector<_TReal> &lower, const std::vector<_TReal> &upper)
{
	assert(lower.size() == upper.size());
	std::vector<_TReal> point(lower.size());
	for (size_t i = 0; i < point.size(); ++i)
	{
		std::uniform_real_distribution<_TReal> dist(lower[i], upper[i]);
		point[i] = dist(random);
	}
	return point;
}

template <typename _TReal, typename _TIterator>
bool IsInsideContributionArea(const std::vector<_TReal> &point, _TIterator current, _TIterator begin, _TIterator end)
{
	if (!otl::utility::relation::WeaklyDominate((**current).objective_, point))
		return false;
	for (_TIterator i = begin; i != end; ++i)
	{
		if (i != current)
		{
			if (otl::utility::relation::WeaklyDominate((**i).objective_, point))
				return false;
		}
	}
	return true;
}

template <typename _TReal, typename _TIterator, typename _TRandom>
std::vector<size_t> Sampling(const std::vector<_TReal> &lower, const std::vector<_TReal> &upper, _TIterator begin, _TIterator end, _TRandom &random, const size_t nSample)
{
	std::vector<size_t> counters(std::distance(begin, end), 0);
	for (size_t sample = 0; sample < nSample; ++sample)
	{
		const std::vector<_TReal> point = GeneratePoint(random, lower, upper);
		size_t index = 0;
		for (_TIterator i = begin; i != end; ++i)
		{
			if (IsInsideContributionArea(point, i, begin, end))
			{
				++counters[index];
				break;
			}
			++index;
		}
	}
	return counters;
}

template <typename _TRandom, typename _TIterator, typename _TReal>
void ContributionEstimation(_TRandom &random, _TIterator begin, _TIterator end, const std::vector<_TReal> &lower, const std::vector<_TReal> &upper, const size_t nSample)
{
	typedef typename std::iterator_traits<_TIterator>::value_type _TPointer;
	const std::vector<size_t> counters = Sampling(lower, upper, begin, end, random, nSample);
	const _TReal volume = std::inner_product(upper.begin(), upper.end(), lower.begin(), (_TReal)1, std::multiplies<_TReal>(), std::minus<_TReal>());
	size_t index = 0;
	for (_TIterator i = begin; i != end; ++i)
	{
		(**i).hvContribution_ = counters[index] * volume / nSample;
		++index;
	}
}
}
}
}
