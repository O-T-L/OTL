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
#include <iterator>
#include <OTL/Solution.h>
#include <OTL/Crossover/Crossover.h>
#include <OTL/Selection/TournamentSelection.h>

namespace otl
{
namespace optimizer
{
namespace nsga_ii
{
template <typename _TIterator, typename _TRandom, typename _TCompete>
std::vector<typename std::iterator_traits<_TIterator>::pointer> MatingSelection(const size_t offspringSize, _TIterator begin, _TIterator end, _TRandom &random, _TCompete compete)
{
	typedef typename std::iterator_traits<_TIterator>::pointer _TPointer;
	std::vector<_TPointer> ancestor(std::distance(begin, end));
	{
		_TIterator src = begin;
		for (size_t i = 0; i < ancestor.size(); ++i, ++src)
			ancestor[i] = &*src;
	}
	std::vector<_TPointer> selected(offspringSize);
	otl::selection::TournamentSelection(random, ancestor.begin(), ancestor.end(), selected.begin(), selected.end(), compete);
	return selected;
}

template <typename _TIndividual, typename _TPointer>
void PerformCrossover(std::vector<_TIndividual> &offspring, const std::vector<_TPointer> &ancestor, otl::crossover::Crossover<typename _TIndividual::TReal, typename _TIndividual::TDecision> &crossover)
{
	typedef typename _TIndividual::TReal _TReal;
	typedef typename _TIndividual::TDecision _TDecision;
	typedef typename otl::crossover::Crossover<_TReal, _TDecision>::TSolution _TSolution;
	std::vector<const _TSolution *> _ancestor(ancestor.size());
	for (size_t i = 0; i < ancestor.size(); ++i)
		_ancestor[i] = ancestor[i];
	std::vector<_TSolution *> _offspring(offspring.size());
	for (size_t i = 0; i < offspring.size(); ++i)
		_offspring[i] = &offspring[i];
	crossover(_ancestor, _offspring);
}

template <typename _TIterator, typename _TRandom, typename _TCompete>
std::vector<typename std::iterator_traits<_TIterator>::value_type> MakeOffspring(const size_t offspringSize, _TIterator begin, _TIterator end, _TRandom &random, _TCompete compete, otl::crossover::Crossover<typename std::iterator_traits<_TIterator>::value_type::TReal, typename std::iterator_traits<_TIterator>::value_type::TDecision> &crossover)
{
	typedef typename std::iterator_traits<_TIterator>::value_type _TIndividual;
	const auto selected = MatingSelection(offspringSize, begin, end, random, compete);
	std::vector<_TIndividual> offspring(selected.size());
	PerformCrossover(offspring, selected, crossover);
	return offspring;
}
}
}
}
