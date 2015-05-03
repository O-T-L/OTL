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
#include <OTL/Utility/WithProbability.h>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Initial/DynamicBitset.h>
#include "CoupleCoupleCrossover.h"

#undef min
#undef max

namespace otl
{
namespace crossover
{
template <typename _TReal, typename _TRandom, typename _TDecision = boost::dynamic_bitset<> >
class DynamicBitsetUniformCrossover : public CoupleCoupleCrossover<_TReal, _TDecision>, public otl::utility::WithProbability<_TReal>, public otl::utility::WithRandom<_TRandom>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef CoupleCoupleCrossover<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;

	DynamicBitsetUniformCrossover(TRandom random, const TReal probability);
	~DynamicBitsetUniformCrossover(void);

protected:
	void _DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2);
	void _Crossover(const TDecision &parent1, const TDecision &parent2, TDecision &child1, TDecision &child2);
};

template <typename _TReal, typename _TRandom, typename _TDecision>
DynamicBitsetUniformCrossover<_TReal, _TRandom, _TDecision>::DynamicBitsetUniformCrossover(TRandom random, const TReal probability)
	: otl::utility::WithProbability<TReal>(probability)
	, otl::utility::WithRandom<TRandom>(random)
{
}

template <typename _TReal, typename _TRandom, typename _TDecision>
DynamicBitsetUniformCrossover<_TReal, _TRandom, _TDecision>::~DynamicBitsetUniformCrossover(void)
{
}

template <typename _TReal, typename _TRandom, typename _TDecision>
void DynamicBitsetUniformCrossover<_TReal, _TRandom, _TDecision>::_DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2)
{
	_Crossover(parent1.decision_, parent2.decision_, child1.decision_, child2.decision_);
}

template <typename _TReal, typename _TRandom, typename _TDecision>
void DynamicBitsetUniformCrossover<_TReal, _TRandom, _TDecision>::_Crossover(const TDecision &parent1, const TDecision &parent2, TDecision &child1, TDecision &child2)
{
	assert(parent1.size() == parent2.size());
	child1.resize(parent1.size());
	child2.resize(parent2.size());
	TDecision mask = otl::initial::UniformDynamicBitset(this->GetRandom(), parent1.size());
	TDecision inverseMask = ~mask;
	child1 = (parent1 & mask) | (parent2 & inverseMask);
	child2 = (parent1 & inverseMask) | (parent2 & mask);
}
}
}
