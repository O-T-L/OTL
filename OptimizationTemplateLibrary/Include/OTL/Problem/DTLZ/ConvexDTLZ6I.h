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
#include "DTLZ6I.h"
#include "Shape/Convex.h"

namespace otl
{
namespace problem
{
namespace dtlz
{
template <typename _TReal>
class ConvexDTLZ6I : public DTLZ6I<_TReal>
{
public:
	typedef _TReal TReal;
	typedef DTLZ6I<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TMinMax TMinMax;
	typedef typename TSuper::TBoundary TBoundary;

	ConvexDTLZ6I(const size_t nObjectives, const size_t manifold);
	ConvexDTLZ6I(const size_t nObjectives, const size_t manifold, const size_t distDecisions);
	~ConvexDTLZ6I(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
};

template <typename _TReal>
ConvexDTLZ6I<_TReal>::ConvexDTLZ6I(const size_t nObjectives, const size_t manifold)
	: TSuper(nObjectives, manifold)
{
}

template <typename _TReal>
ConvexDTLZ6I<_TReal>::ConvexDTLZ6I(const size_t nObjectives, const size_t manifold, const size_t distDecisions)
	: TSuper(nObjectives, manifold, distDecisions)
{
}

template <typename _TReal>
ConvexDTLZ6I<_TReal>::~ConvexDTLZ6I(void)
{
}

template <typename _TReal>
size_t ConvexDTLZ6I<_TReal>::_DoEvaluate(TSolution &solution)
{
	TSuper::_DoEvaluate(solution);
	shape::ConvertConvex(solution.objective_);
	return 1;
}
}
}
}
