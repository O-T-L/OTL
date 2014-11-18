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

#include <limits>

namespace otl
{
namespace indicator
{
namespace hypervolume
{
template <typename _THypervolume>
class FilterAdapter : public _THypervolume
{
public:
	typedef _THypervolume THypervolume;
	typedef THypervolume TSuper;
	typedef typename TSuper::TMetric TMetric;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;

	FilterAdapter(const THypervolume &hypervolume);
	~FilterAdapter(void);
	bool IsValid(const TPoint &point) const;

protected:
	TMetric _DoEvaluate(const TFront &front);
};

template <typename _THypervolume>
FilterAdapter<_THypervolume>::FilterAdapter(const THypervolume &hypervolume)
	: TSuper(hypervolume)
{
}

template <typename _THypervolume>
FilterAdapter<_THypervolume>::~FilterAdapter(void)
{
}

template <typename _THypervolume>
bool FilterAdapter<_THypervolume>::IsValid(const TPoint &point) const
{
	if (point.size() != TSuper::GetReferencePoint().size())
		return false;
	for (size_t i = 0; i < point.size(); ++i)
	{
		if (point[i] > TSuper::GetReferencePoint()[i])
			return false;
	}
	return true;
}

template <typename _THypervolume>
typename FilterAdapter<_THypervolume>::TMetric FilterAdapter<_THypervolume>::_DoEvaluate(const TFront &front)
{
	typedef typename TFront::const_pointer TConstPointer;
	std::list<TConstPointer> _front;
	for (size_t i = 0; i < front.size(); ++i)
	{
		if (IsValid(*front[i]))
			_front.push_back(&front[i]);
	}
	if (_front.empty())
		return std::numeric_limits<TMetric>::infinity();
	else
	{
		TFront front(_front.size());
		typename std::list<TConstPointer>::iterator src = _front.begin();
		for (size_t i = 0; i < front.size(); ++i)
		{
			front[i] = **src;
			++src;
		}
		return TSuper::_DoEvaluate(front);
	}
}
}
}
}
