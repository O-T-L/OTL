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
  Title                    = {An Investigation on Noisy Environments in Evolutionary Multiobjective Optimization},
  Author                   = {C. K. Goh and Kay Chen Tan},
  Journal                  = {IEEE Transactions on Evolutionary Computation},
  Year                     = {2007},
  Month                    = {June},
  Number                   = {3},
  Pages                    = {354-381},
  Volume                   = {11},
  Doi                      = {10.1109/TEVC.2006.882428}
}
*/

#pragma once

#include "Utility.h"

namespace otl
{
namespace indicator
{
namespace ms
{
/*!
 * \brief Calculate $MS2=\sqrt{\frac{1}{m}\Sigma_{\imath=1}^{m}\left(\frac{\max\left(f_{\imath}^{\min},F_{\imath}^{\min}\right)-\min\left(f_{\imath}^{\max},F_{\imath}^{\max}\right)}{F_{\imath}^{\max}-F_{\imath}^{\min}}\right)^{2}}$
 * where $f_{\imath}^{\min}$ and $f_{\imath}^{\max}$ are the minimum and maximum of the ith objective in the evolved population,
 * respectively, $F_{\imath}^{\min}$ and $F_{\imath}^{\max}$ are the minimum and maximum of the ith objective in the true Pareto front.
 * \param[in] _TReal The type of objective value, must be a real number type
 */
template <typename _TReal>
class MaximumSpread2 : public Indicator<_TReal, _TReal>, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef TReal TMetric;
	typedef Indicator<TReal, TMetric> TSuper;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	MaximumSpread2(const TBoundary &boundary);
	~MaximumSpread2(void);

protected:
	TMetric _DoEvaluate(const TFront &front);
};

template <typename _TReal>
MaximumSpread2<_TReal>::MaximumSpread2(const TBoundary &boundary)
	: otl::utility::WithBoundary<TReal>(boundary)
{
}

template <typename _TReal>
MaximumSpread2<_TReal>::~MaximumSpread2(void)
{
}

template <typename _TReal>
typename MaximumSpread2<_TReal>::TMetric MaximumSpread2<_TReal>::_DoEvaluate(const TFront &front)
{
	const auto spaceBoundary = CalcSpaceBoundary<TReal>(front.begin(), front.end());
	const auto &spaceBoundaryOfPF = this->GetBoundary();
	assert(spaceBoundary.size() == spaceBoundaryOfPF.size());
	_TReal sum = 0;
	for (size_t i = 0; i < spaceBoundary.size(); ++i)
	{
		const _TReal max = std::max(spaceBoundary[i].first, spaceBoundaryOfPF[i].first);
		const _TReal min = std::min(spaceBoundary[i].second, spaceBoundaryOfPF[i].second);
		const _TReal temp = (max - min) / (spaceBoundaryOfPF[i].second - spaceBoundaryOfPF[i].first);
		sum += temp * temp;
	}
	return sqrt(sum / spaceBoundary.size());
}
}
}
}
