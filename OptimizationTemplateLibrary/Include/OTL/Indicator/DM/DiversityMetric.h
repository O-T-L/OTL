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
@CONFERENCE{,
  author = {Kalyanmoy Deb and Sachin Jain},
  title = {Running Performance Metrics for Evolutionary Multi-Objective Optimization},
  booktitle = {AsiaPacific Conference on Simulated Evolution and Learning},
  year = {2002},
  volume = {1},
  pages = {13-20},
  publisher = {Orchid Country Club, Nanyang Technical University}
}
*/

#pragma once

#include <cmath>
#include <numeric>
#include <boost/dynamic_bitset.hpp>
#include <OTL/Indicator/Indicator.h>
#include <OTL/Utility/WithSpaceBoundary.h>
#include "Utility.h"

namespace otl
{
namespace indicator
{
namespace dm
{
template <typename _TReal>
class DiversityMetric : public Indicator<_TReal, _TReal>, public otl::utility::WithSpaceBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef TReal TMetric;
	typedef Indicator<TReal, TMetric> TSuper;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TMinMax TMinMax;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TBoundary TBoundary;
	typedef boost::dynamic_bitset<> TGridContainer;

	DiversityMetric(const TBoundary &boundary, const std::vector<size_t> &division, const std::vector<TPoint> &front);
	virtual ~DiversityMetric(void);
	const std::vector<size_t> &GetDivision(void) const;
	const std::vector<TPoint> &GetFront(void) const;
	bool IsCrossBorder(const std::vector<TReal> &coordinate) const;
	std::vector<size_t> ConvertGridCoordinate(const std::vector<TReal> &coordinate) const;
	std::vector<size_t> ConvertGridCoordinate(const size_t offset) const;
	size_t ConvertOffset(const std::vector<size_t> &gridCoordinate) const;
	template <typename _TIterator> TGridContainer FillGrid(_TIterator begin, _TIterator end);

protected:
	template <typename _TIterator> TGridContainer _FillGrid(_TIterator begin, _TIterator end);
	TMetric _DoEvaluate(const TFront &front);
	TReal _CalcDM(const TGridContainer &gridContainer) const;

private:
	const std::vector<size_t> division_;
	const std::vector<TPoint> &front_;
};

template <typename _TReal>
DiversityMetric<_TReal>::DiversityMetric(const TBoundary &boundary, const std::vector<size_t> &division, const std::vector<TPoint> &front)
	: otl::utility::WithSpaceBoundary<TReal>(ExpandHalfBox(boundary, division))
	, division_(division)
	, front_(front)
{
}

template <typename _TReal>
DiversityMetric<_TReal>::~DiversityMetric(void)
{
}

template <typename _TReal>
const std::vector<size_t> &DiversityMetric<_TReal>::GetDivision(void) const
{
	return division_;
}

template <typename _TReal>
const std::vector<typename DiversityMetric<_TReal>::TPoint> &DiversityMetric<_TReal>::GetFront(void) const
{
	return front_;
}

template <typename _TReal>
bool DiversityMetric<_TReal>::IsCrossBorder(const std::vector<TReal> &coordinate) const
{
	assert(coordinate.size() == this->GetBoundary().size());
	for (size_t i = 0; i < coordinate.size(); ++i)
	{
		if (coordinate[i] < this->GetBoundary()[i].first || coordinate[i] > this->GetBoundary()[i].second)
			return true;
	}
	return false;
}

template <typename _TReal>
std::vector<size_t> DiversityMetric<_TReal>::ConvertGridCoordinate(const std::vector<TReal> &coordinate) const
{
	assert(coordinate.size() == this->GetBoundary().size());
	std::vector<size_t> gridCoordinate(coordinate.size());
	for (size_t i = 0; i < coordinate.size(); ++i)
	{
		assert(this->GetBoundary()[i].first <= coordinate[i] && coordinate[i] <= this->GetBoundary()[i].second);
		gridCoordinate[i] = (size_t)floor((coordinate[i] - this->GetBoundary()[i].first) * division_[i] / (this->GetBoundary()[i].second - this->GetBoundary()[i].first));
	}
	return gridCoordinate;
}

template <typename _TReal>
std::vector<size_t> DiversityMetric<_TReal>::ConvertGridCoordinate(const size_t offset) const
{
	std::vector<size_t> gridCoordinate(division_.size());
	size_t dimension, offsetRemain;
	for (offsetRemain = offset, dimension = 0; offsetRemain && dimension < gridCoordinate.size(); offsetRemain /= division_[dimension], ++dimension)
		gridCoordinate[dimension] = offsetRemain % division_[dimension];
	assert(!offsetRemain);
	while (dimension < gridCoordinate.size())
		gridCoordinate[dimension++] = 0;
	return gridCoordinate;
}

template <typename _TReal>
size_t DiversityMetric<_TReal>::ConvertOffset(const std::vector<size_t> &gridCoordinate) const
{
	size_t offset = 0;
	for (size_t i = gridCoordinate.size(), j = i - 1; i; i = j)
	{
		j = i - 1;
		offset *= division_[j];
		assert(0 <= gridCoordinate[j] && gridCoordinate[j] < division_[j]);
		offset += gridCoordinate[j];
	}
	return offset;
}

template <typename _TReal>
template <typename _TIterator> typename DiversityMetric<_TReal>::TGridContainer DiversityMetric<_TReal>::FillGrid(_TIterator begin, _TIterator end)
{
	TGridContainer gridContainer(std::accumulate(division_.begin(), division_.end(), (size_t)1, std::multiplies<size_t>()));
	for (_TIterator i = begin; i != end; ++i)
	{
		const auto &point = *i;
		if (IsCrossBorder(point))
			continue;
		const auto gridCoordinate = ConvertGridCoordinate(point);
		gridContainer.set(ConvertOffset(gridCoordinate));
	}
	return gridContainer;
}

template <typename _TReal>
template <typename _TIterator> typename DiversityMetric<_TReal>::TGridContainer DiversityMetric<_TReal>::_FillGrid(_TIterator begin, _TIterator end)
{
	TGridContainer gridContainer(std::accumulate(division_.begin(), division_.end(), (size_t)1, std::multiplies<size_t>()));
	for (_TIterator i = begin; i != end; ++i)
	{
		const auto &point = **i;
		if (IsCrossBorder(point))
			continue;
		const auto gridCoordinate = ConvertGridCoordinate(point);
		gridContainer.set(ConvertOffset(gridCoordinate));
	}
	return gridContainer;
}

template <typename _TReal>
typename DiversityMetric<_TReal>::TMetric DiversityMetric<_TReal>::_DoEvaluate(const TFront &front)
{
	const TReal dm1 = _CalcDM(_FillGrid(front.begin(), front.end()));
	const TReal dm2 = _CalcDM(FillGrid(front_.begin(), front_.end()));
	return dm1 / dm2;
}

template <typename _TReal>
_TReal DiversityMetric<_TReal>::_CalcDM(const TGridContainer &gridContainer) const
{
	TReal score = 0;
	std::vector<TReal> scoreList(this->GetBoundary().size());
	for (size_t offset = 0; offset < gridContainer.size(); ++offset)
	{
		const auto gridCoordinate = ConvertGridCoordinate(offset);
		for (size_t i = 0; i < gridCoordinate.size(); ++i)
		{
			if (gridContainer[ConvertOffset(gridCoordinate)])
			{
				if (gridCoordinate[i] == 0)
				{
					std::vector<size_t> downCoordinate = gridCoordinate;
					++downCoordinate[i];
					if (gridContainer[ConvertOffset(downCoordinate)])
						scoreList[i] = 1;
					else
						scoreList[i] = .5;
				}
				else if (gridCoordinate[i] == division_[i] - 1)
				{
					std::vector<size_t> upCoordinate = gridCoordinate;
					--upCoordinate[i];
					if (gridContainer[ConvertOffset(upCoordinate)])
						scoreList[i] = 1;
					else
						scoreList[i] = .5;
				}
				else
				{
					std::vector<size_t> downCoordinate = gridCoordinate;
					++downCoordinate[i];
					std::vector<size_t> upCoordinate = gridCoordinate;
					--upCoordinate[i];
					const bool down = gridContainer[ConvertOffset(downCoordinate)];
					const bool up = gridContainer[ConvertOffset(upCoordinate)];
					if (down == false && up == false)
						scoreList[i] = .75;
					if (down == false && up == true)
						scoreList[i] = .67;
					if (down == true && up == false)
						scoreList[i] = .67;
					if (down == true && up == true)
						scoreList[i] = 1;
				}
			}
			else
			{
				if (gridCoordinate[i] == 0)
				{
					std::vector<size_t> downCoordinate = gridCoordinate;
					++downCoordinate[i];
					if (gridContainer[ConvertOffset(downCoordinate)])
						scoreList[i] = .5;
					else
						scoreList[i] = 0;
				}
				else if (gridCoordinate[i] == division_[i] - 1)
				{
					std::vector<size_t> upCoordinate = gridCoordinate;
					--upCoordinate[i];
					if (gridContainer[ConvertOffset(upCoordinate)])
						scoreList[i] = .5;
					else
						scoreList[i] = 0;
				}
				else
				{
					std::vector<size_t> downCoordinate = gridCoordinate;
					++downCoordinate[i];
					std::vector<size_t> upCoordinate = gridCoordinate;
					--upCoordinate[i];
					const bool down = gridContainer[ConvertOffset(downCoordinate)];
					const bool up = gridContainer[ConvertOffset(upCoordinate)];
					if (down == false && up == false)
						scoreList[i] = 0;
					if (down == false && up == true)
						scoreList[i] = .5;
					if (down == true && up == false)
						scoreList[i] = .5;
					if (down == true && up == true)
						scoreList[i] = .75;
				}
			}
		}
		score += std::accumulate(scoreList.begin(), scoreList.end(), (TReal)0) / gridCoordinate.size();
	}
	return score / gridContainer.size();
}
}
}
}
