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
#include <list>
#include <iterator>
#include <numeric>
#include <random>

namespace otl
{
namespace optimizer
{
namespace hype
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
std::vector<_TReal> FindLower(_TIterator begin, _TIterator end)
{
	assert(begin != end);
	std::vector<_TReal> lower((**begin).objective_.size());
	for (size_t obj = 0; obj < lower.size(); ++obj)
	{
		lower[obj] = (**begin).objective_[obj];
		for (_TIterator i = ++_TIterator(begin); i != end; ++i)
		{
			assert((**i).objective_.size() == lower.size());
			const _TReal coordinate = (**i).objective_[obj];
			if (coordinate < lower[obj])
				lower[obj] = coordinate;
		}
	}
	return lower;
}

template <typename _TIterator, typename _TReal>
std::list<typename std::iterator_traits<_TIterator>::value_type> FindDominators(_TIterator begin, _TIterator end, const std::vector<_TReal> &point)
{
	typedef typename std::iterator_traits<_TIterator>::value_type _TPointer;
	std::list<_TPointer> dominators;
	for (_TIterator i = begin; i != end; ++i)
	{
		if (otl::utility::relation::WeaklyDominate((**i).objective_, point))
			dominators.push_back(*i);
	}
	return dominators;
}

template <typename _TIterator, typename _TReal>
std::list<typename std::iterator_traits<_TIterator>::value_type> FindDominators(_TIterator begin, _TIterator end, const std::vector<_TReal> &point, size_t limits)
{
	typedef typename std::iterator_traits<_TIterator>::value_type _TPointer;
	std::list<_TPointer> dominators;
	for (_TIterator i = begin; i != end && limits; ++i)
	{
		if (otl::utility::relation::WeaklyDominate((**i).objective_, point))
		{
			dominators.push_back(*i);
			--limits;
		}
	}
	return dominators;
}

template <typename _TReal>
_TReal CalculateAlpha(const size_t populationSize, const size_t remove, const size_t nDominators)
{
	assert(populationSize > 0);
	assert(0 < nDominators && nDominators <= populationSize);
	_TReal alpha = 1;
	for (size_t i = 1; i < nDominators - 1; ++i)
		alpha *= (_TReal)(remove - i) / (populationSize - i);
	return alpha / nDominators;
}

template <typename _TReal>
std::vector<_TReal> CalculateAlphas(const size_t populationSize, const size_t remove, const size_t nMaxDominators)
{
	std::vector<_TReal> alphas(nMaxDominators + 1);
	alphas[0] = 0;
	for(size_t nDominators = 1; nDominators < alphas.size(); ++nDominators)
		alphas[nDominators] = CalculateAlpha<_TReal>(populationSize, remove, nDominators);
	return alphas;
}

template <typename _TReal>
std::vector<_TReal> CalculateAlphas(const size_t populationSize, const size_t remove)
{
	return CalculateAlphas<_TReal>(populationSize, remove, populationSize);
}

template <typename _TRandom, typename _TIterator, typename _TReal>
void FitnessEstimation(_TRandom &random, _TIterator begin, _TIterator end, const std::vector<_TReal> &referencePoint, const size_t nSample, const size_t remove)
{
	assert(0 < remove && remove <= std::distance(begin, end));
	const std::vector<_TReal> lower = FindLower<_TReal>(begin, end);
	assert(referencePoint.size() == lower.size());
	for (_TIterator i = begin; i != end; ++i)
		(**i).fitness_ = 0;
	const std::vector<_TReal> alphas = CalculateAlphas<_TReal>(std::distance(begin, end), remove);
	for (size_t sample = 0; sample < nSample; ++sample)
	{
		const std::vector<_TReal> point = GeneratePoint(random, lower, referencePoint);
		const auto dominators = FindDominators(begin, end, point);
		const _TReal alpha = alphas[dominators.size()];
		for (auto i = dominators.begin(); i != dominators.end(); ++i)
			(**i).fitness_ += alpha;
	}
	const _TReal volume = std::inner_product(referencePoint.begin(), referencePoint.end(), lower.begin(), (_TReal)1, std::multiplies<_TReal>(), std::minus<_TReal>());
	for (_TIterator i = begin; i != end; ++i)
		(**i).fitness_ *= volume / nSample;
}

template <typename _TRandom, typename _TIterator, typename _TReal>
void FastFitnessEstimation(_TRandom &random, _TIterator begin, _TIterator end, const std::vector<_TReal> &referencePoint, const size_t nSample, const size_t remove)
{
	assert(0 < remove && remove <= std::distance(begin, end));
	const std::vector<_TReal> lower = FindLower<_TReal>(begin, end);
	assert(referencePoint.size() == lower.size());
	for (_TIterator i = begin; i != end; ++i)
		(**i).fitness_ = 0;
	const std::vector<_TReal> alphas = CalculateAlphas<_TReal>(std::distance(begin, end), remove, remove);
	for (size_t sample = 0; sample < nSample; ++sample)
	{
		const std::vector<_TReal> point = GeneratePoint(random, lower, referencePoint);
		const auto dominators = FindDominators(begin, end, point, remove);
		const _TReal alpha = alphas[dominators.size()];
		for (auto i = dominators.begin(); i != dominators.end(); ++i)
			(**i).fitness_ += alpha;
	}
	const _TReal volume = std::inner_product(referencePoint.begin(), referencePoint.end(), lower.begin(), (_TReal)1, std::multiplies<_TReal>(), std::minus<_TReal>());
	for (_TIterator i = begin; i != end; ++i)
		(**i).fitness_ *= volume / nSample;
}
}
}
}
