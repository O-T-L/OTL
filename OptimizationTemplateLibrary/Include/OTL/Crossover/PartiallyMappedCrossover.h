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

/*
parent1 =	(1 2 3|4 5 6 7|8 9)
parent2 =	(4 5 2|1 8 7 6|9 3)
			(=====|       |===)
child1 =	(4 2 3|1 8 7 6|5 9)
child2 =	(1 8 2|4 5 6 7|9 3)
*/

#pragma once

#include <algorithm>
#include <limits>
#include <random>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/WithProbability.h>
#include "CoupleCoupleCrossover.h"

namespace otl
{
namespace crossover
{
template <typename _TReal, typename _TRandom>
class PartiallyMappedCrossover : public CoupleCoupleCrossover<_TReal, std::vector<size_t> >, public otl::utility::WithRandom<_TRandom>, public otl::utility::WithProbability<_TReal>
{
public:
	typedef _TReal TReal;
	typedef _TRandom TRandom;
	typedef std::vector<size_t> TDecision;
	typedef CoupleCoupleCrossover<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;

	PartiallyMappedCrossover(TRandom random, const TReal probability);
	~PartiallyMappedCrossover(void);
	bool ShouldCrossover(void);

protected:
	void _DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2);
	void _Crossover(const TDecision &parent1, const TDecision &parent2, TDecision &child1, TDecision &child2);
	static void _Crossover(TDecision &child1, TDecision &child2, const size_t begin, const size_t end);

private:
	std::uniform_real_distribution<TReal> dist_;
};

template <typename _TReal, typename _TRandom>
PartiallyMappedCrossover<_TReal, _TRandom>::PartiallyMappedCrossover(TRandom random, const TReal probability)
	: otl::utility::WithRandom<TRandom>(random)
	, otl::utility::WithProbability<TReal>(probability)
	, dist_(0, 1)
{
}

template <typename _TReal, typename _TRandom>
PartiallyMappedCrossover<_TReal, _TRandom>::~PartiallyMappedCrossover(void)
{
}

template <typename _TReal, typename _TRandom>
bool PartiallyMappedCrossover<_TReal, _TRandom>::ShouldCrossover(void)
{
	return dist_(this->GetRandom()) < this->GetProbability();
}

template <typename _TReal, typename _TRandom>
void PartiallyMappedCrossover<_TReal, _TRandom>::_DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2)
{
	_Crossover(parent1.decision_, parent2.decision_, child1.decision_, child2.decision_);
}

template <typename _TReal, typename _TRandom>
void PartiallyMappedCrossover<_TReal, _TRandom>::_Crossover(const TDecision &parent1, const TDecision &parent2, TDecision &child1, TDecision &child2)
{
	child1 = parent1;
	child2 = parent2;
	if (ShouldCrossover())
	{
		std::uniform_int_distribution<size_t> dist(0, child1.size() - 1);
		const size_t position1 = dist(this->GetRandom());
		const size_t position2 = dist(this->GetRandom());
		assert(0 <= position1 && position1 < child1.size());
		assert(0 <= position2 && position2 < child1.size());
		if (position1 > position2)
			_Crossover(child1, child2, position2, position1 + 1);
		else
			_Crossover(child1, child2, position1, position2 + 1);
	}
}

template <typename _TReal, typename _TRandom>
void PartiallyMappedCrossover<_TReal, _TRandom>::_Crossover(TDecision &child1, TDecision &child2, const size_t begin, const size_t end)
{
	assert(begin < end);
	for (size_t i = begin; i < end; ++i)
	{
		typename TDecision::iterator city1 = std::find(child1.begin(), child1.end(), child2[i]);
		typename TDecision::iterator city2 = std::find(child2.begin(), child2.end(), child1[i]);
		std::swap(child1[i], child2[i]);
		std::swap(*city1, *city2);
	}
}
}
}
