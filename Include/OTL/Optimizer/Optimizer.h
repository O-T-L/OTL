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

#include <string>
#include <algorithm>
#include <cctype>
#include <cassert>
#include <OTL/Problem/Problem.h>

namespace otl
{
namespace optimizer
{
template <typename _TReal, typename _TDecision>
class Optimizer
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef otl::problem::Problem<TReal, TDecision> TProblem;

	Optimizer(TProblem &problem);
	virtual ~Optimizer(void);
	TProblem &GetProblem(void) const;
	void operator ()(void);

protected:
	virtual void _DoStep(void) = 0;

private:
	TProblem &problem_;
};

template <typename _TReal, typename _TDecision>
Optimizer<_TReal, _TDecision>::Optimizer(TProblem &problem) : problem_(problem)
{
}

template <typename _TReal, typename _TDecision>
Optimizer<_TReal, _TDecision>::~Optimizer(void)
{
}

template <typename _TReal, typename _TDecision>
typename Optimizer<_TReal, _TDecision>::TProblem &Optimizer<_TReal, _TDecision>::GetProblem(void) const
{
	return problem_;
}

template <typename _TReal, typename _TDecision>
void Optimizer<_TReal, _TDecision>::operator ()(void)
{
	_DoStep();
}
}
}
