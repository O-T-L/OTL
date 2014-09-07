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

#include "NSGA-II.h"
#include "ConstrainedDominate.h"

namespace otl
{
namespace optimizer
{
namespace nsga_ii
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class ConstrainedNSGA_II : public NSGA_II<_TReal, _TDecision, _TRandom>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef NSGA_II<TReal, TDecision, TRandom> TSuper;
	typedef typename TSuper::TIndividual TIndividual;
	typedef typename TSuper::TSolutionSet TSolutionSet;
	typedef typename TSuper::TProblem TProblem;
	typedef typename TSuper::TCrossover TCrossover;
	typedef typename TSuper::TMutation TMutation;

	ConstrainedNSGA_II(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation);
	~ConstrainedNSGA_II(void);
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	void _DoStep(void);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);
};

template <typename _TReal, typename _TDecision, typename _TRandom>
ConstrainedNSGA_II<_TReal, _TDecision, _TRandom>::ConstrainedNSGA_II(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation)
	: TSuper(random, problem, initial, crossover, mutation)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
ConstrainedNSGA_II<_TReal, _TDecision, _TRandom>::~ConstrainedNSGA_II(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename ConstrainedNSGA_II<_TReal, _TDecision, _TRandom>::TSolutionSet ConstrainedNSGA_II<_TReal, _TDecision, _TRandom>::MakeOffspring(const TSolutionSet &ancestor)
{
	TSolutionSet offspring = otl::optimizer::nsga_ii::MakeOffspring(ancestor.size(), ancestor.begin(), ancestor.end(), this->GetRandom(), &_Compete, this->GetCrossover());
	for (size_t i = 0; i < offspring.size(); ++i)
	{
		TIndividual &child = offspring[i];
		this->GetMutation()(child);
		TSuper::GetProblem()(child);
	}
	return offspring;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool ConstrainedNSGA_II<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return ConstrainedDominate(individual1, individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool ConstrainedNSGA_II<_TReal, _TDecision, _TRandom>::_Dominate(const TIndividual *individual1, const TIndividual *individual2)
{
	return Dominate(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void ConstrainedNSGA_II<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	TSolutionSet offspring = MakeOffspring(ancestor);
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	typedef typename TSolutionSet::iterator _TIterator;
	otl::selection::NondominateSelection(mix, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), &_Dominate, &TSuper::template _SelectNoncritical<_TPointer, _TIterator>, &TSuper::template _SelectCritical<_TPointer, _TIterator>);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename ConstrainedNSGA_II<_TReal, _TDecision, _TRandom>::TIndividual *ConstrainedNSGA_II<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition)
{
	if (Dominate(*competition[0], *competition[1]))
		return competition[0];
	else if (Dominate(*competition[1], *competition[0]))
		return competition[1];
	assert(competition[0]->crowdingDistance_ >= 0);
	assert(competition[1]->crowdingDistance_ >= 0);
	return competition[0]->crowdingDistance_ > competition[1]->crowdingDistance_ ? competition[0] : competition[1];
}
}
}
}
