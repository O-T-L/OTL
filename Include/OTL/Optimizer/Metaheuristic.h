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

#include "Optimizer.h"

namespace otl
{
namespace optimizer
{
template <typename _TSolutionSet>
class Metaheuristic : public Optimizer<typename _TSolutionSet::value_type::TReal, typename _TSolutionSet::value_type::TDecision>
{
public:
	typedef _TSolutionSet TSolutionSet;
	typedef typename TSolutionSet::value_type TSolution;
	typedef typename TSolution::TReal TReal;
	typedef typename TSolution::TDecision TDecision;
	typedef Optimizer<TReal, TDecision> TSuper;
	typedef typename TSuper::TProblem TProblem;

	Metaheuristic(TProblem &problem);
	virtual ~Metaheuristic(void);
	const TSolutionSet &GetSolutionSet(void) const;

protected:
	TSolutionSet solutionSet_;
};

template <typename _TSolutionSet>
Metaheuristic<_TSolutionSet>::Metaheuristic(TProblem &problem) : TSuper(problem)
{
}

template <typename _TSolutionSet>
Metaheuristic<_TSolutionSet>::~Metaheuristic(void)
{
}

template <typename _TSolutionSet>
const _TSolutionSet &Metaheuristic<_TSolutionSet>::GetSolutionSet(void) const
{
	return solutionSet_;
}
}
}
