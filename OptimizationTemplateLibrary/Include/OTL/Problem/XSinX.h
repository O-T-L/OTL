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

/*
@Book{,
  Title                    = {遗传算法: 理论应用与软件实现},
  Author                   = {王小平 and 曹立明},
  Publisher                = {西安交通大学出版社},
  Year                     = {2002}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <boost/math/constants/constants.hpp>
#include <OTL/Problem/Problem.h>
#include <OTL/Utility/WithBoundary.h>

namespace otl
{
namespace problem
{
template <typename _TReal>
class XSinX : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	XSinX(void);
	~XSinX(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
XSinX<_TReal>::XSinX(void)
	: TSuper(1)
	, otl::utility::WithBoundary<TReal>(TBoundary(1, std::make_pair<TReal, TReal>(-1, 2)))
{
}

template <typename _TReal>
XSinX<_TReal>::~XSinX(void)
{
}

template <typename _TReal>
size_t XSinX<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void XSinX<_TReal>::_DoFix(std::vector<TReal> &objective)
{
	for (size_t i = 0; i < objective.size(); ++i)
		objective[i] = -objective[i];
}

template <typename _TReal>
void XSinX<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	assert(this->IsInside(decision));
	objective.resize(TSuper::GetNumberOfObjectives());
	assert(decision.size() == 1);
	objective[0] = -(decision[0] * sin(10 * boost::math::constants::pi<_TReal>() * decision[0]) + 2);
}

template <typename _TReal>
template<class _TArchive> void XSinX<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithBoundary<TReal> >(*this);
}
}
}
