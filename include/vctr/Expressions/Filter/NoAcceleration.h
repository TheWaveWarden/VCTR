/*
  ==============================================================================
    DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.

    Copyright 2022- by sonible GmbH.

    This file is part of VCTR - Versatile Container Templates Reconceptualized.

    VCTR is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    VCTR is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License version 3 for more details.

    You should have received a copy of the GNU Lesser General Public License
    version 3 along with VCTR.  If not, see <https://www.gnu.org/licenses/>.
  ==============================================================================
*/

namespace vctr
{

template <size_t extent, class SrcType>
class NoAccelerationFilter : public ExpressionTemplateBase
{
public:
    using value_type = ValueType<SrcType>;

    VCTR_COMMON_UNARY_EXPRESSION_MEMBERS (NoAccelerationFilter)

    VCTR_FORCEDINLINE constexpr value_type operator[] (size_t i) const
    {
        return src[i];
    }

private:
    SrcType src;
};

/** This filter expression ensures that only basic operator[] based evaluation of the previous
    expression is possible.
 */
 constexpr ExpressionChainBuilder<NoAccelerationFilter> dontUseAcceleration;

}
