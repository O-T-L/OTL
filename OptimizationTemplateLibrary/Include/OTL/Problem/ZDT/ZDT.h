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
  author = {Eckart Zitzler and Kalyanmoy Deb and Lothar Thiele},
  title = {Comparison of Multiobjective Evolutionary Algorithms: Empirical Results},
  journal = {Evolutionary Computation},
  year = {2000},
  volume = {8},
  pages = {173-195},
  number = {2},
  month = {June},
  doi = {10.1162/106365600568202}
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
namespace zdt
{
template <typename _TReal>
class ZDT : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithSpaceBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TMinMax TMinMax;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TBoundary TBoundary;

	ZDT(const TBoundary &boundary);
	~ZDT(void);

protected:
	void _DoFix(std::vector<TReal> &objective);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
ZDT<_TReal>::ZDT(const TBoundary &boundary)
	: TSuper(2)
	, otl::utility::WithSpaceBoundary<TReal>(boundary)
{
}

template <typename _TReal>
ZDT<_TReal>::~ZDT(void)
{
}

template <typename _TReal>
void ZDT<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
template<class _TArchive> void ZDT<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithSpaceBoundary<TReal> >(*this);
}
}
}
}
