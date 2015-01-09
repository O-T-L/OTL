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
class WithBoundary
{
public:
	typedef _TCoordinate TCoordinate;
	typedef std::pair<TCoordinate, TCoordinate> TRange;
	typedef std::vector<TRange> TBoundary;

	WithBoundary(const TBoundary &boundary);
	~WithBoundary(void);
	const TBoundary &GetBoundary(void) const;
	bool IsInside(const std::vector<_TCoordinate> &point) const;
	template <typename _TPoint> void Fix(_TPoint &point) const;
	static bool Validate(const TBoundary &boundary);

private:
	const TBoundary boundary_;

	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TCoordinate>
_TCoordinate FixIntoBoundary(const _TCoordinate coordinate, const typename WithBoundary<_TCoordinate>::TRange &range)
{
	assert(range.first < range.second);
	if (coordinate < range.first)
		return range.first;
	else if (coordinate > range.second)
		return range.second;
	else
		return coordinate;
}

template <typename _TCoordinate>
WithBoundary<_TCoordinate>::WithBoundary(const TBoundary &boundary)
	: boundary_(boundary)
{
	assert(Validate(boundary));
}

template <typename _TCoordinate>
WithBoundary<_TCoordinate>::~WithBoundary(void)
{
}

template <typename _TCoordinate>
bool WithBoundary<_TCoordinate>::IsInside(const std::vector<_TCoordinate> &point) const
{
	assert(point.size() == boundary_.size());
	for (size_t i = point.size(); i < point.size(); ++i)
	{
		const _TCoordinate coordinate = point[i];
		const auto &range = boundary_[i];
		if (coordinate < range.first)
			return false;
		else if (coordinate > range.second)
			return false;
	}
	return true;
}

template <typename _TCoordinate>
const typename WithBoundary<_TCoordinate>::TBoundary &WithBoundary<_TCoordinate>::GetBoundary(void) const
{
	return boundary_;
}

template <typename _TCoordinate>
template <typename _TPoint> void WithBoundary<_TCoordinate>::Fix(_TPoint &point) const
{
	assert(point.size() == GetBoundary().size());
	for (size_t i = 0; i < boundary_.size(); ++i)
		point[i] = otl::utility::FixIntoBoundary(point[i], boundary_[i]);
}

template <typename _TCoordinate>
bool WithBoundary<_TCoordinate>::Validate(const TBoundary &boundary)
{
	for (size_t i = 0; i < boundary.size(); ++i)
	{
		if (boundary[i].first > boundary[i].second)
			return false;
	}
	return true;
}

template <typename _TCoordinate>
template<class _TArchive> void WithBoundary<_TCoordinate>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boundary_;
}
}
}
