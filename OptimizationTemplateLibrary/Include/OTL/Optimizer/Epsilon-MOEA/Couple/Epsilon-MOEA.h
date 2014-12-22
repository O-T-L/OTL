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

#include <OTL/Crossover/WithCoupleCrossover.h>
#include <OTL/Mutation/WithMutation.h>
#include "../Epsilon-MOEA.h"

namespace otl
{
namespace optimizer
{
namespace epsilon_moea
{
namespace couple
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class Epsilon_MOEA : public otl::optimizer::epsilon_moea::Epsilon_MOEA<_TReal, _TDecision, _TRandom>, public otl::crossover::WithCoupleCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef otl::optimizer::epsilon_moea::Epsilon_MOEA<TReal, TDecision, TRandom> TSuper;
	typedef typename TSuper::TIndividual TIndividual;
	typedef typename TSuper::TSolutionSet TSolutionSet;
	typedef typename TSuper::TPopulation TPopulation;
	typedef typename TSuper::TProblem TProblem;
	typedef typename otl::crossover::WithCoupleCrossover<TReal, TDecision>::TCrossover TCrossover;
	typedef typename otl::mutation::WithMutation<TReal, TDecision>::TMutation TMutation;

	Epsilon_MOEA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<TReal> &objectiveLower, const std::vector<TReal> &epsilon);
	~Epsilon_MOEA(void);

protected:
	void _DoStep(void);
	void _ProduceOffspring(const TIndividual &parent1, const TIndividual &parent2, TIndividual &child);
};

template <typename _TReal, typename _TDecision, typename _TRandom>
Epsilon_MOEA<_TReal, _TDecision, _TRandom>::Epsilon_MOEA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<TReal> &objectiveLower, const std::vector<TReal> &epsilon)
	: TSuper(random, problem, initial, objectiveLower, epsilon)
	, otl::crossover::WithCoupleCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
Epsilon_MOEA<_TReal, _TDecision, _TRandom>::~Epsilon_MOEA(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void Epsilon_MOEA<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	const TIndividual &elite = TSuper::_ArchiveSelection(TSuper::solutionSet_);
	const TIndividual &individual = TSuper::_PopulationSelection(TSuper::population_);
	TIndividual child;
	_ProduceOffspring(elite, individual, child);
	TSuper::_ArchiveAcceptance(child, TSuper::solutionSet_);
	TSuper::_PopulationAcceptance(child, TSuper::population_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void Epsilon_MOEA<_TReal, _TDecision, _TRandom>::_ProduceOffspring(const TIndividual &parent1, const TIndividual &parent2, TIndividual &child)
{
	this->GetCrossover()(parent1, parent2, child);
	this->GetMutation()(child);
	TSuper::GetProblem()(child);
	TSuper::Update(child);
}
}
}
}
}
