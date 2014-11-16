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
#include <iterator>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/exception/all.hpp>

namespace otl
{
namespace optimizer
{
namespace nsga_iii
{
template <typename _TReal, typename _TIterator>
std::vector<_TReal> ComputeIdealPoint(_TIterator begin, _TIterator end)
{
	typedef typename std::iterator_traits<_TIterator>::value_type _TPointer;
	assert(begin != end);
	std::vector<_TReal> idealPoint((**begin).objective_.size());
	for (size_t obj = 0; obj < idealPoint.size(); ++obj)
	{
		_TPointer extreme = *std::min_element(begin, end, [obj](_TPointer individual1, _TPointer individual2)->bool{return individual1->objective_[obj] < individual2->objective_[obj];});
		idealPoint[obj] = extreme->objective_[obj];
	}
	return idealPoint;
}

template <typename _TReal, typename _TIterator>
void ShiftPopulation(const std::vector<_TReal> &idealPoint, _TIterator begin, _TIterator end)
{
	for (_TIterator individual = begin; individual != end; ++individual)
	{
		auto &_individual = **individual;
		_individual.translatedObjective_.resize(idealPoint.size());
		for (size_t i = 0; i < idealPoint.size(); ++i)
		{
			assert(idealPoint[i] <= _individual.objective_[i]);
			_individual.translatedObjective_[i] = _individual.objective_[i] - idealPoint[i];
		}
	}
}

template <typename _TReal>
_TReal ASF(const size_t axis, const _TReal epsilon, const std::vector<_TReal> &objective)
{
	_TReal asf = axis ? objective[0] / epsilon : objective[0];
	for (size_t i = 1; i < objective.size(); ++i)
	{
		const _TReal value = axis == i ? objective[i] : objective[i] / epsilon;
		if (value > asf)
			asf = value;
	}
	return asf;
}

template <typename _TReal, typename _TIterator>
_TIterator LocateExtremeIndividual(const size_t axis, const _TReal epsilon, _TIterator begin, _TIterator end)
{
	assert(begin != end);
	_TReal minASF = ASF(axis, epsilon, (**begin).translatedObjective_);
	_TIterator extremeIndividual = begin;
	for (_TIterator i = ++_TIterator(begin); i != end; ++i)
	{
		const _TReal asf = ASF(axis, epsilon, (**i).translatedObjective_);
		if (asf < minASF)
		{
			minASF = asf;
			extremeIndividual = i;
		}
	}
	return extremeIndividual;
}

template <typename _TIterator, typename _TReal>
std::vector<typename std::iterator_traits<_TIterator>::value_type> ComputeExtremePoints(_TIterator begin, _TIterator end, const _TReal epsilon, const std::vector<_TReal> &idealPoint)
{
	typedef typename std::iterator_traits<_TIterator>::value_type _TPointer;
	std::vector<_TPointer> extremeIndividuals(idealPoint.size());
	for (size_t i = 0; i < extremeIndividuals.size(); ++i)
		extremeIndividuals[i] = *LocateExtremeIndividual(i, epsilon, begin, end);
	return extremeIndividuals;
}

template<class _TReal>
boost::numeric::ublas::matrix<_TReal> InvertMatrix(boost::numeric::ublas::matrix<_TReal> &input)
{
	assert(input.size1() == input.size2());
	boost::numeric::ublas::permutation_matrix<std::size_t> pm(input.size1());
	if (boost::numeric::ublas::lu_factorize(input, pm))
		boost::throw_exception(std::exception());
	boost::numeric::ublas::matrix<_TReal> inverse(input.size1(), input.size2());
	inverse.assign(boost::numeric::ublas::identity_matrix<_TReal>(input.size1()));
	boost::numeric::ublas::lu_substitute(input, pm, inverse);
	return inverse;
}

template <typename _TReal, typename _TPointer>
std::vector<_TReal> ComputeIntercept(const std::vector<_TPointer> &extremeIndividuals)
{
	try
	{
		typedef boost::numeric::ublas::matrix<_TReal> _TMatrix;
		_TMatrix extremePoints(extremeIndividuals.size(), extremeIndividuals.size());
		for (size_t row = 0; row < extremePoints.size1(); ++row)
		{
			auto &extremeIndividual = *extremeIndividuals[row];
			boost::numeric::ublas::matrix_row<_TMatrix> extremePoint(extremePoints, row);
			for (size_t col = 0; col < extremePoint.size(); ++col)
				extremePoint(col) = extremeIndividual.objective_[col];
		}
		const auto inverse = InvertMatrix(extremePoints);
		assert(inverse.size1() == inverse.size2());
		assert(inverse.size1() == extremePoints.size2());
		boost::numeric::ublas::vector<_TReal> temp(inverse.size2());
		std::fill(temp.begin(), temp.end(), 1);
		temp = boost::numeric::ublas::prod(inverse, temp);
		std::vector<_TReal> intercepts(temp.size());
		for (size_t i = 0; i < intercepts.size(); ++i)
			intercepts[i] = 1 / temp(i);
		return intercepts;
	}
	catch (...)
	{
		std::vector<_TReal> intercepts(extremeIndividuals.size());
		for (size_t i = 0; i < intercepts.size(); ++i)
			intercepts[i] = extremeIndividuals[i]->objective_[i];
		return intercepts;
	}
}

template <typename _TReal, typename _TIterator>
void NormalizePopulation(const std::vector<_TReal> &idealPoint, const std::vector<_TReal> &intercepts, _TIterator begin, _TIterator end)
{
	assert(intercepts.size() == idealPoint.size());
	std::vector<_TReal> _intercepts(intercepts.size());
	for (size_t i = 0; i < intercepts.size(); ++i)
		_intercepts[i] = intercepts[i] - idealPoint[i];
	for (_TIterator i = begin; i != end; ++i)
	{
		for (size_t j = 0; j < intercepts.size(); ++j)
			(**i).translatedObjective_[j] /= _intercepts[j];
	}
}

template <typename _TIterator, typename _TReal>
void Normalize(_TIterator begin, _TIterator end, const _TReal epsilon)
{
	const std::vector<_TReal> idealPoint = ComputeIdealPoint<_TReal>(begin, end);
	ShiftPopulation(idealPoint, begin, end);
	auto extremeIndividuals = ComputeExtremePoints(begin, end, epsilon, idealPoint);
	const auto intercepts = ComputeIntercept<_TReal>(extremeIndividuals);
	NormalizePopulation(idealPoint, intercepts, begin, end);
}
}
}
}
