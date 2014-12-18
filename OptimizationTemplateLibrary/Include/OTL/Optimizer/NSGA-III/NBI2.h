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

#include <OTL/Utility/Weight/NormalBoundaryIntersection.h>

namespace otl
{
namespace optimizer
{
namespace nsga_iii
{
template <typename _TReal>
std::list<std::vector<_TReal> > NBI2(const size_t dimension, const size_t divisionBoundary, const size_t divisionInside)
{
	auto points = otl::utility::weight::NormalBoundaryIntersection<_TReal>(dimension, divisionBoundary);
	auto inside = otl::utility::weight::NormalBoundaryIntersection<_TReal>(dimension, divisionInside);
	const _TReal center = 1. / dimension;
	for (auto _point = inside.begin(); _point != inside.end(); ++_point)
	{
		auto &point = *_point;
		for (size_t i = 0; i < point.size(); ++i)
			point[i] = (center + point[i]) / 2;
		points.push_back(point);
	}
	return points;
}
}
}
}
