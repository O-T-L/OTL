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
#include <cmath>
#include <random>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/WithProbability.h>
#include "Mutation.h"

namespace otl
{
namespace mutation
{
template <typename _TReal, typename _TInteger, typename _TRandom>
class BitwiseMutation : public Mutation<_TReal, std::vector<_TInteger> >, public otl::utility::WithRandom<_TRandom>, public otl::utility::WithProbability<_TReal>
{
public:
	typedef _TReal TReal;
	typedef _TInteger TInteger;
	typedef _TRandom TRandom;
	typedef std::vector<_TInteger> TDecision;
	typedef Mutation<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;

	BitwiseMutation(TRandom random, const TReal probability, const std::vector<size_t> &decisionBits);
	~BitwiseMutation(void);
	const std::vector<size_t> &GetDecisionBits(void) const;

protected:
	void _DoMutate(TSolution &solution);
	void _Mutate(TDecision &decision);
	TInteger _Mutate(TInteger coding, const size_t nBits);

private:
	std::uniform_real_distribution<TReal> dist_;
	std::vector<size_t> decisionBits_;
};

template <typename _TReal, typename _TInteger, typename _TRandom>
BitwiseMutation<_TReal, _TInteger, _TRandom>::BitwiseMutation(TRandom random, const TReal probability, const std::vector<size_t> &decisionBits)
	: otl::utility::WithRandom<TRandom>(random)
	, otl::utility::WithProbability<TReal>(probability)
	, dist_(0, 1)
	, decisionBits_(decisionBits)
{
}

template <typename _TReal, typename _TInteger, typename _TRandom>
BitwiseMutation<_TReal, _TInteger, _TRandom>::~BitwiseMutation(void)
{
}

template <typename _TReal, typename _TInteger, typename _TRandom>
const std::vector<size_t> &BitwiseMutation<_TReal, _TInteger, _TRandom>::GetDecisionBits(void) const
{
	return decisionBits_;
}

template <typename _TReal, typename _TInteger, typename _TRandom>
void BitwiseMutation<_TReal, _TInteger, _TRandom>::_DoMutate(TSolution &solution)
{
	_Mutate(solution.decision_);
}

template <typename _TReal, typename _TInteger, typename _TRandom>
void BitwiseMutation<_TReal, _TInteger, _TRandom>::_Mutate(TDecision &decision)
{
	assert(!decisionBits_.empty());
	assert(decision.size() == decisionBits_.size());
	for (size_t i = 0; i < decisionBits_.size(); ++i)
		decision[i] = _Mutate(decision[i], decisionBits_[i]);
}

template <typename _TReal, typename _TInteger, typename _TRandom>
_TInteger BitwiseMutation<_TReal, _TInteger, _TRandom>::_Mutate(TInteger coding, const size_t nBits)
{
	TInteger mask = 1;
	for (size_t i = 0; i < nBits; ++i)
	{
		if (dist_(this->GetRandom()) < this->GetProbability())
			coding ^= mask;
		mask <<= 1;
	}
	return coding;
}
}
}
