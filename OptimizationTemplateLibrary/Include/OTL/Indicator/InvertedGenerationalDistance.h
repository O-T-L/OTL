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
@ARTICLE{,
  author = {Peter A.N. Bosman and Dirk Thierens},
  title = {The Balance Between Proximity and Diversity in Multi-objective Evolutionary
	Algorithms},
  journal = {IEEE Transactions on Evolutionary Computation},
  year = {2003},
  volume = {7},
  pages = {174-188},
  number = {2},
  month = {April},
  doi = {10.1109/TEVC.2003.810761}
}
*/

#pragma once

#include <vector>
#include <cmath>
#include <limits>
#include <numeric>
#include <OTL/Indicator/Indicator.h>
#include <OTL/Indicator/GD/Utility.h>

namespace otl
{
namespace indicator
{
template <typename _TReal>
class InvertedGenerationalDistance : public Indicator<_TReal, _TReal>
{
public:
	typedef _TReal TReal;
	typedef TReal TMetric;
	typedef Indicator<TReal, TMetric> TSuper;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;

	InvertedGenerationalDistance(const TFront &front);
	virtual ~InvertedGenerationalDistance(void);
	const TFront &GetFront(void) const;

protected:
	TMetric _DoEvaluate(const TFront &front);

private:
	const TFront &front_;
};

template <typename _TReal>
InvertedGenerationalDistance<_TReal>::InvertedGenerationalDistance(const TFront &front) : front_(front)
{
}

template <typename _TReal>
InvertedGenerationalDistance<_TReal>::~InvertedGenerationalDistance(void)
{
}

template <typename _TReal>
const typename InvertedGenerationalDistance<_TReal>::TFront &InvertedGenerationalDistance<_TReal>::GetFront(void) const
{
	return front_;
}

template <typename _TReal>
typename InvertedGenerationalDistance<_TReal>::TMetric InvertedGenerationalDistance<_TReal>::_DoEvaluate(const TFront &front)
{
	assert(!front_.empty());
	TReal sum = 0;
	for (size_t i = 0; i < front_.size(); ++i)
		sum += otl::indicator::gd::Distance2Population(front_[i], front.begin(), front.end());
	return sum / front_.size();
}
}
}
