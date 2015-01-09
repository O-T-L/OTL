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
  author = {Kalyanmoy Deb and Mayank Goyal},
  title = {A Combined Genetic Adaptive Search (GeneAS) for Engineering Design},
  journal = {Computer Science and Informatics},
  year = {1996},
  volume = {26},
  pages = {30-45},
  number = {4}
}
*/

#pragma once

#include <cassert>
#include <cmath>
#include <random>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/WithProbability.h>
#include <OTL/Utility/WithBoundary.h>
#include "Mutation.h"

namespace otl
{
namespace mutation
{
template <typename _TReal>
_TReal CalculatePerturbanceFactor(const _TReal distributionIndex, const _TReal perturbanceFactorLower, const _TReal perturbanceFactorUpper, const _TReal random01)
{
	assert(-1 <= perturbanceFactorLower && perturbanceFactorLower <= 0);
	assert(0 <= perturbanceFactorUpper && perturbanceFactorUpper <= 1);
	if (random01 < 0.5)
	{
		const _TReal temp = pow(1 + perturbanceFactorLower, distributionIndex + 1);
		const _TReal perturbanceFactor = pow(2 * random01 + (1 - 2 * random01) * temp, 1 / (distributionIndex + 1)) - 1;
		assert(perturbanceFactor <= 0);
		return perturbanceFactor;
	}
	else
	{
		const _TReal temp = pow(1 - perturbanceFactorUpper, distributionIndex + 1);
		const _TReal perturbanceFactor = 1 - pow(2 * (1 - random01) + (2 * random01 - 1) * temp, 1 / (distributionIndex + 1));
		assert(0 <= perturbanceFactor);
		return perturbanceFactor;
	}
}

template <typename _TReal>
_TReal CalculatePerturbanceFactorProbability(const _TReal distributionIndex, const _TReal perturbanceFactor)
{
	return (distributionIndex + 1) * pow(1 - std::fabs(perturbanceFactor), distributionIndex) / 2;
}

template <typename _TReal>
_TReal CalculateAmplificationLower(const _TReal distributionIndex, const _TReal perturbanceFactor)
{
	assert(distributionIndex >= 0);
	assert(-1 <= perturbanceFactor && perturbanceFactor <= 0);
	return 2 / (1 - pow(1 + perturbanceFactor, distributionIndex + 1));
}

template <typename _TReal>
_TReal CalculateAmplificationUpper(const _TReal distributionIndex, const _TReal perturbanceFactor)
{
	assert(distributionIndex >= 0);
	assert(0 <= perturbanceFactor && perturbanceFactor <= 1);
	return 2 / (1 - pow(1 - perturbanceFactor, distributionIndex + 1));
}

template <typename _TReal, typename _TRandom>
class PolynomialMutation : public Mutation<_TReal, std::vector<_TReal> >, public otl::utility::WithRandom<_TRandom>, public otl::utility::WithProbability<_TReal>, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef _TRandom TRandom;
	typedef std::vector<TReal> TDecision;
	typedef Mutation<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	PolynomialMutation(TRandom random, const TReal probability, const TBoundary &boundary, const TReal distributionIndex);
	~PolynomialMutation(void);
	TReal GetDistributionIndex(void) const;

protected:
	void _DoMutate(TSolution &solution);
	void _Mutate(TDecision &decision);
	TReal _Mutate(const TReal coding, const TRange &range);

private:
	std::uniform_real_distribution<TReal> dist_;
	TReal distributionIndex_;
};

template <typename _TReal, typename _TRandom>
PolynomialMutation<_TReal, _TRandom>::PolynomialMutation(TRandom random, const TReal probability, const TBoundary &boundary, const TReal distributionIndex)
	: otl::utility::WithRandom<TRandom>(random)
	, otl::utility::WithProbability<TReal>(probability)
	, otl::utility::WithBoundary<TReal>(boundary)
	, dist_(0, 1)
{
	assert(distributionIndex >= 0);
	distributionIndex_ = distributionIndex;
}

template <typename _TReal, typename _TRandom>
PolynomialMutation<_TReal, _TRandom>::~PolynomialMutation(void)
{
}

template <typename _TReal, typename _TRandom>
typename PolynomialMutation<_TReal, _TRandom>::TReal PolynomialMutation<_TReal, _TRandom>::GetDistributionIndex(void) const
{
	assert(distributionIndex_ >= 0);
	return distributionIndex_;
}

template <typename _TReal, typename _TRandom>
void PolynomialMutation<_TReal, _TRandom>::_DoMutate(TSolution &solution)
{
	_Mutate(solution.decision_);
}

template <typename _TReal, typename _TRandom>
void PolynomialMutation<_TReal, _TRandom>::_Mutate(TDecision &decision)
{
	assert(!this->GetBoundary().empty());
	assert(decision.size() == this->GetBoundary().size());
	for (size_t i = 0; i < this->GetBoundary().size(); ++i)
	{
		if (dist_(this->GetRandom()) < this->GetProbability())
			decision[i] = _Mutate(decision[i], this->GetBoundary()[i]);
	}
}

template <typename _TReal, typename _TRandom>
_TReal PolynomialMutation<_TReal, _TRandom>::_Mutate(const TReal coding, const TRange &range)
{
	assert(range.first < range.second);
	const TReal maxDistance = range.second - range.first;
	assert(maxDistance > 0);
	const TReal perturbanceFactorLower = (range.first - coding) / maxDistance;
	const TReal perturbanceFactorUpper = (range.second - coding) / maxDistance;
	const TReal pandom01 = dist_(this->GetRandom());
	assert(0 <= pandom01 && pandom01 < 1);
	const TReal perturbanceFactor = CalculatePerturbanceFactor(GetDistributionIndex(), perturbanceFactorLower, perturbanceFactorUpper, pandom01);
	return otl::utility::FixIntoBoundary(coding + perturbanceFactor * maxDistance, range);
}
}
}
