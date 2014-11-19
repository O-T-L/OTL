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

#include <OTL/Indicator/Indicator.h>
#include <OTL/Utility/WithSpaceBoundary.h>
#include "Utility.h"

namespace otl
{
namespace indicator
{
namespace ms
{
/*!
 * \brief Calculate $MS1=\frac{\sqrt{\Sigma_{\imath=1}^{m}\left(f_{\imath}^{\max}-f_{\imath}^{\min}\right)^{2}}}{\sqrt{\Sigma_{\imath=1}^{m}\left(F_{\imath}^{\max}-F_{\imath}^{\min}\right)^{2}}}$
 * where $f_{\imath}^{\min}$ and $f_{\imath}^{\max}$ are the minimum and maximum of the ith objective in the evolved population,
 * respectively, $F_{\imath}^{\min}$ and $F_{\imath}^{\max}$ are the minimum and maximum of the ith objective in the true Pareto front.
 * \param[in] _TReal The type of objective, must be a real number type
 */
template <typename _TReal>
class MaximumSpread1 : public Indicator<_TReal, _TReal>, public otl::utility::WithSpaceBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef TReal TMetric;
	typedef Indicator<TReal, TMetric> TSuper;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TMinMax TMinMax;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TBoundary TBoundary;

	MaximumSpread1(const TBoundary &boundary);
	~MaximumSpread1(void);

protected:
	TMetric _DoEvaluate(const TFront &front);
};

template <typename _TReal>
MaximumSpread1<_TReal>::MaximumSpread1(const TBoundary &boundary)
	: otl::utility::WithSpaceBoundary<TReal>(boundary)
{
}

template <typename _TReal>
MaximumSpread1<_TReal>::~MaximumSpread1(void)
{
}

template <typename _TReal>
typename MaximumSpread1<_TReal>::TMetric MaximumSpread1<_TReal>::_DoEvaluate(const TFront &front)
{
	return CalcMaximumSpread(CalcSpaceBoundary<TReal>(front.begin(), front.end())) / CalcMaximumSpread(this->GetBoundary());
}
}
}
}
