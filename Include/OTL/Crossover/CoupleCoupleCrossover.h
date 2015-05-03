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
#include <OTL/Solution.h>

namespace otl
{
namespace crossover
{
template <typename _TReal, typename _TDecision>
class CoupleCoupleCrossover
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef Solution<TReal, TDecision> TSolution;

	CoupleCoupleCrossover(void);
	virtual ~CoupleCoupleCrossover(void);
	void operator ()(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2);

protected:
	virtual void _DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2) = 0;
};

template <typename _TReal, typename _TDecision>
CoupleCoupleCrossover<_TReal, _TDecision>::CoupleCoupleCrossover(void)
{
}

template <typename _TReal, typename _TDecision>
CoupleCoupleCrossover<_TReal, _TDecision>::~CoupleCoupleCrossover(void)
{
}

template <typename _TReal, typename _TDecision>
void CoupleCoupleCrossover<_TReal, _TDecision>::operator ()(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2)
{
	_DoCrossover(parent1, parent2, child1, child2);
}
}
}
