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
#include <random>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/WithProbability.h>
#include <OTL/Crossover/CoupleCoupleCrossover.h>

namespace otl
{
namespace crossover
{
namespace bitset
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class SinglePointCrossover : public CoupleCoupleCrossover<_TReal, _TDecision>, public otl::utility::WithRandom<_TRandom>, public otl::utility::WithProbability<_TReal>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef CoupleCoupleCrossover<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;

	SinglePointCrossover(TRandom random, const TReal probability);
	~SinglePointCrossover(void);

protected:
	void _DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2);
	void _Crossover(const TDecision &parent1, const TDecision &parent2, TDecision &child1, TDecision &child2);
};

template <typename _TReal, typename _TDecision, typename _TRandom>
SinglePointCrossover<_TReal, _TDecision, _TRandom>::SinglePointCrossover(TRandom random, const TReal probability)
	: otl::utility::WithRandom<TRandom>(random)
	, otl::utility::WithProbability<TReal>(probability)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
SinglePointCrossover<_TReal, _TDecision, _TRandom>::~SinglePointCrossover(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void SinglePointCrossover<_TReal, _TDecision, _TRandom>::_DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2)
{
	_Crossover(parent1.decision_, parent2.decision_, child1.decision_, child2.decision_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void SinglePointCrossover<_TReal, _TDecision, _TRandom>::_Crossover(const TDecision &parent1, const TDecision &parent2, TDecision &child1, TDecision &child2)
{
	assert(parent1.size() == parent2.size());
	child1.resize(parent1.size());
	child2.resize(parent2.size());
	std::uniform_int_distribution<size_t> dist(0, parent1.size());
	const size_t end = dist(this->GetRandom());
	assert(0 <= end && end <= parent1.size());
	for (size_t i = 0; i < end; ++i)
	{
		child1[i] = parent1[i];
		child2[i] = parent2[i];
	}
	for (size_t i = end; i < parent1.size(); ++i)
	{
		child1[i] = parent2[i];
		child2[i] = parent1[i];
	}
}
}
}
}
