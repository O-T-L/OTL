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
#include "DTLZ3.h"
#include "Shape/Convex.h"

namespace otl
{
namespace problem
{
namespace dtlz
{
template <typename _TReal>
class ConvexDTLZ3 : public DTLZ3<_TReal>
{
public:
	typedef _TReal TReal;
	typedef DTLZ3<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TRange TRange;
	typedef typename TSuper::TBoundary TBoundary;

	ConvexDTLZ3(const size_t nObjectives);
	ConvexDTLZ3(const size_t nObjectives, const size_t distDecisions);
	~ConvexDTLZ3(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
};

template <typename _TReal>
ConvexDTLZ3<_TReal>::ConvexDTLZ3(const size_t nObjectives)
	: TSuper(nObjectives)
{
}

template <typename _TReal>
ConvexDTLZ3<_TReal>::ConvexDTLZ3(const size_t nObjectives, const size_t distDecisions)
	: TSuper(nObjectives, distDecisions)
{
}

template <typename _TReal>
ConvexDTLZ3<_TReal>::~ConvexDTLZ3(void)
{
}

template <typename _TReal>
size_t ConvexDTLZ3<_TReal>::_DoEvaluate(TSolution &solution)
{
	TSuper::_DoEvaluate(solution);
	shape::ConvertConvex(solution.objective_);
	return 1;
}
}
}
}
