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
#include <cmath>
#include <utility>
#include <vector>
#include <algorithm>
#include <limits>

namespace otl
{
namespace optimizer
{
namespace msops
{
template <typename _TReal>
_TReal WeightedMinMax(const std::vector<_TReal> &objective, const std::vector<_TReal> &target)
{
	assert(objective.size() == target.size());
	_TReal max = std::numeric_limits<_TReal>::min();
	for(size_t i = 0; i < objective.size(); ++i)
	{
		assert(target[i] >= 0);
		const _TReal value = objective[i] / (target[i] + std::numeric_limits<_TReal>::epsilon());
		if(value > max)
			max = value;
	}
	return max;
}

template <typename _TReal>
_TReal VectorAngleDistanceScaling(const std::vector<_TReal> &objective, const std::vector<_TReal> &target, const _TReal factor)
{
	assert(objective.size() == target.size());
	const _TReal weightMagnitude = sqrt(std::inner_product(target.begin(), target.end(), target.begin(), (_TReal)0));
	const _TReal objectivesMagnitude = sqrt(std::inner_product(objective.begin(), objective.end(), objective.begin(), (_TReal)0));
	_TReal value = 0;
	for(size_t i = 0; i < objective.size(); ++i)
	{
		const _TReal unitWeight = target[i] / weightMagnitude;
		const _TReal unitObjectives = objective[i] / objectivesMagnitude;
		value += unitObjectives * unitWeight;
	}
	return objectivesMagnitude / pow(value, factor);
}

template <typename _TReal, typename _TIterator, typename _TAggregation>
std::list<std::vector<std::pair<_TReal, _TIterator> > > CalculateScores(const std::vector<std::vector<_TReal> > &targets, _TIterator begin, _TIterator end, _TAggregation aggregation)
{
	typedef std::pair<_TReal, _TIterator> _TScore;
	typedef std::vector<_TScore> _TColumn;
	std::list<_TColumn> scores;
	for (size_t j = 0; j < targets.size(); ++j)
	{
		scores.push_back(_TColumn(std::distance(begin, end)));
		size_t i = 0;
		for (_TIterator individual = begin; individual != end; ++individual, ++i)
		{
			_TScore &score = scores.back()[i];
			score.first = aggregation((*individual)->objective_, targets[j]);
			score.second = individual;
		}
	}
	return scores;
}

template <typename _TReal, typename _TIterator>
void AssignRanks(const std::list<std::vector<std::pair<_TReal, _TIterator> > > &scores)
{
	typedef std::pair<_TReal, _TIterator> _TScore;
	size_t j = 0;
	for (auto scoreColumn = scores.begin(); scoreColumn != scores.end(); ++scoreColumn, ++j)
	{
		std::vector<const _TScore *> _scoreColumn(scoreColumn->size());
		for (size_t i = 0; i < _scoreColumn.size(); ++i)
			_scoreColumn[i] = &(*scoreColumn)[i];
		std::sort(_scoreColumn.begin(), _scoreColumn.end(), [](const _TScore *score1, const _TScore *score2)->bool{return score1->first < score2->first;});
		for (size_t i = 0; i < _scoreColumn.size(); ++i)
			(**_scoreColumn[i]->second).fitness_[j] = i;
	}
}

template <typename _TIterator>
void SortFitness(_TIterator begin, _TIterator end)
{
	for (_TIterator individual = begin; individual != end; ++individual)
	{
		auto &fitness = (*individual)->fitness_;
		std::sort(fitness.begin(), fitness.end());
	}
}
}
}
}
