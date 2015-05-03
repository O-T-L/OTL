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
#include <OTL/Optimizer/Metaheuristic.h>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Crossover/WithCrossover.h>
#include <OTL/Mutation/WithMutation.h>
#include <OTL/Utility/Relation/Pareto.h>
#include <OTL/Utility/Nondominate.h>
#include <OTL/Selection/NondominateSelection.h>
#include <OTL/Optimizer/NSGA-II/Offspring.h>
#include <OTL/Optimizer/NSGA-II/CrowdingDistanceAssignment.h>
#include <OTL/Optimizer/NSGA-II/Individual.h>

namespace otl
{
namespace optimizer
{
namespace r_nsga_ii
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class R_NSGA_II : public Metaheuristic<std::vector<otl::optimizer::nsga_ii::Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef otl::optimizer::nsga_ii::Individual<TReal, TDecision> TIndividual;
	typedef std::vector<TIndividual> TSolutionSet;
	typedef Metaheuristic<TSolutionSet> TSuper;
	typedef typename TSuper::TProblem TProblem;
	typedef typename otl::crossover::WithCrossover<TReal, TDecision>::TCrossover TCrossover;
	typedef typename otl::mutation::WithMutation<TReal, TDecision>::TMutation TMutation;

	R_NSGA_II(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<TReal> &referencePoint, const std::vector<TReal> &weight, const TReal threshold);
	~R_NSGA_II(void);
	const std::vector<TReal> &GetReferencePoint(void) const;
	const std::vector<TReal> &GetWeight(void) const;
	TReal GetThreshold(void) const;
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);
	bool Dominate(const std::vector<TReal> &point1, const std::vector<TReal> &point2) const;
	bool Dominate(const TIndividual &individual1, const TIndividual &individual2) const;
	TReal CalculateDist(const std::vector<TReal> &objective) const;

protected:
	template <typename _TIterator> void _CalculateObjRange(_TIterator begin, _TIterator end);
	template <typename _TIterator> void _CalculateDistRange(_TIterator begin, _TIterator end);
	void _DoStep(void);
	template <typename _TPointer, typename _TIterator> static _TIterator _SelectNoncritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	template <typename _TPointer, typename _TIterator> static _TIterator _SelectCritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	const TIndividual *_Compete(const std::vector<const TIndividual *> &competition) const;

private:
	std::vector<TReal> referencePoint_;
	std::vector<TReal> weight_;
	TReal threshold_;
	std::vector<TReal> objRange_;
	TReal distRange_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
R_NSGA_II<_TReal, _TDecision, _TRandom>::R_NSGA_II(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<TReal> &referencePoint, const std::vector<TReal> &weight, const TReal threshold)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, otl::crossover::WithCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
	, referencePoint_(referencePoint)
	, weight_(weight)
	, threshold_(threshold)
	, objRange_(problem.GetNumberOfObjectives())
{
	TSuper::solutionSet_.resize(initial.size());
	for (size_t i = 0; i < initial.size(); ++i)
	{
		TIndividual &individual = TSuper::solutionSet_[i];
		individual.decision_ = initial[i];
		TSuper::GetProblem()(individual);
	}

	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> solutionSet;
	for (size_t i = 0; i < TSuper::solutionSet_.size(); ++i)
		solutionSet.push_back(&TSuper::solutionSet_[i]);
	_CalculateObjRange(solutionSet.begin(), solutionSet.end());
	_CalculateDistRange(solutionSet.begin(), solutionSet.end());
	while (!solutionSet.empty())
	{
		std::list<_TPointer> nondominate = otl::utility::ExtractNondominate(solutionSet, [this](const TIndividual *individual1, const TIndividual *individual2)->bool{return this->Dominate(*individual1, *individual2);});
		std::vector<_TPointer> _nondominate(nondominate.begin(), nondominate.end());
		otl::optimizer::nsga_ii::CrowdingDistanceAssignment<TReal>(_nondominate.begin(), _nondominate.end());
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
R_NSGA_II<_TReal, _TDecision, _TRandom>::~R_NSGA_II(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const std::vector<_TReal> &R_NSGA_II<_TReal, _TDecision, _TRandom>::GetReferencePoint(void) const
{
	return referencePoint_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const std::vector<_TReal> &R_NSGA_II<_TReal, _TDecision, _TRandom>::GetWeight(void) const
{
	return weight_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal R_NSGA_II<_TReal, _TDecision, _TRandom>::GetThreshold(void) const
{
	return threshold_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename R_NSGA_II<_TReal, _TDecision, _TRandom>::TSolutionSet R_NSGA_II<_TReal, _TDecision, _TRandom>::MakeOffspring(const TSolutionSet &ancestor)
{
	TSolutionSet offspring = otl::optimizer::nsga_ii::MakeOffspring(ancestor.size(), ancestor.begin(), ancestor.end(), this->GetRandom()
		, [this](const std::vector<const TIndividual *> &competition)->const TIndividual *{return this->_Compete(competition);}
		, this->GetCrossover());
	for (size_t i = 0; i < offspring.size(); ++i)
	{
		TIndividual &child = offspring[i];
		this->GetMutation()(child);
		TSuper::GetProblem()(child);
	}
	return offspring;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool R_NSGA_II<_TReal, _TDecision, _TRandom>::Dominate(const std::vector<TReal> &point1, const std::vector<TReal> &point2) const
{
	assert(point1.size() == point2.size());
	assert(point1.size() == referencePoint_.size());
	assert(0 <= threshold_ && threshold_ <= 1);
	assert(distRange_ >= 0);
	if (otl::utility::relation::Dominate(point1, point2))
		return true;
	else if (otl::utility::relation::Dominate(point2, point1))
		return false;
	else
		return (CalculateDist(point1) - CalculateDist(point2)) / distRange_ < -threshold_;

}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool R_NSGA_II<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2) const
{
	return Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal R_NSGA_II<_TReal, _TDecision, _TRandom>::CalculateDist(const std::vector<TReal> &objective) const
{
	TReal dist = 0;
	for (size_t i = 0; i < referencePoint_.size(); ++i)
	{
		const TReal temp = (objective[i] - referencePoint_[i]) / objRange_[i];
		dist += temp * temp * weight_[i];
	}
	return sqrt(dist);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator> void R_NSGA_II<_TReal, _TDecision, _TRandom>::_CalculateObjRange(_TIterator begin, _TIterator end)
{
	for (size_t i = 0; i < objRange_.size(); ++i)
	{
		auto fMinMax = std::minmax_element(begin, end, [i](const TIndividual *individual1, const TIndividual *individual2)->bool{return individual1->objective_[i] < individual2->objective_[i];});
		objRange_[i] = (**fMinMax.second).objective_[i] - (**fMinMax.first).objective_[i];
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator> void R_NSGA_II<_TReal, _TDecision, _TRandom>::_CalculateDistRange(_TIterator begin, _TIterator end)
{
	std::list<TReal> dists;
	for (_TIterator i = begin; i != end; ++i)
		dists.push_back(CalculateDist((**i).objective_));
	auto minMax = std::minmax_element(dists.begin(), dists.end());
	distRange_ = *minMax.second - *minMax.first;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void R_NSGA_II<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	TSolutionSet offspring = MakeOffspring(ancestor);
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	_CalculateObjRange(mix.begin(), mix.end());
	_CalculateDistRange(mix.begin(), mix.end());
	typedef typename TSolutionSet::iterator _TIterator;
	otl::selection::NondominateSelection(mix, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end()
		, [this](const TIndividual *individual1, const TIndividual *individual2)->bool{return this->Dominate(*individual1, *individual2);}
		, &_SelectNoncritical<_TPointer, _TIterator>, &_SelectCritical<_TPointer, _TIterator>);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator R_NSGA_II<_TReal, _TDecision, _TRandom>::_SelectNoncritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	std::vector<_TPointer> _front(front.begin(), front.end());
	otl::optimizer::nsga_ii::CrowdingDistanceAssignment<TReal>(_front.begin(), _front.end());
	assert(_front.size() <= std::distance(begin, end));
	_TIterator dest = begin;
	for (size_t i = 0; i < _front.size(); ++i, ++dest)
		*dest = *_front[i];
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator R_NSGA_II<_TReal, _TDecision, _TRandom>::_SelectCritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	std::vector<_TPointer> _front(front.begin(), front.end());
	otl::optimizer::nsga_ii::CrowdingDistanceAssignment<TReal>(_front.begin(), _front.end());
	std::partial_sort(_front.begin(), _front.begin() + std::distance(begin, end), _front.end()
		, [](_TPointer individual1, _TPointer individual2)->bool{return individual1->crowdingDistance_ > individual2->crowdingDistance_;}
	);
	assert(_front.size() >= std::distance(begin, end));
	_TIterator dest = begin;
	for (size_t i = 0; dest != end; ++i, ++dest)
		*dest = *_front[i];
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename R_NSGA_II<_TReal, _TDecision, _TRandom>::TIndividual *R_NSGA_II<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition) const
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
