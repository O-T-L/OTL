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
#include <cassert>
#include <vector>

namespace otl
{
namespace utility
{
namespace relation
{
template <typename _TReal>
bool Dominate(const std::vector<_TReal> &point1, const std::vector<_TReal> &point2)
{
	assert(point1.size() == point2.size());
	bool existBetter = false;
	for (size_t i = 0; i < point1.size(); ++i)
	{
		if (point2[i] < point1[i])
			return false;
		if (point1[i] < point2[i])
			existBetter = true;
	}
	return existBetter;
}

template <typename _TReal>
bool MaxDominate(const std::vector<_TReal> &point1, const std::vector<_TReal> &point2)
{
	assert(point1.size() == point2.size());
	bool existBetter = false;
	for (size_t i = 0; i < point1.size(); ++i)
	{
		if (point2[i] > point1[i])
			return false;
		if (point1[i] > point2[i])
			existBetter = true;
	}
	return existBetter;
}

template <typename _TReal>
bool WeaklyDominate(const std::vector<_TReal> &point1, const std::vector<_TReal> &point2)
{
	assert(point1.size() == point2.size());
	for (size_t i = 0; i < point1.size(); ++i)
	{
		if (point1[i] > point2[i])
			return false;
	}
	return true;
}

template <typename _TReal>
bool MaxWeaklyDominate(const std::vector<_TReal> &point1, const std::vector<_TReal> &point2)
{
	assert(point1.size() == point2.size());
	for (size_t i = 0; i < point1.size(); ++i)
	{
		if (point1[i] < point2[i])
			return false;
	}
	return true;
}
}
}
}
