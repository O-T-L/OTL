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
  author = {Marco Farina and Kalyanmoy Deb and Paolo Amato},
  title = {Dynamic Multiobjective Optimization Problems: Test Cases, Approximations,
	and Applications},
  journal = {IEEE Transactions on Evolutionary Computation},
  year = {2004},
  volume = {8},
  pages = {425-442},
  number = {5},
  month = {October},
  doi = {10.1109/TEVC.2004.831456}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <utility>
#include <cmath>
#include <OTL/Problem/DynamicProblem.h>
#include <OTL/Utility/WithSpaceBoundary.h>

namespace otl
{
namespace problem
{
namespace fda
{
template <typename _TReal>
class FDA : public DynamicProblem<_TReal, std::vector<_TReal> >, public otl::utility::WithSpaceBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef DynamicProblem<TReal, TDecision> TSuper;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TMinMax TMinMax;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TBoundary TBoundary;

	FDA(const size_t nObjectives, const size_t &progress, const size_t nFixedSteps, const size_t nDistinctSteps, const TBoundary &boundary);
	~FDA(void);
	size_t GetNumberOfFixedSteps(void) const;
	size_t GetNumberOfDistinctSteps(void) const;
	TReal CalculateEnvironment(void) const;

protected:
	void _DoFix(std::vector<TReal> &objective);

private:
	size_t nFixedSteps_;
	size_t nDistinctSteps_;
};

template <typename _TReal>
FDA<_TReal>::FDA(const size_t nObjectives, const size_t &progress, const size_t nFixedSteps, const size_t nDistinctSteps, const TBoundary &boundary)
	: TSuper(nObjectives, progress)
	, otl::utility::WithSpaceBoundary<TReal>(boundary)
	, nFixedSteps_(nFixedSteps)
	, nDistinctSteps_(nDistinctSteps)
{
}

template <typename _TReal>
FDA<_TReal>::~FDA(void)
{
}

template <typename _TReal>
size_t FDA<_TReal>::GetNumberOfFixedSteps(void) const
{
	return nFixedSteps_;
}

template <typename _TReal>
size_t FDA<_TReal>::GetNumberOfDistinctSteps(void) const
{
	return nDistinctSteps_;
}

template <typename _TReal>
_TReal FDA<_TReal>::CalculateEnvironment(void) const
{
	return (TSuper::GetProgress() / nFixedSteps_) / (_TReal)nDistinctSteps_;
}

template <typename _TReal>
void FDA<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}
}
}
}
