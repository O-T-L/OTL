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
@Article{,
  Title                    = {Performance Assesment of Multiobjective Optimizers: An Analysis and Review},
  Author                   = {Eckart Zitzler and Lothar Thiele and Marco Laumanns and Carlos M. Fonseca and Viviane Grunert da Fonseca},
  Journal                  = {IEEE Transactions on Evolutionary Computation},
  Year                     = {2003},
  Month                    = {April},
  Number                   = {2},
  Pages                    = {117-132},
  Volume                   = {7},
  Doi                      = {10.1109/TEVC.2003.810758}
}
*/

#pragma once

#include <OTL/Indicator/Indicator.h>

namespace otl
{
namespace indicator
{
namespace epsilon
{
template <typename _TReal>
class Epsilon : public Indicator<_TReal, _TReal>
{
public:
	typedef _TReal TReal;
	typedef TReal TMetric;
	typedef Indicator<TReal, TMetric> TSuper;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TFront TFront;

	Epsilon(const std::vector<TPoint> &front);
	virtual ~Epsilon(void);
	const std::vector<TPoint> &GetFront(void) const;

private:
	const std::vector<TPoint> &front_;
};

template <typename _TReal>
Epsilon<_TReal>::Epsilon(const std::vector<TPoint> &front) : front_(front)
{
}

template <typename _TReal>
Epsilon<_TReal>::~Epsilon(void)
{
}

template <typename _TReal>
const std::vector<typename Epsilon<_TReal>::TPoint> &Epsilon<_TReal>::GetFront(void) const
{
	return front_;
}
}
}
}
