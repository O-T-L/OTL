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

#include <cassert>
#include <cmath>
#include "Fitness.h"

namespace otl
{
namespace optimizer
{
namespace grea
{
size_t GridDifference(const std::vector<size_t> &gridCoordinate1, const std::vector<size_t> &gridCoordinate2)
{
	assert(gridCoordinate1.size() == gridCoordinate2.size());
	size_t gd = 0;
	for (size_t i = 0; i < gridCoordinate1.size(); ++i)
	{
		if (gridCoordinate1[i] > gridCoordinate2[i])
			gd += gridCoordinate1[i] - gridCoordinate2[i];
		else
			gd += gridCoordinate2[i] - gridCoordinate1[i];
	}
	return gd;
}
}
}
}
