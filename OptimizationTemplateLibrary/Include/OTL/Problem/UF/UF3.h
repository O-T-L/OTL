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

#include <cassert>
#include <vector>
#include "UF.h"
#include "Distance/Distance3.h"

namespace otl
{
namespace problem
{
namespace uf
{
template <typename _TReal>
class UF3 : public UF<_TReal>
{
public:
	typedef _TReal TReal;
	typedef UF<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TMinMax TMinMax;
	typedef typename TSuper::TBoundary TBoundary;

	UF3(const size_t nDistDecisions = 29);
	~UF3(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);
};

template <typename _TReal>
UF3<_TReal>::UF3(const size_t nDistDecisions)
	: TSuper(2, TBoundary(1 + nDistDecisions, TMinMax(0, 1)))
{
}

template <typename _TReal>
UF3<_TReal>::~UF3(void)
{
}

template <typename _TReal>
size_t UF3<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void UF3<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	objective.resize(TSuper::GetNumberOfObjectives());
	const TReal x = decision[0];
	objective[0] = x + 2 * distance::Distance3(decision, 2);
	objective[1] = 1 - sqrt(x) + 2 * distance::Distance3(decision, 1);
}
}
}
}
