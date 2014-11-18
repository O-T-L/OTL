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
#include <vector>
#include <list>

namespace otl
{
namespace optimizer
{
namespace sms_emoa
{
template <typename _TReal, typename _TIterator>
std::vector<const std::vector<_TReal> *> MakeFront(_TIterator begin, _TIterator end)
{
	typedef std::vector<_TReal> _TPoint;
	typedef std::vector<const _TPoint *> _TFront;
	_TFront front(std::distance(begin, end));
	_TIterator src = begin;
	for (size_t i = 0; i < front.size(); ++i)
	{
		front[i] = &(**src).objective_;
		++src;
	}
	return front;
}

template <typename _TIterator, typename _TReal, typename _TMakeHypervolume>
void ContributionAssignment(_TIterator begin, _TIterator end, const std::vector<_TReal> &referencePoint, _TMakeHypervolume makeHypervolume)
{
	assert(std::distance(begin, end) > 1);
	auto front = MakeFront<_TReal>(begin, end);
	auto hypervolume = makeHypervolume(referencePoint);
	const auto totalHV = hypervolume(front);
	_TIterator individual = begin;
	for (size_t i = 0; i < front.size(); ++i)
	{
		assert(individual != end);
		assert(front[i] == &(**individual).objective_);
		front[i] = front[(i + 1) % front.size()];
		const auto removedHV = hypervolume(front);
		(**individual).hvContribution_ = totalHV - removedHV;
		front[i] = &(**individual).objective_;
		++individual;
	}
}
}
}
}
