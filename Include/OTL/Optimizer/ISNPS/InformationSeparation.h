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
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

namespace otl
{
namespace optimizer
{
namespace isnps
{
template <typename _TReal, typename _TMatrix>
void LinearIndependentlyFill(const boost::numeric::ublas::vector<_TReal> &convergenceDirection, _TMatrix &axesMatrix)
{
	boost::numeric::ublas::matrix_column<_TMatrix>(axesMatrix, 0) = convergenceDirection;
	for (size_t colIndex = 1; colIndex < axesMatrix.size2(); ++colIndex)
	{
		boost::numeric::ublas::matrix_column<_TMatrix> col(axesMatrix, colIndex);
		for (size_t rowIndex = 0; rowIndex < col.size(); ++rowIndex)
		{
			if (rowIndex == colIndex)
				col(rowIndex) = 1;
			else
				col(rowIndex) = 0;
		}
	}
}

template <typename _TMatrix>
void Orthogonalize(_TMatrix &axesMatrix)
{
	for (size_t colIndex = 1; colIndex < axesMatrix.size2(); ++colIndex)
	{
		boost::numeric::ublas::matrix_column<_TMatrix> nonOrthogonalCol(axesMatrix, colIndex);
		boost::numeric::ublas::vector<typename _TMatrix::value_type> col = nonOrthogonalCol;
		for (size_t orthogonalIndex = 0; orthogonalIndex < colIndex; ++orthogonalIndex)
		{
			boost::numeric::ublas::matrix_column<_TMatrix> orthogonalCol(axesMatrix, orthogonalIndex);
			col -= (boost::numeric::ublas::inner_prod(orthogonalCol, nonOrthogonalCol) / boost::numeric::ublas::inner_prod(orthogonalCol, orthogonalCol)) * orthogonalCol;
		}
		nonOrthogonalCol = col;
	}
}

template <typename _TMatrix>
void Normalize(_TMatrix &axesMatrix)
{
	for (size_t colIndex = 0; colIndex < axesMatrix.size2(); ++colIndex) //Normalize
	{
		boost::numeric::ublas::matrix_column<_TMatrix> col(axesMatrix, colIndex);
		const typename _TMatrix::value_type norm = boost::numeric::ublas::norm_2(col);
		for (size_t rowIndex = 0; rowIndex < col.size(); ++rowIndex)
			col[rowIndex] /= norm;
	}
}

template <typename _TReal>
boost::numeric::ublas::matrix<_TReal> RotateAxes(const boost::numeric::ublas::vector<_TReal> &convergenceDirection)
{
	typedef boost::numeric::ublas::matrix<_TReal> _TMatrix;
	assert(!convergenceDirection.empty());
	_TMatrix rotatedAxes(convergenceDirection.size(), convergenceDirection.size());
	LinearIndependentlyFill(convergenceDirection, rotatedAxes);
	Orthogonalize(rotatedAxes);
	Normalize(rotatedAxes);
	return rotatedAxes;
}

template <typename _TMatrix, typename _TReal>
_TReal InformationSeparation(const _TMatrix &invertRotateAxes, const boost::numeric::ublas::vector<_TReal> &direction, std::vector<_TReal> &distribution)
{
	assert(!direction.empty());
	assert(direction.size() == invertRotateAxes.size1() && invertRotateAxes.size1() == invertRotateAxes.size2());
	const boost::numeric::ublas::vector<_TReal> transform = boost::numeric::ublas::prod(invertRotateAxes, direction);
	assert(transform.size() > 1);
	distribution.assign(transform.begin() + 1, transform.end());
	return transform[0];
}
}
}
}
