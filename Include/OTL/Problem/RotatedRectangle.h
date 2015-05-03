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

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "Rectangle.h"

namespace otl
{
namespace problem
{
template <typename _TReal>
class RotatedRectangle : public Rectangle<_TReal>
{
public:
	typedef _TReal TReal;
	typedef Rectangle<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TRange TRange;
	typedef typename TSuper::TBoundary TBoundary;
	typedef boost::numeric::ublas::matrix<TReal> TMatrix;

	RotatedRectangle(const TBoundary &boundary, const TBoundary &boundaryOptimal, const boost::numeric::ublas::vector<TReal> &direction);
	~RotatedRectangle(void);
	const TMatrix &GetAxes(void) const;
	boost::numeric::ublas::vector<TReal> Transform(const boost::numeric::ublas::vector<TReal> &point) const;

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _LinearIndependentlyFill(const boost::numeric::ublas::vector<TReal> &direction);
	void _Orthogonalize(void);
	void _Normalize(void);

private:
	TMatrix axes_;
};

template <typename _TReal>
RotatedRectangle<_TReal>::RotatedRectangle(const TBoundary &boundary, const TBoundary &boundaryOptimal, const boost::numeric::ublas::vector<TReal> &direction)
	: TSuper(boundary, boundaryOptimal)
	, axes_(direction.size(), direction.size())
{
	assert(direction.size() == boundary.size());
	_LinearIndependentlyFill(direction);
	_Orthogonalize();
	_Normalize();
}

template <typename _TReal>
RotatedRectangle<_TReal>::~RotatedRectangle(void)
{
}

template <typename _TReal>
const typename RotatedRectangle<_TReal>::TMatrix &RotatedRectangle<_TReal>::GetAxes(void) const
{
	return axes_;
}

template <typename _TReal>
boost::numeric::ublas::vector<_TReal> RotatedRectangle<_TReal>::Transform(const boost::numeric::ublas::vector<TReal> &point) const
{
	return boost::numeric::ublas::prod(axes_, point);
}

template <typename _TReal>
size_t RotatedRectangle<_TReal>::_DoEvaluate(TSolution &solution)
{
	boost::numeric::ublas::vector<_TReal> _decision(solution.decision_.size());
	std::copy(solution.decision_.begin(), solution.decision_.end(), _decision.begin());
	_decision = Transform(_decision);
	TDecision decision(_decision.begin(), _decision.end());
	TSuper::_Evaluate(decision, solution.objective_);
	return 1;
}

template <typename _TReal>
void RotatedRectangle<_TReal>::_LinearIndependentlyFill(const boost::numeric::ublas::vector<_TReal> &direction)
{
	assert(axes_.size1() == axes_.size2());
	assert(axes_.size1() == direction.size());
	boost::numeric::ublas::matrix_column<TMatrix>(axes_, 0) = direction;
	for (size_t colIndex = 1; colIndex < axes_.size2(); ++colIndex)
	{
		boost::numeric::ublas::matrix_column<TMatrix> col(axes_, colIndex);
		for (size_t rowIndex = 0; rowIndex < col.size(); ++rowIndex)
		{
			if (rowIndex == colIndex)
				col(rowIndex) = 1;
			else
				col(rowIndex) = 0;
		}
	}
}

template <typename _TReal>
void RotatedRectangle<_TReal>::_Orthogonalize(void)
{
	for (size_t colIndex = 1; colIndex < axes_.size2(); ++colIndex)
	{
		boost::numeric::ublas::matrix_column<TMatrix> nonOrthogonalCol(axes_, colIndex);
		boost::numeric::ublas::vector<typename TMatrix::value_type> col = nonOrthogonalCol;
		for (size_t orthogonalIndex = 0; orthogonalIndex < colIndex; ++orthogonalIndex)
		{
			boost::numeric::ublas::matrix_column<TMatrix> orthogonalCol(axes_, orthogonalIndex);
			col -= (boost::numeric::ublas::inner_prod(orthogonalCol, nonOrthogonalCol) / boost::numeric::ublas::inner_prod(orthogonalCol, orthogonalCol)) * orthogonalCol;
		}
		nonOrthogonalCol = col;
	}
}

template <typename _TReal>
void RotatedRectangle<_TReal>::_Normalize(void)
{
	for (size_t colIndex = 0; colIndex < axes_.size2(); ++colIndex) //Normalize
	{
		boost::numeric::ublas::matrix_column<TMatrix> col(axes_, colIndex);
		const typename TMatrix::value_type norm = boost::numeric::ublas::norm_2(col);
		for (size_t rowIndex = 0; rowIndex < col.size(); ++rowIndex)
			col[rowIndex] /= norm;
	}
}
}
}
