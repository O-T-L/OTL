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

#include <cmath>
#include <boost/math/constants/constants.hpp>
#include <OTL/Problem/Problem.h>
#include <OTL/Utility/WithBoundary.h>

namespace otl
{
namespace problem
{
template <typename _TReal>
class OKA2 : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	OKA2(void);
	~OKA2(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);
	static TBoundary _GetDecisionSpaceBoundary(void);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
OKA2<_TReal>::OKA2(void)
	: TSuper(2)
	, otl::utility::WithBoundary<TReal>(_GetDecisionSpaceBoundary())
{
}

template <typename _TReal>
OKA2<_TReal>::~OKA2(void)
{
}

template <typename _TReal>
size_t OKA2<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void OKA2<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
void OKA2<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	objective.resize(TSuper::GetNumberOfObjectives());
	assert(decision.size() == 3);
	objective[0] = decision[0];
	objective[1] = 1 - pow((decision[0] + boost::math::constants::pi<TReal>()), 2) / (4 * pow(boost::math::constants::pi<TReal>(), 2)) + pow(std::abs(decision[1] - 5 * cos(decision[0])), (TReal)1 / 3) + pow(std::abs(decision[2] - 5 * sin(decision[0])), (TReal)1 / 3);
}

template <typename _TReal>
typename OKA2<_TReal>::TBoundary OKA2<_TReal>::_GetDecisionSpaceBoundary(void)
{
	TBoundary boundary(3);
	boundary[0].first = -boost::math::constants::pi<TReal>();
	boundary[0].second = boost::math::constants::pi<TReal>();
	for (size_t i = 1; i < boundary.size(); ++i)
	{
		boundary[i].first = -5;
		boundary[i].second = 5;
	}
	return boundary;
}

template <typename _TReal>
template<class _TArchive> void OKA2<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithBoundary<TReal> >(*this);
}
}
}
