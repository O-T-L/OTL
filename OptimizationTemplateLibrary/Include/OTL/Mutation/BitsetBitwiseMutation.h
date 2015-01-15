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
#include <random>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/WithProbability.h>
#include "Mutation.h"

namespace otl
{
namespace mutation
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class BitsetBitwiseMutation : public Mutation<_TReal, _TDecision>, public otl::utility::WithRandom<_TRandom>, public otl::utility::WithProbability<_TReal>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef Mutation<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;

	BitsetBitwiseMutation(TRandom random, const TReal probability);
	~BitsetBitwiseMutation(void);
	bool ShouldMutate(void);

protected:
	void _DoMutate(TSolution &solution);
	void _Mutate(TDecision &decision);

private:
	std::uniform_real_distribution<TReal> dist_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
BitsetBitwiseMutation<_TReal, _TDecision, _TRandom>::BitsetBitwiseMutation(TRandom random, const TReal probability)
	: otl::utility::WithRandom<TRandom>(random)
	, otl::utility::WithProbability<TReal>(probability)
	, dist_(0, 1)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
BitsetBitwiseMutation<_TReal, _TDecision, _TRandom>::~BitsetBitwiseMutation(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool BitsetBitwiseMutation<_TReal, _TDecision, _TRandom>::ShouldMutate(void)
{
	return dist_(this->GetRandom()) < this->GetProbability();
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void BitsetBitwiseMutation<_TReal, _TDecision, _TRandom>::_DoMutate(TSolution &solution)
{
	_Mutate(solution.decision_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void BitsetBitwiseMutation<_TReal, _TDecision, _TRandom>::_Mutate(TDecision &decision)
{
	for (size_t i = 0; i < decision.size(); ++i)
	{
		if (ShouldMutate())
			decision[i].flip();
	}
}
}
}
