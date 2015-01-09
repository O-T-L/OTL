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

#include <cstddef>
#include <vector>
#include <utility>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>

namespace otl
{
namespace utility
{
template <typename _TCoordinate>
class WithSpaceBoundary
{
public:
	typedef _TCoordinate TCoordinate;
	typedef std::pair<TCoordinate, TCoordinate> TMinMax;
	typedef std::vector<TMinMax> TBoundary;

	WithSpaceBoundary(const TBoundary &boundary);
	~WithSpaceBoundary(void);
	const TBoundary &GetBoundary(void) const;
	template <typename _TPoint> void Fix(_TPoint &point) const;

protected:
	static bool _Check(const TBoundary &boundary);

private:
	const TBoundary boundary_;

	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TCoordinate>
_TCoordinate Fix(const _TCoordinate coordinate, const typename WithSpaceBoundary<_TCoordinate>::TMinMax &minMax)
{
	assert(minMax.first < minMax.second);
	if (coordinate < minMax.first)
		return minMax.first;
	else if (coordinate > minMax.second)
		return minMax.second;
	else
		return coordinate;
}

template <typename _TCoordinate>
WithSpaceBoundary<_TCoordinate>::WithSpaceBoundary(const TBoundary &boundary)
	: boundary_(boundary)
{
	assert(_Check(boundary));
}

template <typename _TCoordinate>
WithSpaceBoundary<_TCoordinate>::~WithSpaceBoundary(void)
{
}

template <typename _TCoordinate>
const typename WithSpaceBoundary<_TCoordinate>::TBoundary &WithSpaceBoundary<_TCoordinate>::GetBoundary(void) const
{
	return boundary_;
}

template <typename _TCoordinate>
template <typename _TPoint> void WithSpaceBoundary<_TCoordinate>::Fix(_TPoint &point) const
{
	assert(point.size() == GetBoundary().size());
	for (size_t i = 0; i < boundary_.size(); ++i)
		point[i] = otl::utility::Fix(point[i], boundary_[i]);
}

template <typename _TCoordinate>
bool WithSpaceBoundary<_TCoordinate>::_Check(const TBoundary &boundary)
{
	for (size_t i = 0; i < boundary.size(); ++i)
	{
		if (boundary[i].first > boundary[i].second)
			return false;
	}
	return true;
}

template <typename _TCoordinate>
template<class _TArchive> void WithSpaceBoundary<_TCoordinate>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boundary_;
}
}
}
