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

#include "HypE.h"

namespace otl
{
namespace optimizer
{
namespace hype
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class FastHypE : public HypE<_TReal, _TDecision, _TRandom>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef HypE<TReal, TDecision, TRandom> TSuper;
	typedef typename TSuper::TIndividual TIndividual;
	typedef typename TSuper::TSolutionSet TSolutionSet;
	typedef typename TSuper::TProblem TProblem;
	typedef typename TSuper::TCrossover TCrossover;
	typedef typename TSuper::TMutation TMutation;

	FastHypE(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const size_t nSample, const TReal expand = 1);
	~FastHypE(void);

protected:
	void _DoStep(void);
	template <typename _TPointer, typename _TIterator> _TIterator _SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
};

template <typename _TReal, typename _TDecision, typename _TRandom>
FastHypE<_TReal, _TDecision, _TRandom>::FastHypE(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const size_t nSample, const TReal expand)
	: TSuper(random, problem, initial, crossover, mutation, nSample, expand)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
FastHypE<_TReal, _TDecision, _TRandom>::~FastHypE(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void FastHypE<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	TSolutionSet offspring = TSuper::MakeOffspring(ancestor);
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	typedef typename TSolutionSet::iterator _TIterator;
	otl::selection::NondominateSelection(mix, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), &TSuper::_Dominate
		, [this](std::list<_TPointer> &front, _TIterator begin, _TIterator end)->_TIterator{return this->_SelectNoncritical(front, begin, end);}
		, [this](std::list<_TPointer> &front, _TIterator begin, _TIterator end)->_TIterator{return this->_SelectCritical(front, begin, end);}
	);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator FastHypE<_TReal, _TDecision, _TRandom>::_SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	const auto lower = FindLower<TReal>(front.begin(), front.end());
	const auto upper = FindUpper<TReal>(front.begin(), front.end());
	const auto referencePoint = CalculateReferencePoint(lower, upper, TSuper::GetExpand());
	assert(front.size() >= std::distance(begin, end));
	for (size_t remove = front.size() - std::distance(begin, end); remove; --remove)
	{
		FastFitnessEstimation(this->GetRandom(), front.begin(), front.end(), lower, referencePoint, TSuper::GetSampleSize(), remove);
		auto worst = std::min_element(front.begin(), front.end(), [](_TPointer individual1, _TPointer individual2)->bool{return individual1->fitness_ < individual2->fitness_;});
		front.erase(worst);
	}
	_TIterator dest = begin;
	for (auto i = front.begin(); i != front.end(); ++i, ++dest)
		*dest = **i;
	return dest;
}
}
}
}
