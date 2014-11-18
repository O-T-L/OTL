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
@TECHREPORT{,
  author = {Michael Pilegaard Hansen and Andrzej Jaszkiewicz},
  title = {Evaluating the quality of approximations to the non-dominated set},
  institution = {Technical University of Denmark},
  year = {1998},
  number = {IMM-REP-1998-7},
  month = {March}
}
*/

#pragma once

#include <vector>
#include <cmath>
#include <limits>
#include <numeric>
#include <OTL/Indicator/Indicator.h>
#include <OTL/Utility/Aggregation/Tchebycheff.h>

namespace otl
{
namespace indicator
{
template <typename _TReal>
class R2 : public Indicator<_TReal, _TReal>
{
public:
	typedef _TReal TReal;
	typedef TReal TMetric;
	typedef Indicator<TReal, TMetric> TSuper;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;

	R2(const TPoint &referencePoint, const std::vector<TPoint> &weightVectors);
	virtual ~R2(void);
	const TPoint &GetReferencePoint(void) const;
	const std::vector<TPoint> &GetWeightVectors(void) const;
	TPoint ComputeDirection(const TPoint &point) const;

protected:
	TMetric _DoEvaluate(const TFront &front);
	TMetric MinTchebycheff(const TFront &front, const TPoint &weight) const;

private:
	TPoint referencePoint_;
	std::vector<TPoint> weightVectors_;
};

template <typename _TReal>
R2<_TReal>::R2(const TPoint &referencePoint, const std::vector<TPoint> &weightVectors)
	: referencePoint_(referencePoint)
	, weightVectors_(weightVectors)
{
}

template <typename _TReal>
R2<_TReal>::~R2(void)
{
}

template <typename _TReal>
const typename R2<_TReal>::TPoint &R2<_TReal>::GetReferencePoint(void) const
{
	return referencePoint_;
}

template <typename _TReal>
const std::vector<typename R2<_TReal>::TPoint> &R2<_TReal>::GetWeightVectors(void) const
{
	return weightVectors_;
}

template <typename _TReal>
typename R2<_TReal>::TPoint R2<_TReal>::ComputeDirection(const TPoint &point) const
{
	assert(point.size() == referencePoint_.size());
	TPoint direction(point.size());
	for (size_t i = 0; i < direction.size(); ++i)
	{
		direction[i] = point[i] - referencePoint_[i];
		assert(direction[i] >= 0);
	}
	return direction;
}

template <typename _TReal>
typename R2<_TReal>::TMetric R2<_TReal>::_DoEvaluate(const TFront &front)
{
	assert(!weightVectors_.empty());
	TMetric sum = 0;
	for (size_t i = 0; i < weightVectors_.size(); ++i)
		sum += MinTchebycheff(front, weightVectors_[i]);
	return sum / weightVectors_.size();
}

template <typename _TReal>
typename R2<_TReal>::TMetric R2<_TReal>::MinTchebycheff(const TFront &front, const TPoint &weight) const
{
	assert(!front.empty());
	TMetric minTchebycheff = otl::utility::aggregation::Tchebycheff(weight, ComputeDirection(*front[0]));
	for (size_t i = 1; i < front.size(); ++i)
	{
		const TMetric tchebycheff = otl::utility::aggregation::Tchebycheff(weight, ComputeDirection(*front[i]));
		if (tchebycheff < minTchebycheff)
			minTchebycheff = tchebycheff;
	}
	return minTchebycheff;
}
}
}
