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
// clang-format off
struct dBFS : Constant<20> {};

struct dBVoltage : Constant<20> {};

struct dBPower : Constant<10> {};
// clang-format on

template <size_t extent, class SrcType, class DecibelConstant, class MinDb>
using MagToDb = ClampLowByConstant<extent,
                                   MultiplyVecByConstant<extent,
                                                         Log10<extent, SrcType>,
                                                         DecibelConstant>,
                                   MinDb>;

/** Converts the source magnitude into a decibel representation.

    The calculation is max (constant * log10 (src), minDb), with constant being either 20 for dBFS or dBVoltage
    (the typical value when dealing with digital audio amplitudes) or 10 for dBPower.

   @tparam DecibelConstant: Either vctr::dBFS, vctr::dBVoltage or vctr::dBPower.
 * @tparam minDb: The lower threshold for the resulting dB value and thus the value for a magnitude of 0.
 */
template <is::constantWrapper DecibelConstant, auto minDb = -100>
constexpr ExpressionChainBuilder<MagToDb, DecibelConstant, Constant<minDb>> magToDb;

namespace detail
{
template <is::constantWrapper C>
struct InvertedConstant
{
    static constexpr double value = 1.0 / double (C::value);
};

} // namespace detail

template <size_t extent, class SrcType, class DecibelConstant>
using DBToMag = PowConstantBase<extent,
                                MultiplyVecByConstant<extent,
                                                      SrcType,
                                                      detail::InvertedConstant<DecibelConstant>>,
                                Constant<10>>;

/** Converts the source decibel values into their magnitude representation.

    The calculation is pow (10, src * (1 / constant), with constant being either 20 for dBFS or dBVoltage
    (the typical value when dealing with digital audio amplitudes) or 10 for dBPower.

   @tparam DecibelConstant: Either vctr::dBFS, vctr::dBVoltage or vctr::dBPower.
 */
template <is::constantWrapper DecibelConstant>
constexpr ExpressionChainBuilder<DBToMag, DecibelConstant> dbToMag;

} // namespace vctr
