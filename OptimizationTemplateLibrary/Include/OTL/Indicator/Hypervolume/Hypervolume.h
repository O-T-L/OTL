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
#include <OTL/Indicator/Indicator.h>

namespace otl
{
namespace indicator
{
namespace hypervolume
{
template <typename _TReal>
class Hypervolume : public Indicator<_TReal, _TReal>
{
public:
	typedef _TReal TReal;
	typedef TReal TMetric;
	typedef Indicator<TReal, TMetric> TSuper;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;

	Hypervolume(const TPoint &referencePoint);
	~Hypervolume(void);
	const TPoint &GetReferencePoint(void) const;
	bool IsValid(const TPoint &point) const;
	bool IsValid(const TFront &front) const;

private:
	TPoint referencePoint_;
};

template <typename _TReal>
Hypervolume<_TReal>::Hypervolume(const TPoint &referencePoint)
	: referencePoint_(referencePoint)
{
}

template <typename _TReal>
Hypervolume<_TReal>::~Hypervolume(void)
{
}

template <typename _TReal>
const typename Hypervolume<_TReal>::TPoint &Hypervolume<_TReal>::GetReferencePoint(void) const
{
	return referencePoint_;
}

template <typename _TReal>
bool Hypervolume<_TReal>::IsValid(const TPoint &point) const
{
	if (point.size() != GetReferencePoint().size())
		return false;
	for (size_t i = 0; i < GetReferencePoint().size(); ++i)
	{
		if (point[i] > GetReferencePoint()[i])
			return false;
	}
	return true;
}

template <typename _TReal>
bool Hypervolume<_TReal>::IsValid(const TFront &front) const
{
	for (size_t i = 0; i < front.size(); ++i)
	{
		if (!IsValid(*front[i]))
			return false;
	}
	return true;
}
}
}
}
