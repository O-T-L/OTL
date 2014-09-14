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
#include <algorithm>
#include <iterator>
#include <random>

namespace otl
{
namespace selection
{
template <typename _TReal, typename _TIterator, typename _TEvaluate>
_TReal FitnessSum(_TIterator begin, _TIterator end, _TEvaluate evaluate)
{
	_TReal fitnessSum = 0;
	for (_TIterator i = begin; i != end; ++i)
	{
		const _TReal fitness = evaluate(*i);
		assert(fitness >= 0);
		fitnessSum += fitness;
	}
	return fitnessSum;
}

template <typename _TReal, typename _TRandom, typename _TIterator>
_TIterator RandomSelect(_TRandom &random, _TIterator begin, _TIterator end, std::random_access_iterator_tag)
{
	std::uniform_int_distribution<size_t> dist(0, std::distance(begin, end) - 1);
	return begin + dist(random);
}

template <typename _TReal, typename _TRandom, typename _TIterator, typename _TIteratorTag>
_TIterator RandomSelect(_TRandom &random, _TIterator begin, _TIterator end, _TIteratorTag)
{
	std::uniform_int_distribution<size_t> dist(0, std::distance(begin, end) - 1);
	_TIterator where = begin;
	for (size_t count = dist(random); count; --count)
		++where;
	return where;
}

template <typename _TReal, typename _TIterator, typename _TEvaluate>
_TIterator RouletteWheelSelect(const _TReal rouletteWheel, _TIterator begin, _TIterator end, _TEvaluate evaluate)
{
	_TReal seek = 0;
	for (_TIterator i = begin; i != end; ++i)
	{
		const _TReal fitness = evaluate(*i);
		if (fitness > 0)
			seek += fitness;
		if (seek >= rouletteWheel)
			return i;
	}
}

template <typename _TReal, typename _TRandom, typename _TSrcIterator, typename _TDestIterator, typename _TEvaluate, typename _TCopy>
void RouletteWheelSelection(_TRandom &random, _TSrcIterator srcBegin, _TSrcIterator srcEnd, _TDestIterator destBegin, _TDestIterator destEnd, _TEvaluate evaluate, _TCopy copy)
{
	assert(std::distance(srcBegin, srcEnd) > std::distance(destBegin, destEnd));
	const _TReal fitnessSum = FitnessSum<_TReal>(srcBegin, srcEnd, evaluate);
	assert(fitnessSum >= 0);
	if (fitnessSum > 0)
	{
		std::uniform_real_distribution<_TReal> dist(0, fitnessSum);
		for (_TDestIterator dest = destBegin; dest != destEnd; ++dest)
			copy(*RouletteWheelSelect(dist(random), srcBegin, srcEnd, evaluate), *dest);
	}
	else
	{
		for (_TDestIterator dest = destBegin; dest != destEnd; ++dest)
			copy(*RandomSelect<_TReal>(random, srcBegin, srcEnd, typename std::iterator_traits<_TSrcIterator>::iterator_category()), *dest);
	}
}
}
}
