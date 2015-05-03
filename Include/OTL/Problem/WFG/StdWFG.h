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

#include "WFG.h"

namespace otl
{
namespace problem
{
namespace wfg
{
template <typename _TReal>
class StdWFG : public WFG<_TReal>
{
public:
	typedef _TReal TReal;
	typedef WFG<TReal> TSuper;
	typedef typename TSuper::TRange TRange;
	typedef typename TSuper::TBoundary TBoundary;

	StdWFG(const size_t nObjectives, const size_t posGroups, const size_t distDecisions);
	~StdWFG(void);
	size_t GetPosDecisions(void) const;
	static TBoundary GenerateDecisionSpace(const size_t dimension);

private:
	size_t posDecisions_;
};

template <typename _TReal>
StdWFG<_TReal>::StdWFG(const size_t nObjectives, const size_t posGroups, const size_t distDecisions)
	: TSuper(nObjectives, GenerateDecisionSpace(posGroups * (nObjectives - 1) + distDecisions))
	, posDecisions_(posGroups * (nObjectives - 1))
{
}

template <typename _TReal>
StdWFG<_TReal>::~StdWFG(void)
{
}

template <typename _TReal>
size_t StdWFG<_TReal>::GetPosDecisions(void) const
{
	return posDecisions_;
}

template <typename _TReal>
typename StdWFG<_TReal>::TBoundary StdWFG<_TReal>::GenerateDecisionSpace(const size_t dimension)
{
	TBoundary boundary(dimension);
	for (size_t i = 0; i < boundary.size(); ++i)
	{
		boundary[i].first = 0;
		boundary[i].second = 2 * (i + 1);
	}
	return boundary;
}
}
}
}
