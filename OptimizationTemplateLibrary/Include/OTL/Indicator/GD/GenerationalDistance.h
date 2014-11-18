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
@CONFERENCE{,
  author = {David A. Van Veldhuizen and Gary B. Lamont},
  title = {Evolutionary Computation and Convergence to a Pareto Front},
  booktitle = {Late Breaking Papers at the Genetic Programming 1998 Conference},
  year = {1998},
  pages = {221-228},
  address = {University of Wisconsin, Madison, Wisconsin, USA},
  month = {October},
  publisher = {Stanford University Bookstore}
}
*/

#pragma once

#include <OTL/Indicator/Indicator.h>

namespace otl
{
namespace indicator
{
namespace gd
{
template <typename _TReal>
class GenerationalDistance : public Indicator<_TReal, _TReal>
{
public:
	typedef _TReal TReal;
	typedef TReal TMetric;
	typedef Indicator<TReal, TMetric> TSuper;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;

	GenerationalDistance(void);
	virtual ~GenerationalDistance(void);
	TReal Distance2TruePF(const TPoint &objective);

protected:
	TMetric _DoEvaluate(const TFront &front);
	virtual TReal _DoDistance2TruePF(const TPoint &objective) = 0;
};

template <typename _TReal>
GenerationalDistance<_TReal>::GenerationalDistance(void)
{
}

template <typename _TReal>
GenerationalDistance<_TReal>::~GenerationalDistance(void)
{
}

template <typename _TReal>
_TReal GenerationalDistance<_TReal>::Distance2TruePF(const TPoint &objective)
{
	return _DoDistance2TruePF(objective);
}

template <typename _TReal>
typename GenerationalDistance<_TReal>::TMetric GenerationalDistance<_TReal>::_DoEvaluate(const TFront &front)
{
	TReal sum = 0;
	for (size_t i = 0; i < front.size(); ++i)
	{
		const TReal distance = Distance2TruePF(*front[i]);
		sum += distance * distance;
	}
	return sqrt(sum) / front.size();
}
}
}
}
