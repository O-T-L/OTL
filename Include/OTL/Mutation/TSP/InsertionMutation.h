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

#include <algorithm>
#include <limits>
#include <random>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/WithProbability.h>
#include <OTL/Mutation/Mutation.h>

namespace otl
{
namespace mutation
{
template <typename _TReal, typename _TRandom>
class InsertionMutation : public Mutation<_TReal, std::vector<size_t> >, public otl::utility::WithRandom<_TRandom>, public otl::utility::WithProbability<_TReal>
{
public:
	typedef _TReal TReal;
	typedef _TRandom TRandom;
	typedef std::vector<size_t> TDecision;
	typedef Mutation<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;

	InsertionMutation(TRandom random, const TReal probability);
	~InsertionMutation(void);

protected:
	void _DoMutate(TSolution &solution);
	void _Mutate(TDecision &decision);
	void _Mutate(TDecision &decision, const size_t from, const size_t to);

private:
	std::uniform_real_distribution<TReal> dist_;
};

template <typename _TReal, typename _TRandom>
InsertionMutation<_TReal, _TRandom>::InsertionMutation(TRandom random, const TReal probability)
	: otl::utility::WithRandom<TRandom>(random)
	, otl::utility::WithProbability<TReal>(probability)
	, dist_(0, 1)
{
}

template <typename _TReal, typename _TRandom>
InsertionMutation<_TReal, _TRandom>::~InsertionMutation(void)
{
}

template <typename _TReal, typename _TRandom>
void InsertionMutation<_TReal, _TRandom>::_DoMutate(TSolution &solution)
{
	_Mutate(solution.decision_);
}

template <typename _TReal, typename _TRandom>
void InsertionMutation<_TReal, _TRandom>::_Mutate(TDecision &decision)
{
	std::uniform_int_distribution<size_t> dist(0, decision.size() - 1);
	if (dist_(this->GetRandom()) < this->GetProbability())
		_Mutate(decision, dist(this->GetRandom()), dist(this->GetRandom()));
}

template <typename _TReal, typename _TRandom>
void InsertionMutation<_TReal, _TRandom>::_Mutate(TDecision &decision, const size_t from, const size_t to)
{
	assert(0 <= from && from < decision.size());
	assert(0 <= to && to < decision.size());
	const size_t city = decision[from];
	decision.erase(decision.begin() + from);
	decision.insert(decision.begin() + to, city);
}
}
}
