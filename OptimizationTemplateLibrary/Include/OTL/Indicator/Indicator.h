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
namespace indicator
{
template <typename _TReal, typename _TMetric>
class Indicator
{
public:
	typedef _TReal TReal;
	typedef _TMetric TMetric;
	typedef std::vector<TReal> TPoint;
	typedef std::vector<TPoint> TFront;

	Indicator(void);
	virtual ~Indicator(void);
	TMetric operator ()(const TFront &front);

protected:
	virtual TMetric _DoEvaluate(const TFront &front) = 0;
};

template <typename _TReal, typename _TMetric>
Indicator<_TReal, _TMetric>::Indicator(void)
{
}

template <typename _TReal, typename _TMetric>
Indicator<_TReal, _TMetric>::~Indicator(void)
{
}

template <typename _TReal, typename _TMetric>
_TMetric Indicator<_TReal, _TMetric>::operator ()(const TFront &front)
{
	return _DoEvaluate(front);
}
}
}
