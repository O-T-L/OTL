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
@PHDTHESIS{,
  author = {Eckart Zitzler},
  title = {Evolutionary Algorithms for Multiobjective Optimization: Methods
	and Applications},
  school = {Eidgenössische Technische Hochschule Zürich. Swiss Federal Institute
	of Technology},
  year = {1999},
  type = {Ph.D. Thesis}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <utility>
#include <limits>
#include <cmath>
#include <OTL/Indicator/Indicator.h>
#include "Utility.h"

namespace otl
{
namespace indicator
{
namespace ms
{
/*!
 * \brief Calculate $MS=\sqrt{\Sigma_{\imath=1}^{m}\left(f_{\imath}^{\max}-f_{\imath}^{\min}\right)^{2}}$
 * where $f_{\imath}^{\min}$ and $f_{\imath}^{\max}$ are the minimum and maximum of the ith objective in the evolved population.
 * \param[in] _TReal The type of objective value, must be a real number type
 */
template <typename _TReal>
class MaximumSpread : public Indicator<_TReal, _TReal>
{
public:
	typedef _TReal TReal;
	typedef TReal TMetric;
	typedef Indicator<TReal, TMetric> TSuper;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;

	MaximumSpread(void);
	~MaximumSpread(void);

protected:
	TMetric _DoEvaluate(const TFront &front);
};

template <typename _TReal>
MaximumSpread<_TReal>::MaximumSpread(void)
{
}

template <typename _TReal>
MaximumSpread<_TReal>::~MaximumSpread(void)
{
}

template <typename _TReal>
typename MaximumSpread<_TReal>::TMetric MaximumSpread<_TReal>::_DoEvaluate(const TFront &front)
{
	return CalcMaximumSpread(CalcSpaceBoundary<TReal>(front.begin(), front.end()));
}
}
}
}
