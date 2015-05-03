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
@ARTICLE{,
  author = {Julián Molina Luque and Luis V. Santana-Quintero and Alfredo García
	Hernández-Díaz and Carlos A. Coello Coello and Rafael Caballero},
  title = {g-dominance: Reference point based dominance for multiobjective metaheuristics},
  journal = {European Journal of Operational Research},
  year = {2009},
  volume = {197},
  pages = {685-692},
  number = {2},
  doi = {10.1016/j.ejor.2008.07.015}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <OTL/Utility/Relation/Pareto.h>

namespace otl
{
namespace optimizer
{
namespace g_nsga_ii
{
template <typename _TReal>
bool CheckFlag(const std::vector<_TReal> &referencePoint, const std::vector<_TReal> &point)
{
	assert(referencePoint.size() == point.size());
	if (otl::utility::relation::WeaklyDominate(referencePoint, point) || otl::utility::relation::MaxWeaklyDominate(referencePoint, point))
		return true;
	else
		return false;
}

template <typename _TReal>
bool G_Dominance(const std::vector<_TReal> &point1, const std::vector<_TReal> &point2, const std::vector<_TReal> &referencePoint)
{
	assert(point1.size() == point2.size());
	assert(point1.size() == referencePoint.size());
	const bool flag1 = CheckFlag(referencePoint, point1);
	const bool flag2 = CheckFlag(referencePoint, point2);
	if (flag1 == flag2)
		return otl::utility::relation::Dominate(point1, point2);
	else
		return flag1 > flag2;
}
}
}
}
