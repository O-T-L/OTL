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
@CONFERENCE{,
  author = {Kalyanmoy Deb and Lothar Thiele and Marco Laumanns and Eckart Zitzler},
  title = {Scalable Test Problems for Evolutionary Multiobjective Optimization},
  booktitle = {Evolutionary Multiobjective Optimization: Theoretical Advances and Applications},
  year = {2005},
  pages = {105-145},
  doi = {10.1007/1-84628-137-7_6}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <utility>
#include <OTL/Problem/Problem.h>
#include <OTL/Utility/WithBoundary.h>

namespace otl
{
namespace problem
{
namespace dtlz
{
template <typename _TReal>
class DTLZ : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	DTLZ(const size_t nObjectives, const TBoundary &boundary);
	~DTLZ(void);

protected:
	void _DoFix(std::vector<TReal> &objective);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
DTLZ<_TReal>::DTLZ(const size_t nObjectives, const TBoundary &boundary)
	: TSuper(nObjectives)
	, otl::utility::WithBoundary<TReal>(boundary)
{
}

template <typename _TReal>
DTLZ<_TReal>::~DTLZ(void)
{
}

template <typename _TReal>
void DTLZ<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
template<class _TArchive> void DTLZ<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithBoundary<TReal> >(*this);
}
}
}
}
