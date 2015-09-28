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
@ARTICLE{,
  author = {Kalyanmoy Deb and Ram Bhusan Agrawal},
  title = {Simulated Binary Crossover for Continuous Search Space},
  journal = {Complex Systems},
  year = {1994},
  volume = {9},
  pages = {115-148},
  number = {2}
}
*/

#pragma once

#include <cassert>
#include <cmath>
#include <algorithm>
#include <random>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/WithProbability.h>
#include <OTL/Utility/WithBoundary.h>
#include <OTL/Utility/Fix/Truncate.h>
#include <OTL/Crossover/CoupleCoupleCrossover.h>
#include "Utility.h"

namespace otl
{
namespace crossover
{
namespace real
{
namespace sbx
{
template <typename _TReal, typename _TRandom>
class SimulatedBinaryCrossover : public CoupleCoupleCrossover<_TReal, std::vector<_TReal> >, public otl::utility::WithRandom<_TRandom>, public otl::utility::WithProbability<_TReal>, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef _TRandom TRandom;
	typedef std::vector<TReal> TDecision;
	typedef CoupleCoupleCrossover<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	SimulatedBinaryCrossover(TRandom random, const TReal probability, const TBoundary &boundary, const TReal distributionIndex, const TReal componentProbability = 0.5);
	~SimulatedBinaryCrossover(void);
	TReal GetDistributionIndex(void) const;
	TReal GetComponentProbability(void) const;

protected:
	void _DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2);
	void _Crossover(const TDecision &parent1, const TDecision &parent2, TDecision &child1, TDecision &child2);

private:
	std::uniform_real_distribution<TReal> dist_;
	TReal distributionIndex_;
	TReal componentProbability_;
};

template <typename _TReal, typename _TRandom>
SimulatedBinaryCrossover<_TReal, _TRandom>::SimulatedBinaryCrossover(TRandom random, const TReal probability, const TBoundary &boundary, const TReal distributionIndex, const TReal componentProbability)
	: otl::utility::WithRandom<TRandom>(random)
	, otl::utility::WithProbability<TReal>(probability)
	, otl::utility::WithBoundary<TReal>(boundary)
	, dist_(0, 1)
	, distributionIndex_(distributionIndex)
	, componentProbability_(componentProbability)
{
	assert(distributionIndex >= 0);
	assert(0 <= componentProbability && componentProbability <= 1);
}

template <typename _TReal, typename _TRandom>
SimulatedBinaryCrossover<_TReal, _TRandom>::~SimulatedBinaryCrossover(void)
{
}

template <typename _TReal, typename _TRandom>
typename SimulatedBinaryCrossover<_TReal, _TRandom>::TReal SimulatedBinaryCrossover<_TReal, _TRandom>::GetDistributionIndex(void) const
{
	return distributionIndex_;
}

template <typename _TReal, typename _TRandom>
_TReal SimulatedBinaryCrossover<_TReal, _TRandom>::GetComponentProbability(void) const
{
	return componentProbability_;
}

template <typename _TReal, typename _TRandom>
void SimulatedBinaryCrossover<_TReal, _TRandom>::_DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2)
{
	_Crossover(parent1.decision_, parent2.decision_, child1.decision_, child2.decision_);
}

template <typename _TReal, typename _TRandom>
void SimulatedBinaryCrossover<_TReal, _TRandom>::_Crossover(const TDecision &parent1, const TDecision &parent2, TDecision &child1, TDecision &child2)
{
	assert(!this->GetBoundary().empty());
	assert(parent1.size() == this->GetBoundary().size());
	assert(parent2.size() == this->GetBoundary().size());
	if (dist_(this->GetRandom()) < this->GetProbability())
	{
		child1.resize(parent1.size());
		child2.resize(parent2.size());
		for (size_t i = 0; i < this->GetBoundary().size(); ++i)
		{
			const TRange &range = this->GetBoundary()[i];
			if (dist_(this->GetRandom()) < GetComponentProbability())
				BoundedCrossover(this->GetRandom(), this->GetDistributionIndex(), parent1[i], parent2[i], child1[i], child2[i], range.first, range.second);
			else
			{
				child1[i] = parent1[i];
				child2[i] = parent2[i];
			}
		}
	}
	else
	{
		child1 = parent1;
		child2 = parent2;
	}
}
}
}
}
}
