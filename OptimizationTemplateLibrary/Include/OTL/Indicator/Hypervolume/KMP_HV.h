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
\author      T.Voss, O.Krause
\date        2014

\par Copyright 1995-2014 Shark Development Team

This file comes from Shark.
<http://image.diku.dk/shark/>

Shark is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Shark is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Shark.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
@Article{,
  Title                    = {Multiobjective Evolutionary Algorithms: A Comparative Case Study and the Strength {Pareto} Approach},
  Author                   = {Eckart Zitzler and Lothar Thiele},
  Journal                  = {IEEE Transactions on Evolutionary Computation},
  Year                     = {1999},
  Number                   = {4},
  Pages                    = {257-271},
  Volume                   = {3},
  Doi                      = {10.1109/4235.797969}
}

@Article{,
  Title                    = {Faster {S}-Metric Calculation by Considering Dominated Hypervolume as {Klee}’s Measure Problem},
  Author                   = {Nicola Beume},
  Journal                  = {Evolutionary Computation},
  Year                     = {2009},
  Month                    = {9 December},
  Number                   = {4},
  Pages                    = {477-492},
  Volume                   = {17},
  Doi                      = {10.1162/evco.2009.17.4.17402}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <algorithm>
#include "Hypervolume.h"

namespace otl
{
namespace indicator
{
namespace hypervolume
{
template <typename _TReal>
class KMP_HV : public Hypervolume<_TReal>
{
public:
	typedef _TReal TReal;
	typedef Hypervolume<TReal> TSuper;
	typedef typename TSuper::TMetric TMetric;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;

	KMP_HV(const TPoint &referencePoint);
	~KMP_HV(void);

protected:
	TMetric _DoEvaluate(const TFront &front);
	TReal _CalculateHV2(const TFront &front) const;
	TReal _CalculateHV(const TFront &front) const;
	static bool _Covers(const TPoint &point, const TPoint &lower);
	static bool _PartCovers(const TPoint &point, const TPoint &upper);
	static int _ContainsBoundary(const TPoint &point, const TPoint &lower, const size_t split);
	static TReal _ComputeVolume(const TPoint &lower, const TPoint &upper);
	static size_t _Pile(const TPoint &point, const TPoint &lower, const TPoint &upper);
	static TReal _ComputeTrellis(const TPoint &lower, const TPoint &upper, const TPoint &trellis);
	static TReal _GetMedian(const TPoint &bounds, const size_t length);
	static TReal _Stream(const TPoint &lower, const TPoint &upper, const TFront &front, size_t split, TReal cover);
};

template <typename _TReal>
KMP_HV<_TReal>::KMP_HV(const TPoint &referencePoint)
	: TSuper(referencePoint)
{
}

template <typename _TReal>
KMP_HV<_TReal>::~KMP_HV(void)
{
}

template <typename _TReal>
typename KMP_HV<_TReal>::TMetric KMP_HV<_TReal>::_DoEvaluate(const TFront &front)
{
	TFront _front( front );
	std::stable_sort( _front.begin(), _front.end(), [](const TPoint *point1, const TPoint *point2)->bool{return point1->back() < point2->back();});
	if (TSuper::GetReferencePoint().size() == 2)
		return _CalculateHV2(_front);
	else
		return _CalculateHV(_front);
}

template <typename _TReal>
_TReal KMP_HV<_TReal>::_CalculateHV2(const TFront &front) const
{
	TReal hv = (TSuper::GetReferencePoint()[0] - (*front[0])[0]) * (TSuper::GetReferencePoint()[1] - (*front[0])[1]);
	for (size_t i = 1, j = 0; i < front.size(); ++i)
	{
		const TReal diff = (*front[j])[0] - (*front[i])[0];
		if (diff > 0)
		{
			hv += diff * (TSuper::GetReferencePoint()[1] - (*front[i])[1]);
			j = i;
		}
	}
	return hv;
}

template <typename _TReal>
_TReal KMP_HV<_TReal>::_CalculateHV(const TFront &front) const
{
	TPoint lower(TSuper::GetReferencePoint().size());
	for (size_t i = 0; i < lower.size(); ++i)
		lower[i] = (**std::min_element(front.begin(), front.end(), [i](const TPoint *point1, const TPoint *point2)->bool{return (*point1)[i] < (*point2)[i];}))[i];
	return _Stream(lower, TSuper::GetReferencePoint(), front, 0, TSuper::GetReferencePoint().back());
}

template <typename _TReal>
bool KMP_HV<_TReal>::_Covers(const TPoint &point, const TPoint &lower)
{
	assert(point.size() == lower.size());
	for (size_t i = 0; i < point.size() - 1; ++i)
	{
		if (point[i] > lower[i])
			return false;
	}
	return true;
}

template <typename _TReal>
bool KMP_HV<_TReal>::_PartCovers(const TPoint &point, const TPoint &upper)
{
	assert(point.size() == upper.size());
	for (size_t i = 0; i < point.size() - 1; ++i)
	{
		if (point[i] >= upper[i])
			return false;
	}
	return true;
}

template <typename _TReal>
int KMP_HV<_TReal>::_ContainsBoundary(const TPoint &point, const TPoint &lower, const size_t split)
{
	assert(point.size() == lower.size());
	if (lower[split] < point[split])
	{
		for (size_t j = 0; j < split; ++j)
		{
			if (lower[j] < point[j])
				return 1;
		}
	}
	else
		return -1;
	return 0;
}

template <typename _TReal>
_TReal KMP_HV<_TReal>::_ComputeVolume(const TPoint &lower, const TPoint &upper)
{
	assert(lower.size() == upper.size());
	TReal volume = 1;
	for (size_t i = 0; i < lower.size() - 1; ++i)
		volume *= upper[i] - lower[i];
	return volume;
}

template <typename _TReal>
size_t KMP_HV<_TReal>::_Pile(const TPoint &point, const TPoint &lower, const TPoint &upper)
{
	assert(point.size() == lower.size());
	assert(lower.size() == upper.size());
	size_t pile = point.size();
	for (size_t i = 0; i < point.size() - 1; ++i)
	{
		if (point[i] > lower[i])
		{
			if (pile != point.size())
				throw 0;
			pile = i;
		}
	}
	return pile;
}

template <typename _TReal>
_TReal KMP_HV<_TReal>::_ComputeTrellis(const TPoint &lower, const TPoint &upper, const TPoint &trellis)
{
	assert(lower.size() == upper.size());
	TReal result = 0;
	const size_t nSummands = 1 << lower.size() - 1;
	for (size_t i = 1; i <= nSummands; ++i)
	{
		TReal summand = 1;
		size_t count = 0;
		for (size_t j = 0; j < lower.size() - 1; ++j)
		{
			if (i & (1 << j))
			{
				summand *= upper[j] - trellis[j];
				++count;
			}
			else
				summand *= upper[j] - lower[j];
		}
		if (count % 2 == 0)
			result -= summand;
		else
			result += summand;
	}
	return result;
}

template <typename _TReal>
_TReal KMP_HV<_TReal>::_GetMedian(const TPoint &bounds, const size_t length)
{
	if (length == 1)
		return bounds[0];
	else if (length == 2)
		return bounds[1];
	TPoint v(length);
	std::copy(bounds.begin(), bounds.begin() + length, v.begin());
	std::sort(v.begin(), v.end());
	return length % 2 ? v[length / 2] : (v[length / 2 - 1] + v[length / 2]) / 2;
}

template <typename _TReal>
_TReal KMP_HV<_TReal>::_Stream(const TPoint &lower, const TPoint &upper, const TFront &front, size_t split, TReal cover)
{
	assert(lower.size() == upper.size());
	const size_t nPointSqrt = (size_t)sqrt((TReal)front.size());
	TReal result = 0;
	size_t coverIndex = 0;
	{
		TReal _cover = cover;
		size_t _coverIndex = -1;
		TReal volume = _ComputeVolume(lower, upper);
		while (cover == _cover && coverIndex < front.size())
		{
			if (_coverIndex == coverIndex)
				break;
			_coverIndex = coverIndex;
			if (_Covers((*front[coverIndex]), lower))
			{
				cover = (*front[coverIndex])[lower.size() - 1];
				result += volume * (_cover - cover);
			} else
				++coverIndex;
		}
		for (size_t i = coverIndex; i > 0; --i)
		{
			if ((*front[i - 1])[lower.size() - 1] == cover)
				--coverIndex;
		}
	}
	if (coverIndex == 0)
		return result;
	try
	{
		std::vector<size_t> piles(coverIndex);
		for (size_t i = 0; i < coverIndex; ++i)
			piles[i] = _Pile(*front[i], lower, upper);
		TPoint trellis = upper;
		TReal current = 0;
		TReal next = 0;
		size_t i = 0;
		do
		{
			current = (*front[i])[lower.size() - 1];
			do
			{
				if ((*front[i])[piles[i]] < trellis[piles[i]])
					trellis[piles[i]] = (*front[i])[piles[i]];
				++i;
				if (i < coverIndex)
					next = (*front[i])[lower.size() - 1];
				else
				{
					next = cover;
					break;
				}

			} while (next == current);
			result += _ComputeTrellis(lower, upper, trellis) * (next - current);
		} while (next != cover);
	}
	catch (...)
	{
		TReal boundary = -1;
		TPoint boundaries(coverIndex);
		TPoint nBoundaries(coverIndex);
		size_t boundIdx = 0;
		size_t nBoundIdx = 0;
		do
		{
			for (size_t i = 0; i < coverIndex; ++i)
			{
				const int contained = _ContainsBoundary((*front[i]), lower, split);
				if (contained == 1)
				{
					boundaries[boundIdx] = (*front[i])[split];
					++boundIdx;
				}
				else if (contained == 0)
				{
					nBoundaries[nBoundIdx] = (*front[i])[split];
					++nBoundIdx;
				}
			}
			if (boundIdx > 0)
				boundary = _GetMedian(boundaries, boundIdx);
			else if (nBoundIdx > nPointSqrt)
				boundary = _GetMedian(nBoundaries, nBoundIdx);
			else
				++split;
		} while (boundary == -1);

		TFront lowerFront, upperFront;
		TPoint _upper(upper);
		_upper[split] = boundary;
		TPoint regionLowC(lower);
		regionLowC[split] = boundary;

		for (size_t i = 0; i < coverIndex; ++i)
		{
			if (_PartCovers((*front[i]), _upper))
				upperFront.push_back(front[i]);
			if (_PartCovers((*front[i]), upper))
				lowerFront.push_back(front[i]);
		}
		if (upperFront.size() > 0)
			result += _Stream(lower, _upper, upperFront, split, cover);
		if (lowerFront.size() > 0)
			result += _Stream(regionLowC, upper, lowerFront, split, cover);
	}
	return result;
}
}
}
}
