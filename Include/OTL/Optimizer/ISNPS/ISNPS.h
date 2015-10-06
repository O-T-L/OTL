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
#include <list>
#include <numeric>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <OTL/Optimizer/Metaheuristic.h>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Crossover/WithCrossover.h>
#include <OTL/Mutation/WithMutation.h>
#include <OTL/Utility/Relation/Pareto.h>
#include <OTL/Optimizer/NSGA-II/Offspring.h>
#include "Individual.h"
#include "InformationSeparation.h"
#include "NeighborPunishmentSelection.h"
#include "Utility.h"

namespace otl
{
namespace optimizer
{
namespace isnps
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class ISNPS : public Metaheuristic<std::vector<Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef Individual<TReal, TDecision> TIndividual;
	typedef std::vector<TIndividual> TSolutionSet;
	typedef Metaheuristic<TSolutionSet> TSuper;
	typedef typename TSuper::TProblem TProblem;
	typedef typename otl::crossover::WithCrossover<TReal, TDecision>::TCrossover TCrossover;
	typedef typename otl::mutation::WithMutation<TReal, TDecision>::TMutation TMutation;
	typedef boost::numeric::ublas::matrix<TReal> TMatrix;

	ISNPS(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const boost::numeric::ublas::vector<TReal> &convergenceDirection, const TReal angle1, const TReal angle2, const TReal amplification);
	~ISNPS(void);
	TReal GetAngle1(void) const;
	TReal GetAngle2(void) const;
	TReal GetAmplification(void) const;
	size_t GetRounds(void) const;
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);
	template <typename _TPointer, typename _TIterator> size_t EnvironmentalSelection(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	bool Punishment(TIndividual &individual1, TIndividual &individual2) const;
	bool DiversityPunishment(const TIndividual &individual1, const TIndividual &individual2) const;
	bool ExtremePunishment(TIndividual &individual1, const TIndividual &individual2) const;
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	void _DoStep(void);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);
	void _UpdateLower(const std::vector<TReal> &objective);

private:
	std::vector<TReal> lower_;
	TReal angle1_;
	TReal angle2_;
	TReal amplification_;
	TMatrix invertRotateAxes_;
	size_t rounds_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
ISNPS<_TReal, _TDecision, _TRandom>::ISNPS(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const boost::numeric::ublas::vector<TReal> &convergenceDirection, const TReal angle1, const TReal angle2, const TReal amplification)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, otl::crossover::WithCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
	, lower_(problem.GetNumberOfObjectives(), std::numeric_limits<TReal>::max())
	, angle1_(angle1)
	, angle2_(angle2)
	, amplification_(amplification)
{
	assert(convergenceDirection.size() == TSuper::GetProblem().GetNumberOfObjectives());
	invertRotateAxes_ = boost::numeric::ublas::trans(RotateAxes(convergenceDirection));
#ifndef NDEBUG
	rounds_ = 0;
#endif
	TSuper::solutionSet_.resize(initial.size());
	for (size_t i = 0; i < initial.size(); ++i)
	{
		TIndividual &individual = TSuper::solutionSet_[i];
		individual.decision_ = initial[i];
		TSuper::GetProblem()(individual);
		_UpdateLower(individual.objective_);
	}
	for (size_t i = 0; i < TSuper::solutionSet_.size(); ++i)
	{
		TIndividual &individual = TSuper::solutionSet_[i];
		individual.direction_ = ComputeDirection(individual.objective_, lower_);
		individual.convergence_ = InformationSeparation(invertRotateAxes_, individual.direction_, individual.distribution_);
		individual.exclude_ = 0;
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
ISNPS<_TReal, _TDecision, _TRandom>::~ISNPS(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal ISNPS<_TReal, _TDecision, _TRandom>::GetAngle1(void) const
{
	return angle1_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal ISNPS<_TReal, _TDecision, _TRandom>::GetAngle2(void) const
{
	return angle2_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal ISNPS<_TReal, _TDecision, _TRandom>::GetAmplification(void) const
{
	return amplification_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
size_t ISNPS<_TReal, _TDecision, _TRandom>::GetRounds(void) const
{
	assert(rounds_ != -1);
	return rounds_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename ISNPS<_TReal, _TDecision, _TRandom>::TSolutionSet ISNPS<_TReal, _TDecision, _TRandom>::MakeOffspring(const TSolutionSet &ancestor)
{
	TSolutionSet offspring = otl::optimizer::nsga_ii::MakeOffspring(ancestor.size(), ancestor.begin(), ancestor.end(), this->GetRandom(), &_Compete, this->GetCrossover());
	for (size_t i = 0; i < offspring.size(); ++i)
	{
		TIndividual &child = offspring[i];
		this->GetMutation()(child);
		TSuper::GetProblem()(child);
		_UpdateLower(child.objective_);
	}
	for (size_t i = 0; i < offspring.size(); ++i)
	{
		TIndividual &child = offspring[i];
		child.direction_ = ComputeDirection(child.objective_, lower_);
		child.convergence_ = InformationSeparation(invertRotateAxes_, child.direction_, child.distribution_);
	}
	return offspring;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> size_t ISNPS<_TReal, _TDecision, _TRandom>::EnvironmentalSelection(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	size_t rounds = 0;
	for (auto dest = begin; dest != end; ++rounds)
		dest = NeighborPunishmentSelection(front, dest, end
			, [](_TPointer individual1, _TPointer individual2)->bool{return individual1->convergence_ < individual2->convergence_;}
			, [this](_TPointer individual1, _TPointer individual2)->bool{return this->Punishment(*individual1, *individual2);}
		);
	return rounds;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool ISNPS<_TReal, _TDecision, _TRandom>::Punishment(TIndividual &individual1, TIndividual &individual2) const
{
	const bool punish = otl::utility::relation::Dominate(individual1.objective_, individual2.objective_) || DiversityPunishment(individual1, individual2) || ExtremePunishment(individual1, individual2);
	if (punish)
		++individual2.exclude_;
	return punish;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool ISNPS<_TReal, _TDecision, _TRandom>::DiversityPunishment(const TIndividual &individual1, const TIndividual &individual2) const
{
	const TReal radius = sqrt(std::inner_product(individual1.distribution_.begin(), individual1.distribution_.end(), individual2.distribution_.begin(), (TReal)0, std::plus<TReal>(), [](TReal x, TReal y)->TReal{TReal t = x - y;return t * t;}));
	return atan(radius / individual2.convergence_ / amplification_) <= angle1_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool ISNPS<_TReal, _TDecision, _TRandom>::ExtremePunishment(TIndividual &individual1, const TIndividual &individual2) const
{
	std::vector<TReal> diff(individual2.objective_.size());
	for (size_t i = 0; i < diff.size(); ++i)
		diff[i] = individual2.objective_[i] - individual1.objective_[i];
	return acos(std::inner_product(individual1.objective_.begin(), individual1.objective_.end(), diff.begin(), (TReal)0) / (sqrt(std::inner_product(individual1.objective_.begin(), individual1.objective_.end(), individual1.objective_.begin(), (TReal)0)) * sqrt(std::inner_product(diff.begin(), diff.end(), diff.begin(), (TReal)0)))) <= angle2_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool ISNPS<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void ISNPS<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	TSolutionSet offspring = MakeOffspring(ancestor);
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	for (auto i = mix.begin(); i != mix.end(); ++i)
		(**i).exclude_ = 0;
	rounds_ = EnvironmentalSelection(mix, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end());
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename ISNPS<_TReal, _TDecision, _TRandom>::TIndividual *ISNPS<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition)
{
	if (Dominate(*competition[0], *competition[1]))
		return competition[0];
	else if (Dominate(*competition[1], *competition[0]))
		return competition[1];
	return competition[0]->exclude_ < competition[1]->exclude_ ? competition[0] : competition[1];
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void ISNPS<_TReal, _TDecision, _TRandom>::_UpdateLower(const std::vector<TReal> &objective)
{
	assert(objective.size() == lower_.size());
	for (size_t i = 0; i < lower_.size(); ++i)
	{
		if (objective[i] < lower_[i])
			lower_[i] = objective[i];
	}
}
}
}
}
