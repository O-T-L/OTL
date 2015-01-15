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
#include "CoupleCoupleCrossover.h"

#undef min
#undef max

namespace otl
{
namespace crossover
{
template <typename _TReal>
_TReal CalculateSpreadFactorAttenuation(const _TReal distributionIndex, const _TReal spreadFactor)
{
	assert(distributionIndex >= 0);
	assert(spreadFactor >= 0);
	return 2 - pow(spreadFactor, -(distributionIndex + 1));
}

template <typename _TReal>
_TReal CalculateSpreadFactor(const _TReal distributionIndex, const _TReal spreadFactorAttenuation, const _TReal random01)
{
	assert(0 <= random01 && random01 < 1);
	if (random01 < 1. / spreadFactorAttenuation)
		return pow(random01 * spreadFactorAttenuation, 1. / (distributionIndex + 1));
	else
		return pow(1. / (2 - random01 * spreadFactorAttenuation), 1 / (distributionIndex + 1));
}

template <typename _TReal>
_TReal CalculateSpreadFactorProbability(const _TReal distributionIndex, const _TReal spreadFactor)
{
	assert(distributionIndex >= 0);
	assert(spreadFactor >= 0);
	if (spreadFactor < 1)
		return (distributionIndex + 1) * pow(spreadFactor, distributionIndex) / 2;
	else
		return (distributionIndex + 1) / pow(spreadFactor, distributionIndex + 2) / 2;
}

template <typename _TReal>
_TReal CalculateAmplificationFactor(const _TReal distributionIndex, const _TReal spreadFactor)
{
	return 2 / CalculateSpreadFactorAttenuation(distributionIndex, spreadFactor);
}

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
	bool ShouldCrossover(void);
	TReal GetDistributionIndex(void) const;
	TReal GetComponentProbability(void) const;

protected:
	void _DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2);
	void _Crossover(const TDecision &parent1, const TDecision &parent2, TDecision &child1, TDecision &child2);
	void _Crossover(const TReal parent1, const TReal parent2, TReal &child1, TReal &child2, const TRange &range);

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
bool SimulatedBinaryCrossover<_TReal, _TRandom>::ShouldCrossover(void)
{
	return dist_(this->GetRandom()) < this->GetProbability();
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
	if (ShouldCrossover())
	{
		child1.resize(parent1.size());
		child2.resize(parent2.size());
		for (size_t i = 0; i < this->GetBoundary().size(); ++i)
		{
			if (dist_(this->GetRandom()) < GetComponentProbability())
				_Crossover(parent1[i], parent2[i], child1[i], child2[i], this->GetBoundary()[i]);
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

template <typename _TReal, typename _TRandom>
void SimulatedBinaryCrossover<_TReal, _TRandom>::_Crossover(const TReal parent1, const TReal parent2, TReal &child1, TReal &child2, const TRange &range)
{
	assert(range.first < range.second);
	const TReal distance = std::fabs(parent1 - parent2);
	if (distance == 0)
	{
		child1 = parent1;
		child2 = parent2;
		return;
	}
	const TReal spreadFactorLower = 1 + 2 * (std::min(parent1, parent2) - range.first) / distance;
	const TReal spreadFactorUpper = 1 + 2 * (range.second - std::max(parent1, parent2)) / distance;
	assert(spreadFactorLower >= 0);
	assert(spreadFactorUpper >= 0);
	const TReal spreadFactorAttenuationLower = CalculateSpreadFactorAttenuation(GetDistributionIndex(), spreadFactorLower);
	const TReal spreadFactorAttenuationUpper = CalculateSpreadFactorAttenuation(GetDistributionIndex(), spreadFactorUpper);
	const TReal random01 = dist_(this->GetRandom());
	assert(0 <= random01 && random01 < 1);
	const TReal spreadFactor1 = CalculateSpreadFactor(GetDistributionIndex(), spreadFactorAttenuationLower, random01);
	const TReal spreadFactor2 = CalculateSpreadFactor(GetDistributionIndex(), spreadFactorAttenuationUpper, random01);
	const TReal middle = (parent1 + parent2) / 2;
	const TReal halfDistance = distance / 2;
	child1 = otl::utility::fix::Truncate(middle - spreadFactor1 * halfDistance, range);
	child2 = otl::utility::fix::Truncate(middle + spreadFactor2 * halfDistance, range);
	if (dist_(this->GetRandom()) < 0.5)
		std::swap(child1, child2);
}
}
}
