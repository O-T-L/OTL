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
@TECHREPORT{,
  author = {Qingfu Zhang and Aimin Zhou and Shizheng Zhao and Ponnuthurai Nagaratnam
	Suganthan and Wudong Liu and Santosh Tiwari},
  title = {Multiobjective optimization Test Instances for the CEC 2009 Special
	Session and Competition},
  institution = {School of Computer Science and Electrical Engineering, University
	of Essex},
  year = {2008},
  number = {CES-887}
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
namespace uf
{
template <typename _TReal>
class UF : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithSpaceBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TMinMax TMinMax;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TBoundary TBoundary;

	UF(const size_t nObjectives, const TBoundary &boundary);
	~UF(void);

protected:
	void _DoFix(std::vector<TReal> &objective);
};

template <typename _TReal>
UF<_TReal>::UF(const size_t nObjectives, const TBoundary &boundary)
	: TSuper(nObjectives)
	, otl::utility::WithSpaceBoundary<TReal>(boundary)
{
}

template <typename _TReal>
UF<_TReal>::~UF(void)
{
}

template <typename _TReal>
void UF<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}
}
}
}
