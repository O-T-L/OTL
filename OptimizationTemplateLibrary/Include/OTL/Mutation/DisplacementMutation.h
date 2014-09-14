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

#include <random>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/WithProbability.h>
#include "Mutation.h"

namespace otl
{
namespace mutation
{
template <typename _TReal, typename _TRandom>
class DisplacementMutation : public Mutation<_TReal, std::vector<size_t> >, public otl::utility::WithRandom<_TRandom>, public otl::utility::WithProbability<_TReal>
{
public:
	typedef _TReal TReal;
	typedef _TRandom TRandom;
	typedef std::vector<size_t> TDecision;
	typedef Mutation<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;

	DisplacementMutation(TRandom random, const TReal probability);
	~DisplacementMutation(void);

protected:
	void _DoMutate(TSolution &solution);
	void _Mutate(TDecision &decision);
	void _Mutate(TDecision &decision, const size_t begin, const size_t end);
	template <typename _TIterator> void _Mutate(TDecision &decision, _TIterator begin, _TIterator end);

private:
	std::uniform_real_distribution<TReal> dist_;
};

template <typename _TReal, typename _TRandom>
DisplacementMutation<_TReal, _TRandom>::DisplacementMutation(TRandom random, const TReal probability)
	: otl::utility::WithRandom<TRandom>(random)
	, otl::utility::WithProbability<TReal>(probability)
	, dist_(0, 1)
{
}

template <typename _TReal, typename _TRandom>
DisplacementMutation<_TReal, _TRandom>::~DisplacementMutation(void)
{
}

template <typename _TReal, typename _TRandom>
void DisplacementMutation<_TReal, _TRandom>::_DoMutate(TSolution &solution)
{
	_Mutate(solution.decision_);
}

template <typename _TReal, typename _TRandom>
void DisplacementMutation<_TReal, _TRandom>::_Mutate(TDecision &decision)
{
	if (dist_(this->GetRandom()) < this->GetProbability())
	{
		std::uniform_int_distribution<size_t> dist(0, decision.size() - 1);
		const size_t position1 = dist(this->GetRandom());
		const size_t position2 = dist(this->GetRandom());
		if (position1 > position2)
			_Mutate(decision, position2, position1 + 1);
		else
			_Mutate(decision, position1, position2 + 1);
	}
}

template <typename _TReal, typename _TRandom>
void DisplacementMutation<_TReal, _TRandom>::_Mutate(TDecision &decision, const size_t begin, const size_t end)
{
	assert(begin < end);
	if (end - begin >= decision.size())
		return;
	_Mutate(decision, decision.begin() + begin, decision.begin() + end);
}

template <typename _TReal, typename _TRandom>
template <typename _TIterator> void DisplacementMutation<_TReal, _TRandom>::_Mutate(TDecision &decision, _TIterator begin, _TIterator end)
{
	TDecision section(begin, end);
	decision.erase(begin, end);
	assert(!decision.empty());
	decision.insert(decision.begin() + std::uniform_int_distribution<size_t>(0, decision.size() - 1)(this->GetRandom()), section.begin(), section.end());
}
}
}
