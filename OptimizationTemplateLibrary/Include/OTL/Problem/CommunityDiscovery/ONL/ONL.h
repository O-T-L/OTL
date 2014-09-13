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

#include <limits>
#include <OTL/Utility/WithRandom.h>
#include "OrderedNeighborList.h"

namespace otl
{
namespace problem
{
namespace community_discovery
{
namespace onl
{
template <typename _TReal, typename _TRandom>
class ONL : public OrderedNeighborList<_TReal>, public otl::utility::WithRandom<_TRandom>
{
public:
	typedef _TReal TReal;
	typedef _TRandom TRandom;
	typedef OrderedNeighborList<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TMatrix TMatrix;
	typedef typename TSuper::TFunction TFunction;

	ONL(const TMatrix &graph, const std::vector<TFunction> &functions, const std::vector<bool> &maximize, TRandom random);
	~ONL(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
};

template <typename _TReal, typename _TRandom>
ONL<_TReal, _TRandom>::ONL(const TMatrix &graph, const std::vector<TFunction> &functions, const std::vector<bool> &maximize, TRandom random)
	: TSuper(graph, functions, maximize)
	, otl::utility::WithRandom<TRandom>(random)
{
}

template <typename _TReal, typename _TRandom>
ONL<_TReal, _TRandom>::~ONL(void)
{
}

template <typename _TReal, typename _TRandom>
size_t ONL<_TReal, _TRandom>::_DoEvaluate(TSolution &solution)
{
	MakeLegal(TSuper::GetList(), solution.decision_, this->GetRandom());
	return TSuper::_DoEvaluate(solution);
}
}
}
}
}
