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

/*
@Article{,
  Title                    = {Multiobjective Evolutionary Algorithms: A Comparative Case Study and the Strength {Pareto} Approach},
  Author                   = {Eckart Zitzler and Lothar Thiele},
  Journal                  = {IEEE Transactions on Evolutionary Computation},
  Year                     = {1999},
  Number                   = {4},
  Pages                    = {257-271},
  Volume                   = {3},
  Doi                      = {10.1109/4235.797969}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <algorithm>
#include <limits>
#include <OTL/Utility/Nondominate.h>
#include "Hypervolume.h"
#include "Pareto.h"

namespace otl
{
namespace indicator
{
namespace hypervolume
{
template <typename _TReal>
class RecursiveHV : public Hypervolume<_TReal>
{
public:
	typedef _TReal TReal;
	typedef Hypervolume<TReal> TSuper;
	typedef typename TSuper::TMetric TMetric;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;

	RecursiveHV(const TPoint &referencePoint);
	~RecursiveHV(void);

protected:
	TMetric _DoEvaluate(const TFront &front);
};

template <typename _TReal>
RecursiveHV<_TReal>::RecursiveHV(const TPoint &referencePoint)
	: TSuper(referencePoint)
{
}

template <typename _TReal>
RecursiveHV<_TReal>::~RecursiveHV(void)
{
}

template <typename _TReal>
typename RecursiveHV<_TReal>::TMetric RecursiveHV<_TReal>::_DoEvaluate(const TFront &front)
{
}
}
}
}
