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

#include "CoupleCoupleCrossover.h"

namespace otl
{
namespace crossover
{
template <typename _TReal, typename _TDecision>
class WithCoupleCoupleCrossover
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef CoupleCoupleCrossover<TReal, TDecision> TCrossover;

	WithCoupleCoupleCrossover(TCrossover &crossover);
	~WithCoupleCoupleCrossover(void);
	TCrossover &GetCrossover(void) const;

private:
	TCrossover &crossover_;
};

template <typename _TReal, typename _TDecision>
WithCoupleCoupleCrossover<_TReal, _TDecision>::WithCoupleCoupleCrossover(TCrossover &crossover) : crossover_(crossover)
{
}

template <typename _TReal, typename _TDecision>
WithCoupleCoupleCrossover<_TReal, _TDecision>::~WithCoupleCoupleCrossover(void)
{
}

template <typename _TReal, typename _TDecision>
typename WithCoupleCoupleCrossover<_TReal, _TDecision>::TCrossover &WithCoupleCoupleCrossover<_TReal, _TDecision>::GetCrossover(void) const
{
	return crossover_;
}
}
}
