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

namespace otl
{
namespace utility
{
template <typename _TRandom>
class WithRandom
{
public:
	typedef _TRandom TRandom;

	WithRandom(TRandom random);
	~WithRandom(void);
	TRandom GetRandom(void);

private:
	TRandom random_;
};

template <typename _TRandom>
WithRandom<_TRandom>::WithRandom(TRandom random) : random_(random)
{
}

template <typename _TRandom>
WithRandom<_TRandom>::~WithRandom(void)
{
}

template <typename _TRandom>
_TRandom WithRandom<_TRandom>::GetRandom(void)
{
	return random_;
}
}
}
