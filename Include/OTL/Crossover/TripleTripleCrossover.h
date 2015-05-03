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
class TripleTripleCrossover
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef Solution<TReal, TDecision> TSolution;

	TripleTripleCrossover(void);
	virtual ~TripleTripleCrossover(void);
	void operator ()(const TSolution &parent1, const TSolution &parent2, const TSolution &parent3, TSolution &child1, TSolution &child2, TSolution &child3);

protected:
	virtual void _DoCrossover(const TSolution &parent1, const TSolution &parent2, const TSolution &parent3, TSolution &child1, TSolution &child2, TSolution &child3) = 0;
};

template <typename _TReal, typename _TDecision>
TripleTripleCrossover<_TReal, _TDecision>::TripleTripleCrossover(void)
{
}

template <typename _TReal, typename _TDecision>
TripleTripleCrossover<_TReal, _TDecision>::~TripleTripleCrossover(void)
{
}

template <typename _TReal, typename _TDecision>
void TripleTripleCrossover<_TReal, _TDecision>::operator ()(const TSolution &parent1, const TSolution &parent2, const TSolution &parent3, TSolution &child1, TSolution &child2, TSolution &child3)
{
	_DoCrossover(parent1, parent2, parent3, child1, child2, child3);
}
}
}
