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
#include <OTL/Solution.h>

namespace otl
{
namespace crossover
{
template <typename _TReal, typename _TDecision>
class Crossover
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef Solution<TReal, TDecision> TSolution;

	Crossover(void);
	virtual ~Crossover(void);
	void operator ()(std::vector<const TSolution *> &ancestor, std::vector<TSolution *> &offspring);

protected:
	virtual void _DoCrossover(std::vector<const TSolution *> &ancestor, std::vector<TSolution *> &offspring) = 0;
};

template <typename _TReal, typename _TDecision>
Crossover<_TReal, _TDecision>::Crossover(void)
{
}

template <typename _TReal, typename _TDecision>
Crossover<_TReal, _TDecision>::~Crossover(void)
{
}

template <typename _TReal, typename _TDecision>
void Crossover<_TReal, _TDecision>::operator ()(std::vector<const TSolution *> &ancestor, std::vector<TSolution *> &offspring)
{
	_DoCrossover(ancestor, offspring);
}
}
}
