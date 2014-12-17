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

#include <vector>
#include <list>
#include <cstdint>
#include <random>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <OTL/Utility/Weight/NormalBoundaryIntersection.h>

namespace nbi
{
template <typename _TInteger>
_TInteger nCr(const _TInteger n, const _TInteger r)
{
	_TInteger f[n + 1];
    f[0] = 1;
    for (size_t i = 1; i <= n; ++i)
        f[i] = i * f[i - 1];
    return f[n] / f[r] / f[n - r];
}

BOOST_AUTO_TEST_CASE(NBI)
{
	typedef double _TReal;
	for (size_t dimension = 2; dimension < 5; ++dimension)
	{
		for (size_t division = 3; division < 6; ++division)
		{
			auto points = otl::utility::weight::NormalBoundaryIntersection<_TReal>(std::vector<size_t>(dimension - 1, division));
			BOOST_CHECK_EQUAL(points.size(), nCr<std::uintmax_t>(dimension + division - 1, division));
		}
	}
}
}
