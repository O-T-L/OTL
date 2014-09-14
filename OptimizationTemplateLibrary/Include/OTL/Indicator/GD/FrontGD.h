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

#include <vector>
#include <OTL/Indicator/GD/GenerationalDistance.h>
#include <OTL/Indicator/InvertedGenerationalDistance.h>
#include "Utility.h"

#undef max

namespace otl
{
namespace indicator
{
namespace gd
{
template <typename _TReal>
class FrontGD : public GenerationalDistance<_TReal>
{
public:
	typedef _TReal TReal;
	typedef GenerationalDistance<TReal> TSuper;
	typedef typename TSuper::TMetric TMetric;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;

	FrontGD(const TFront &front);
	~FrontGD(void);
	const TFront &GetFront(void) const;

protected:
	TReal _DoDistance2TruePF(const TPoint &objective);

private:
	const TFront front_;
};

template <typename _TReal>
FrontGD<_TReal>::FrontGD(const TFront &front) : front_(front)
{
}

template <typename _TReal>
FrontGD<_TReal>::~FrontGD(void)
{
}

template <typename _TReal>
const typename FrontGD<_TReal>::TFront &FrontGD<_TReal>::GetFront(void) const
{
	return front_;
}

template <typename _TReal>
_TReal FrontGD<_TReal>::_DoDistance2TruePF(const TPoint &objective)
{
	return Distance2Population(objective, front_.begin(), front_.end());
}
}
}
}
