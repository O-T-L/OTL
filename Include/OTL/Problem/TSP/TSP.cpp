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
#include <algorithm>
#include "TSP.h"

namespace otl
{
namespace problem
{
namespace tsp
{
bool CheckPathTSP(std::vector<size_t> path)
{
	const size_t nCities = path.size();
	std::sort(path.begin(), path.end());
	std::unique(path.begin(), path.end());
	return path.size() == nCities;
}
}
}
}
