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

#include "MOEA-D.h"

namespace otl
{
namespace optimizer
{
namespace moea_d
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class NormMOEA_D : public MOEA_D<_TReal, _TDecision, _TRandom>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef MOEA_D<TReal, TDecision, TRandom> TSuper;
	typedef typename TSuper::TIndividual TIndividual;
	typedef typename TSuper::TSolutionSet TSolutionSet;
	typedef typename TSuper::TProblem TProblem;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TNeighbors TNeighbors;

	NormMOEA_D(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, const std::vector<TPoint> &weightVectors, const size_t nNeighbors);
	~NormMOEA_D(void);
	const std::vector<TPoint> &GetReferenceObjectiveSet(void) const;

protected:
	std::vector<TPoint> referenceObjectiveSet_;

	void _UpdateReference(const TIndividual &individual);
};

template <typename _TReal, typename _TDecision, typename _TRandom>
NormMOEA_D<_TReal, _TDecision, _TRandom>::NormMOEA_D(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, const std::vector<TPoint> &weightVectors, const size_t nNeighbors)
	: TSuper(random, problem, initial, weightVectors, nNeighbors)
{
	TSuper::referencePoint_ = TSuper::solutionSet_[0].objective_;
	referenceObjectiveSet_.resize(TSuper::referencePoint_.size());
	for (size_t i = 0; i < referenceObjectiveSet_.size(); ++i)
		referenceObjectiveSet_[i] = TSuper::solutionSet_[0].objective_;
	for (size_t i = 1; i < TSuper::solutionSet_.size(); ++i)
		_UpdateReference(TSuper::solutionSet_[i]);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
NormMOEA_D<_TReal, _TDecision, _TRandom>::~NormMOEA_D(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const std::vector<typename NormMOEA_D<_TReal, _TDecision, _TRandom>::TPoint> &NormMOEA_D<_TReal, _TDecision, _TRandom>::GetReferenceObjectiveSet(void) const
{
	return referenceObjectiveSet_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void NormMOEA_D<_TReal, _TDecision, _TRandom>::_UpdateReference(const TIndividual &individual)
{
	assert(TSuper::referencePoint_.size() == referenceObjectiveSet_.size());
	assert(TSuper::referencePoint_.size() == individual.objective_.size());
	for (size_t i = 0; i < TSuper::referencePoint_.size(); ++i)
	{
		if (individual.objective_[i] < TSuper::referencePoint_[i])
		{
			TSuper::referencePoint_[i] = individual.objective_[i];
			referenceObjectiveSet_[i] = individual.objective_;
		}
	}
}
}
}
}
