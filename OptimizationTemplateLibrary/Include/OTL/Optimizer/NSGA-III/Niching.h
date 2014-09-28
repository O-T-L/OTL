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
#include <utility>
#include <algorithm>

namespace otl
{
namespace optimizer
{
namespace nsga_iii
{
template <typename _TIterator, typename _TReal>
std::vector<size_t> CountAssociation(const std::vector<std::list<std::pair<_TIterator, _TReal> > > &association)
{
	std::vector<size_t> counter(association.size());
	for (size_t i = 0; i < association.size(); ++i)
		counter[i] = association[i].size();
	return counter;
}

template <typename _TPointer, typename _TReal, typename _TIterator, typename _TSelection>
void Niching(std::vector<size_t> &counter, std::list<_TPointer> &front, std::vector<std::list<std::pair<typename std::list<_TPointer>::iterator, _TReal> > > &association, _TIterator begin, _TIterator end, _TSelection selection)
{
	typedef typename std::list<_TPointer>::iterator _TSrcIterator;
	typedef std::pair<_TSrcIterator, _TReal> _TAssociation;
	for (_TIterator dest = begin; dest != end; ++dest)
	{
		const size_t referenceIndex = std::min_element(counter.begin(), counter.end()) - counter.begin();
		auto &_association = association[referenceIndex];
		if (!_association.empty())
		{
			if (counter[referenceIndex])
			{
				auto select = selection(_association.begin(), _association.end());
				*dest = **select->first;
				_association.erase(select);
			}
			else
			{
				auto select = std::min_element(_association.begin(), _association.end(), [](const _TAssociation &association1, const _TAssociation &association2)->bool{return association1.second < association2.second;});
				*dest = **select->first;
				_association.erase(select);
			}
			++counter[referenceIndex];
		}
	}
}
}
}
}
