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
#include "FDA.h"
#include "Distance/Distance2.h"

namespace otl
{
namespace problem
{
namespace fda
{
template <typename _TReal>
class StdFDA : public FDA<_TReal>
{
public:
	typedef _TReal TReal;
	typedef FDA<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TRange TRange;
	typedef typename TSuper::TBoundary TBoundary;

	StdFDA(const size_t nObjectives, const size_t &progress, const size_t nFixedSteps, const size_t nDistinctSteps, const size_t distDecisions);
	~StdFDA(void);
};

template <typename _TReal>
StdFDA<_TReal>::StdFDA(const size_t nObjectives, const size_t &progress, const size_t nFixedSteps, const size_t nDistinctSteps, const size_t distDecisions)
	: TSuper(nObjectives, progress, nFixedSteps, nDistinctSteps, TBoundary(nObjectives - 1 + distDecisions, std::make_pair<TReal, TReal>(0, 1)))
{
}

template <typename _TReal>
StdFDA<_TReal>::~StdFDA(void)
{
}
}
}
}
