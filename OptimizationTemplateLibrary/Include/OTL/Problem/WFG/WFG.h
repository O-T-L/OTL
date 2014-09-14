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
@ARTICLE{,
  author = {Simon Huband and Phil Hingston and Luigi Barone and Lyndon While},
  title = {A Review of Multiobjective Test Problems and a Scalable Test Problem
	Toolkit},
  journal = {IEEE Transactions on Evolutionary Computation},
  year = {2006},
  volume = {10},
  pages = {477-506},
  number = {5},
  doi = {10.1109/TEVC.2005.861417}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <utility>
#include <OTL/Problem/Problem.h>
#include <OTL/Utility/WithSpaceBoundary.h>

namespace otl
{
namespace problem
{
namespace wfg
{
template <typename _TReal>
class WFG : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithSpaceBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TMinMax TMinMax;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TBoundary TBoundary;

	WFG(const size_t nObjectives, const TBoundary &boundary);
	~WFG(void);

protected:
	void _DoFix(std::vector<TReal> &objective);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
WFG<_TReal>::WFG(const size_t nObjectives, const TBoundary &boundary)
	: TSuper(nObjectives)
	, otl::utility::WithSpaceBoundary<TReal>(boundary)
{
}

template <typename _TReal>
WFG<_TReal>::~WFG(void)
{
}

template <typename _TReal>
void WFG<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
template<class _TArchive> void WFG<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithSpaceBoundary<TReal> >(*this);
}
}
}
}
