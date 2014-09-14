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

#include <OTL/Mutation/Mutation.h>

namespace otl
{
namespace mutation
{
template <typename _TReal, typename _TDecision>
class WithMutation
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef Mutation<TReal, TDecision> TMutation;

	WithMutation(TMutation &mutation);
	~WithMutation(void);
	TMutation &GetMutation(void) const;

private:
	TMutation &mutation_;
};

template <typename _TReal, typename _TDecision>
WithMutation<_TReal, _TDecision>::WithMutation(TMutation &mutation) : mutation_(mutation)
{
}

template <typename _TReal, typename _TDecision>
WithMutation<_TReal, _TDecision>::~WithMutation(void)
{
}

template <typename _TReal, typename _TDecision>
typename WithMutation<_TReal, _TDecision>::TMutation &WithMutation<_TReal, _TDecision>::GetMutation(void) const
{
	return mutation_;
}
}
}
