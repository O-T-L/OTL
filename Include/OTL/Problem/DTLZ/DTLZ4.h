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

#include <vector>
#include <cmath>
#include "DTLZ.h"
#include "Distance/Distance2.h"
#include "Transform/Bias.h"
#include "Shape/Utility.h"
#include "Shape/Concave.h"

namespace otl
{
namespace problem
{
namespace dtlz
{
template <typename _TReal>
class DTLZ4 : public DTLZ<_TReal>
{
public:
	typedef _TReal TReal;
	typedef DTLZ<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TRange TRange;
	typedef typename TSuper::TBoundary TBoundary;

	DTLZ4(const size_t nObjectives, const size_t distDecisions = 10, const TReal biasFactor = 100);
	~DTLZ4(void);
	TReal GetBiasFactor(void) const;

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	TReal biasFactor_;

	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
DTLZ4<_TReal>::DTLZ4(const size_t nObjectives, const size_t distDecisions, const TReal biasFactor)
	: TSuper(nObjectives, TBoundary(nObjectives - 1 + distDecisions, std::make_pair<TReal, TReal>(0, 1)))
	, biasFactor_(biasFactor)
{
}

template <typename _TReal>
DTLZ4<_TReal>::~DTLZ4(void)
{
}

template <typename _TReal>
_TReal DTLZ4<_TReal>::GetBiasFactor(void) const
{
	return biasFactor_;
}

template <typename _TReal>
size_t DTLZ4<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void DTLZ4<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	assert(this->IsInside(decision));
	objective.resize(TSuper::GetNumberOfObjectives());
	const size_t posDecisions = TSuper::GetNumberOfObjectives() - 1;
	const TReal radius = 1 + distance::Distance2<TReal>(decision.begin() + posDecisions, decision.end());
	TDecision angles(decision.begin(), decision.begin() + posDecisions);
	transform::Bias(angles.begin(), angles.end(), biasFactor_);
	shape::ConvertAngles<TReal>(angles.begin(), angles.end());
	shape::Concave(angles.begin(), angles.end(), objective.begin(), objective.end(), radius);
}

template <typename _TReal>
template<class _TArchive> void DTLZ4<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & biasFactor_;
}
}
}
}
