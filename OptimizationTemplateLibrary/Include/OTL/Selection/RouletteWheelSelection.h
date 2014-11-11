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
_TIterator RouletteWheelSelect(const _TReal minFitness, const _TReal rouletteWheel, _TIterator begin, _TIterator end, _TEvaluate evaluate)
{
	_TReal seek = 0;
	for (_TIterator i = begin; i != end; ++i)
	{
		const _TReal fitness = evaluate(*i) - minFitness;
		assert(fitness >= 0);
		seek += fitness;
		if (seek >= rouletteWheel)
			return i;
	}
	assert(false);
	return end;
}

template <typename _TReal, typename _TRandom, typename _TPointer, typename _TEvaluate>
std::list<_TPointer> RouletteWheelSelection(size_t nElites, _TRandom &random, std::list<_TPointer> &population, _TEvaluate evaluate)
{
	const _TReal minFitness = evaluate(*std::min_element(population.begin(), population.end(), [evaluate](_TPointer individual1, _TPointer individual2)->bool{return evaluate(individual1) < evaluate(individual2);}));
	_TReal sum = std::accumulate(population.begin(), population.end(), (_TReal)0, [evaluate](const _TReal sum, _TPointer individual)->_TReal{return sum + evaluate(individual);}) - minFitness * population.size();
	assert(sum >= 0);
	std::list<_TPointer> elites;
	while (nElites && sum > 0)
	{
		std::uniform_real_distribution<_TReal> dist(0, sum);
		auto elite = RouletteWheelSelect(minFitness, dist(random), population.begin(), population.end(), evaluate);
		const _TReal fitness = evaluate(*elite) - minFitness;
		assert(fitness >= 0);
		sum -= fitness;
		assert(sum >= 0);
		elites.splice(elites.end(), population, elite);
		--nElites;
	}
	if (nElites > 0)
	{
		std::vector<_TPointer> _population(population.begin(), population.end());
		std::random_shuffle(_population.begin(), _population.end(), [&random](const size_t n)-> size_t{std::uniform_int_distribution<size_t> dist(0, n - 1);return dist(random);});
		for (size_t i = 0; i < nElites; ++i)
			elites.push_back(_population[i]);
	}
	return elites;
}
}
}
