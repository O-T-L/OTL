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
#include <cmath>
#include <numeric>
#include <random>
#include <boost/integer/integer_mask.hpp>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/WithProbability.h>
#include <OTL/Crossover/CoupleCoupleCrossover.h>

namespace otl
{
namespace crossover
{
namespace integer
{
template <typename _TReal, typename _TInteger, typename _TRandom>
class SinglePointCrossover : public CoupleCoupleCrossover<_TReal, std::vector<_TInteger> >, public otl::utility::WithRandom<_TRandom>, public otl::utility::WithProbability<_TReal>
{
public:
	typedef _TReal TReal;
	typedef _TInteger TInteger;
	typedef _TRandom TRandom;
	typedef std::vector<_TInteger> TDecision;
	typedef CoupleCoupleCrossover<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;

	SinglePointCrossover(TRandom random, const TReal probability, const std::vector<size_t> &decisionBits);
	~SinglePointCrossover(void);
	const std::vector<size_t> &GetDecisionBits(void) const;

protected:
	void _DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2);
	void _Crossover(const TDecision &parent1, const TDecision &parent2, TDecision &child1, TDecision &child2);
	static void _Crossover(const TInteger parent1, const TInteger parent2, TInteger &child1, TInteger &child2, const size_t position);

private:
	std::uniform_real_distribution<TReal> dist_;
	std::vector<size_t> decisionBits_;
};

template <typename _TReal, typename _TInteger, typename _TRandom>
SinglePointCrossover<_TReal, _TInteger, _TRandom>::SinglePointCrossover(TRandom random, const TReal probability, const std::vector<size_t> &decisionBits)
	: otl::utility::WithRandom<TRandom>(random)
	, otl::utility::WithProbability<TReal>(probability)
	, dist_(0, 1)
	, decisionBits_(decisionBits)
{
}

template <typename _TReal, typename _TInteger, typename _TRandom>
SinglePointCrossover<_TReal, _TInteger, _TRandom>::~SinglePointCrossover(void)
{
}

template <typename _TReal, typename _TInteger, typename _TRandom>
const std::vector<size_t> &SinglePointCrossover<_TReal, _TInteger, _TRandom>::GetDecisionBits(void) const
{
	return decisionBits_;
}

template <typename _TReal, typename _TInteger, typename _TRandom>
void SinglePointCrossover<_TReal, _TInteger, _TRandom>::_DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2)
{
	_Crossover(parent1.decision_, parent2.decision_, child1.decision_, child2.decision_);
}

template <typename _TReal, typename _TInteger, typename _TRandom>
void SinglePointCrossover<_TReal, _TInteger, _TRandom>::_Crossover(const TDecision &parent1, const TDecision &parent2, TDecision &child1, TDecision &child2)
{
	assert(!decisionBits_.empty());
	assert(parent1.size() == decisionBits_.size());
	assert(parent2.size() == decisionBits_.size());
	if (dist_(this->GetRandom()) < this->GetProbability())
	{
		child1.resize(parent1.size());
		child2.resize(parent2.size());
		for (size_t i = 0; i < decisionBits_.size(); ++i)
		{
			std::uniform_int_distribution<size_t> dist(0, decisionBits_[i]);
			const size_t position = dist(this->GetRandom());
			_Crossover(parent1[i], parent2[i], child1[i], child2[i], position);
		}
	}
	else
	{
		child1 = parent1;
		child2 = parent2;
	}
}

template <typename _TReal, typename _TInteger, typename _TRandom>
void SinglePointCrossover<_TReal, _TInteger, _TRandom>::_Crossover(const TInteger parent1, const TInteger parent2, TInteger &child1, TInteger &child2, const size_t position)
{
	const TInteger upperMask = -1 << position;
	const TInteger lowerMask = ~upperMask;
	child1 = (parent1 & upperMask) | (parent2 & lowerMask);
	child2 = (parent1 & lowerMask) | (parent2 & upperMask);
}
}
}
}
