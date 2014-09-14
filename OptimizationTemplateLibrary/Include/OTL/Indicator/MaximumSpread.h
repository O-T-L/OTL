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

namespace otl
{
namespace indicator
{
template <typename _TReal, typename _TIterator>
std::vector<std::pair<_TReal, _TReal> > CalcSpaceBoundary(_TIterator begin, _TIterator end)
{
	std::vector<std::pair<_TReal, _TReal> > spaceBoundary(begin->size());
	for (size_t i = 0; i < spaceBoundary.size(); ++i)
	{
		spaceBoundary[i].first = std::numeric_limits<_TReal>::max();
		spaceBoundary[i].second = std::numeric_limits<_TReal>::min();
	}
	for (_TIterator pointIter = begin; pointIter != end; ++pointIter)
	{
		for (size_t i = 0; i < spaceBoundary.size(); ++i)
		{
			auto &point = (*pointIter);
			assert(point.size() == spaceBoundary.size());
			if (point[i] < spaceBoundary[i].first)
				spaceBoundary[i].first = point[i];
			if (point[i] > spaceBoundary[i].second)
				spaceBoundary[i].second = point[i];
			assert(spaceBoundary[i].first <= spaceBoundary[i].second);
		}
	}
	return spaceBoundary;
}

template <typename _TReal>
_TReal CalcMaximumSpread(const std::vector<std::pair<_TReal, _TReal> > &spaceBoundary)
{
	_TReal sum = 0;
	for (size_t i = 0; i < spaceBoundary.size(); ++i)
	{
		const _TReal temp = spaceBoundary[i].second - spaceBoundary[i].first;
		sum += temp * temp;
	}
	return sqrt(sum);
}

/*!
 * \brief Calculate $MS=\sqrt{\Sigma_{\imath=1}^{m}\left(f_{\imath}^{\max}-f_{\imath}^{\min}\right)^{2}}$
 * where $f_{\imath}^{\min}$ and $f_{\imath}^{\max}$ are the minimum and maximum of the ith objective in the evolved population.
 * \param[in] _TReal The type of objective, must be a real number type
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
