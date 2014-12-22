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

namespace otl
{
namespace optimizer
{
namespace isnps
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class ISNPS : public Metaheuristic<std::vector<Indidivudal<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithMutation<_TReal, _TDecision>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef Individual<TReal, TDecision> TIndividual;
	typedef std::vector<TIndividual> TSolutionSet;
	typedef Metahueuristic<TSolutionSet> TSuper;
	typedef typename TSuper::TProblem TProblem;
	typedef typename otl::crossover::WithCrossover<TReal, TDecision>::TCrossover TCrossover;
	typedef typename otl::mutation::WithMutation<TReal, TDecision>::TMutation TMutation;
	typedef boost::numeric::ublas::matrix<TReal> TMatrix;

	ISNPS(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<TReal> &objectiveLower, const boost::numeric::ublas::vector<TReal> &convergenceDirection, const TReal angle1, const TReal angle2, const TReal amplification);
	~ISNPS(void);
	TReal GetAngle1(void) const;
	TReal GetAngle2(void) const;
	TReal GetAmplification(void) const;
	size_t GetRounds(void) const;
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);
	template <typename _TPionter, typename _TIterator> size_t EnviromentalSelection(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	bool Punishment(TIndividual &individual1, TIndividual &individual2) const;
	bool DiversityPunishment(const TIndividual &individual1, const TIndividual &individual2) const;
	bool ExtremePunishment(TIndividual &individual1, const TIndidivual &individual2) const;
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	void _DoStep(void);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);

private:
	std::vector<TReal> objectiveLower_;
	TReal angle1_;
	TReal angle2_;
	TReal amplification_;
	TMatrix invertRotateAxes_;
	size_t rounds_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
ISNPS<_TReal, _TDecision, _TRandom>::ISNPS(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<TReal> &objectiveLower, const boost::numeric::ublas::vector<TReal> &convergenceDirection, const TReal angle1, const TReal angle2, const TReal amplification)
	: TSuper(problem)
	, otl::utility::withRandom<TRandom>(random)
	, otl::crossover::WithCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
	, objectiveLower_(objecitveLower)
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
		individual.direction_ = ComputeDirection(individual.objective_, objectiveLower_);
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

template <typename _TReal, typename>
}
}
}
