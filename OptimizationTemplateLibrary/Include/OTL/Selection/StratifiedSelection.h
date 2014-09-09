#pragma once

#include <cassert>
#include <algorithm>

namespace otl
{
namespace selection
{
template <typename _TIndividual, typename _TDestIterator, typename _TSelectNoncritical, typename _TSelectCritical>
_TDestIterator StratifiedSelection(std::list<std::list<_TIndividual> > &fronts, _TDestIterator begin, _TDestIterator end, _TSelectNoncritical selectNoncritical, _TSelectCritical selectCritical)
{
	_TDestIterator dest = begin;
	for (auto front = fronts.begin(); front != fronts.end() && dest != end; ++front)
	{
		if ((size_t)std::distance(dest, end) >= front->size())
			dest = selectNoncritical(*front, dest, end);
		else
		{
			dest = selectCritical(*front, dest, end);
			assert(dest == end);
			break;
		}
	}
	return dest;
}
}
}
