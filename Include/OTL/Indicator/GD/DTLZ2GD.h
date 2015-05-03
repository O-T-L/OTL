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

#include <numeric>
#include <cmath>
#include <cassert>
#include <OTL/Indicator/GD/GenerationalDistance.h>

namespace otl
{
namespace indicator
{
namespace gd
{
template <typename _TReal>
class DTLZ2GD : public GenerationalDistance<_TReal>
{
public:
	typedef _TReal TReal;
	typedef GenerationalDistance<TReal> TSuper;
	typedef typename TSuper::TMetric TMetric;
	typedef typename TSuper::TPoint TPoint;

	DTLZ2GD(void);
	~DTLZ2GD(void);

protected:
	TReal _DoDistance2TruePF(const TPoint &objective);
};

template <typename _TReal>
DTLZ2GD<_TReal>::DTLZ2GD(void)
{
}

template <typename _TReal>
DTLZ2GD<_TReal>::~DTLZ2GD(void)
{
}

template <typename _TReal>
_TReal DTLZ2GD<_TReal>::_DoDistance2TruePF(const TPoint &objective)
{
	return sqrt(std::inner_product(objective.begin(), objective.end(), objective.begin(), (TReal)0)) - 1;
}
}
}
}
