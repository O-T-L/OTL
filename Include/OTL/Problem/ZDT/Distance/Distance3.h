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
#include <cmath>
#include <boost/math/constants/constants.hpp>

namespace otl
{
namespace problem
{
namespace zdt
{
namespace distance
{
template <typename _TInteger>
size_t NumberOfTrueBits(_TInteger coding)
{
	size_t count = 0;
	for (size_t i = 0; i < sizeof(_TInteger) * 8; ++i)
	{
		if (coding & 1)
			count += 1;
		coding >>= 1;
	}
	return count;
}

template <typename _TIterator>
size_t Distance3(_TIterator begin, _TIterator end)
{
	size_t sum = 0;
	for (_TIterator i = begin; i != end; ++i)
	{
		const size_t bits = NumberOfTrueBits(*i);
		if (bits < 5)
			sum += 2 + bits;
		else
			sum += 1;
	}
	return sum;
}
}
}
}
}
