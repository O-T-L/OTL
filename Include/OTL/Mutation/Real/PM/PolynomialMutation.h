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
#include <OTL/Utility/Fix/Truncate.h>
#include <OTL/Mutation/Mutation.h>
#include "Utility.h"

namespace otl
{
namespace mutation
{
namespace real
{
namespace pm
{
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
		const TRange &range = this->GetBoundary()[i];
		if (dist_(this->GetRandom()) < this->GetProbability())
			decision[i] = BoundedMutate(this->GetRandom(), GetDistributionIndex(), decision[i], range.first, range.second);
	}
}
}
}
}
}
