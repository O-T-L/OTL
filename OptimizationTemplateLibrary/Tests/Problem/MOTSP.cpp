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

#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <OTL/Problem/TSP/MOTSP.h>

namespace motsp
{
template <typename _TReal>
void FillMatrix(boost::numeric::ublas::symmetric_matrix<_TReal> &matrix, const size_t cities, const size_t index)
{
	matrix.resize(cities, cities);
	for (size_t i = 0; i < matrix.size1(); ++i)
	{
		for (size_t j = 0; j <= i; ++j)
			matrix(i, j) = index;
	}
}

template <typename _TReal>
std::vector<boost::numeric::ublas::symmetric_matrix<_TReal> > GenerateAdjacencyMatrics(const size_t cities, const size_t nObjectives)
{
	std::vector<boost::numeric::ublas::symmetric_matrix<_TReal> > adjacencyMatrics(nObjectives);
	for (size_t i = 0; i < adjacencyMatrics.size(); ++i)
		FillMatrix(adjacencyMatrics[i], cities, i + 1);
	return adjacencyMatrics;
}

BOOST_AUTO_TEST_CASE(Correlate_Matrix)
{
	typedef double _TReal;
	const size_t cities = 30;
	const size_t nObjectives = 3;
	const auto adjacencyMatrics = GenerateAdjacencyMatrics<_TReal>(cities, nObjectives);
	{
		auto _adjacencyMatrics = adjacencyMatrics;
		otl::problem::tsp::CorrelateAdjacencyMatrics(std::vector<_TReal>(adjacencyMatrics.size() - 1, 0), _adjacencyMatrics);
		for (size_t i = 0; i < adjacencyMatrics.size(); ++i)
		{
			for (size_t nRow = 0; nRow < adjacencyMatrics[i].size1(); ++nRow)
			{
				for (size_t nCol = 0; nCol < adjacencyMatrics[i].size2(); ++nCol)
					BOOST_CHECK_CLOSE(adjacencyMatrics[i](nRow, nCol), _adjacencyMatrics[i](nRow, nCol), 0.001);
			}
		}
	}
	{
		auto _adjacencyMatrics = adjacencyMatrics;
		otl::problem::tsp::CorrelateAdjacencyMatrics(std::vector<_TReal>(adjacencyMatrics.size() - 1, 1), _adjacencyMatrics);
		for (size_t i = 0; i < adjacencyMatrics.size(); ++i)
		{
			for (size_t nRow = 0; nRow < adjacencyMatrics[i].size1(); ++nRow)
			{
				for (size_t nCol = 0; nCol < adjacencyMatrics[i].size2(); ++nCol)
					BOOST_CHECK_CLOSE(adjacencyMatrics.front()(nRow, nCol), _adjacencyMatrics[i](nRow, nCol), 0.001);
			}
		}
	}
}
}
