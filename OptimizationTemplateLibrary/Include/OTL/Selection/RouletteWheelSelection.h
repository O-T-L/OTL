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
_TIterator RouletteWheelSelect(const _TReal rouletteWheel, _TIterator begin, _TIterator end, _TEvaluate evaluate)
{
	_TReal seek = 0;
	for (_TIterator i = begin; i != end; ++i)
	{
		const _TReal fitness = evaluate(*i);
		assert(fitness >= 0);
		seek += fitness;
		if (seek >= rouletteWheel)
			return i;
	}
	assert(false);
	return end;
}

template <typename _TReal, typename _TRandom, typename _TSrcIterator, typename _TDestIterator, typename _TEvaluate, typename _TCopy>
void RouletteWheelSelection(_TRandom &random, _TSrcIterator srcBegin, _TSrcIterator srcEnd, _TDestIterator destBegin, _TDestIterator destEnd, _TEvaluate evaluate, _TCopy copy)
{
	typedef typename std::iterator_traits<_TSrcIterator>::value_type _TPointer;
	const _TReal sum = std::accumulate(srcBegin, srcEnd, (_TReal)0, [evaluate](const _TReal sum, _TPointer individual)->_TReal{return sum + evaluate(individual);});
	assert(sum >= 0);
	if (sum > 0)
	{
		std::uniform_real_distribution<_TReal> dist(0, sum);
		for (_TDestIterator i = destBegin; i != destEnd; ++i)
		{
			auto elite = RouletteWheelSelect(dist(random), srcBegin, srcEnd, evaluate);
			copy(*elite, *i);
		}
	}
	else
	{
		std::vector<_TPointer> population(srcBegin, srcEnd);
		std::random_shuffle(population.begin(), population.end(), [&random](const size_t n)-> size_t{std::uniform_int_distribution<size_t> dist(0, n - 1);return dist(random);});
		size_t index = 0;
		for (_TDestIterator i = destBegin; i != destEnd; ++i)
		{
			copy(population[index], *i);
			++index;
		}
	}
}
}
}
