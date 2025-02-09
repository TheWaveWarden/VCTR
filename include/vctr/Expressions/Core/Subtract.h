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

//==============================================================================
/** Subtracts two vector like types */
template <size_t extent, class SrcAType, class SrcBType>
class SubtractVectors : ExpressionTemplateBase
{
public:
    using value_type = std::common_type_t<typename std::remove_cvref_t<SrcAType>::value_type, typename std::remove_cvref_t<SrcBType>::value_type>;

    using Expression = ExpressionTypes<value_type, SrcAType, SrcBType>;

    template <class SrcA, class SrcB>
    constexpr SubtractVectors (SrcA&& a, SrcB&& b)
        : srcA (std::forward<SrcA> (a)),
          srcB (std::forward<SrcB> (b)),
          storageInfo (srcA.getStorageInfo(), srcB.getStorageInfo())
    {}

    constexpr const auto& getStorageInfo() const { return storageInfo; }

    constexpr size_t size() const { return srcA.size(); }

    VCTR_FORCEDINLINE constexpr value_type operator[] (size_t i) const
    {
        return srcA[i] - srcB[i];
    }

    constexpr bool isNotAliased (const void* dst) const
    {
        if constexpr (is::expression<SrcAType> && is::anyVctr<SrcBType>)
        {
            return dst != srcB.data();
        }

        if constexpr (is::anyVctr<SrcAType> && is::expression<SrcBType>)
        {
            return dst != srcA.data();
        }

        return true;
    }

    VCTR_FORCEDINLINE const value_type* evalNextVectorOpInExpressionChain (value_type* dst) const
    requires is::suitableForAccelerateRealFloatBinaryVectorOp<SrcAType, SrcBType, value_type, detail::dontPreferIfIppAndAccelerateAreAvailable>
    {
        Expression::Accelerate::sub (srcA.evalNextVectorOpInExpressionChain (dst), srcB.evalNextVectorOpInExpressionChain (dst), dst, size());
        return dst;
    }

    VCTR_FORCEDINLINE const value_type* evalNextVectorOpInExpressionChain (value_type* dst) const
    requires is::suitableForIppRealOrComplexComplexFloatBinaryVectorOp<SrcAType, SrcBType, value_type, detail::preferIfIppAndAccelerateAreAvailable>
    {
        Expression::IPP::sub (srcA.evalNextVectorOpInExpressionChain (dst), srcB.evalNextVectorOpInExpressionChain (dst), dst, size());
        return dst;
    }

    //==============================================================================
    // AVX Implementation
    VCTR_FORCEDINLINE VCTR_TARGET ("avx") AVXRegister<value_type> getAVX (size_t i) const
    requires (archX64 && has::getAVX<SrcAType> && has::getAVX<SrcBType> && Expression::CommonElement::isFloatingPoint)
    {
        return Expression::AVX::sub (srcA.getAVX (i), srcB.getAVX (i));
    }

    VCTR_FORCEDINLINE VCTR_TARGET ("avx2") AVXRegister<value_type> getAVX (size_t i) const
    requires (archX64 && has::getAVX<SrcAType> && has::getAVX<SrcBType> && Expression::CommonElement::isUnsigned)
    {
        return Expression::AVX::sub (srcA.getAVX (i), srcB.getAVX (i));
    }

    //==============================================================================
    // SSE Implementation
    VCTR_FORCEDINLINE VCTR_TARGET ("sse4.1") SSERegister<value_type> getSSE (size_t i) const
    requires (archX64 && has::getSSE<SrcAType> && has::getSSE<SrcBType>)
    {
        return Expression::SSE::sub (srcA.getSSE (i), srcB.getSSE (i));
    }

    //==============================================================================
    // Neon Implementation
    NeonRegister<value_type> getNeon (size_t i) const
    requires (archARM && has::getNeon<SrcAType> && has::getNeon<SrcBType>)
    {
        return Expression::Neon::sub (srcA.getNeon (i), srcB.getNeon (i));
    }

private:
    SrcAType srcA;
    SrcBType srcB;

    using SrcAStorageInfoType = std::remove_cvref_t<std::invoke_result_t<decltype (&std::remove_cvref_t<SrcAType>::getStorageInfo), SrcAType>>;
    using SrcBStorageInfoType = std::remove_cvref_t<std::invoke_result_t<decltype (&std::remove_cvref_t<SrcBType>::getStorageInfo), SrcBType>>;

    const CombinedStorageInfo<SrcAStorageInfoType, SrcBStorageInfoType> storageInfo;
};

template <class SrcAType, class SrcBType>
requires (is::anyVctrOrExpression<std::remove_cvref_t<SrcAType>> &&
          is::anyVctrOrExpression<std::remove_cvref_t<SrcBType>>)
constexpr auto operator- (SrcAType&& a, SrcBType&& b)
{
    assertCommonSize (a, b);
    constexpr auto extent = getCommonExtent<SrcAType, SrcBType>();

    return SubtractVectors<extent, SrcAType, SrcBType> (std::forward<SrcAType> (a), std::forward<SrcBType> (b));
}

//==============================================================================
/** Subtracts a vector like type from a single value*/
template <size_t extent, class SrcType>
class SubtractVecFromSingle : ExpressionTemplateBase
{
public:
    using value_type = ValueType<SrcType>;

    using Expression = ExpressionTypes<value_type, SrcType>;

    template <class Src>
    constexpr SubtractVecFromSingle (typename Expression::CommonSrcElement::Type a, Src&& b)
        : src (std::forward<Src> (b)),
          single (a),
          asSSE (Expression::SSE::broadcast (a)),
          asNeon (Expression::Neon::broadcast (a))
    {
    }

    constexpr const auto& getStorageInfo() const { return src.getStorageInfo(); }

    constexpr size_t size() const { return src.size(); }

    VCTR_FORCEDINLINE constexpr value_type operator[] (size_t i) const
    {
        return single - src[i];
    }

    constexpr bool isNotAliased (const void* other) const
    {
        return src.isNotAliased (other);
    }

    VCTR_FORCEDINLINE const value_type* evalNextVectorOpInExpressionChain (value_type* dst) const
    requires is::suitableForAccelerateRealFloatVectorOp<SrcType, value_type, detail::dontPreferIfIppAndAccelerateAreAvailable>
    {
        auto fac = value_type (-1);
        Expression::Accelerate::smsa (src.evalNextVectorOpInExpressionChain (dst), fac, single, dst, size());
        return dst;
    }

    VCTR_FORCEDINLINE const value_type* evalNextVectorOpInExpressionChain (value_type* dst) const
    requires is::suitableForIppRealOrComplexFloatVectorOp<SrcType, value_type, detail::preferIfIppAndAccelerateAreAvailable>
    {
        Expression::IPP::sub (single, src.evalNextVectorOpInExpressionChain (dst), dst, sizeToInt (size()));
        return dst;
    }

    //==============================================================================
    // AVX Implementation
    VCTR_FORCEDINLINE VCTR_TARGET ("avx") AVXRegister<value_type> getAVX (size_t i) const
    requires (archX64 && has::getAVX<SrcType> && Expression::allElementTypesSame && Expression::CommonElement::isFloatingPoint)
    {
        return Expression::AVX::sub (Expression::AVX::fromSSE (asSSE, asSSE), src.getAVX (i));
    }

    VCTR_FORCEDINLINE VCTR_TARGET ("avx2") AVXRegister<value_type> getAVX (size_t i) const
    requires (archX64 && has::getAVX<SrcType> && Expression::allElementTypesSame && Expression::CommonElement::isInt)
    {
        return Expression::AVX::sub (Expression::AVX::fromSSE (asSSE, asSSE), src.getAVX (i));
    }
    //==============================================================================
    // SSE Implementation
    VCTR_FORCEDINLINE VCTR_TARGET ("sse4.1") SSERegister<value_type> getSSE (size_t i) const
    requires (archX64 && has::getSSE<SrcType> && Expression::allElementTypesSame)
    {
        return Expression::SSE::sub (asSSE, src.getSSE (i));
    }

    //==============================================================================
    // Neon Implementation
    NeonRegister<value_type> getNeon (size_t i) const
    requires (archARM && has::getNeon<SrcType> && Expression::allElementTypesSame)
    {
        return Expression::Neon::sub (asNeon, src.getNeon (i));
    }

private:
    SrcType src;

    const typename Expression::CommonSrcElement::Type single;
    const typename Expression::SSESrc asSSE;
    const typename Expression::NeonSrc asNeon;
};

/** Returns an expression that subtracts a given vector-like source from a single value */
template <class Src>
requires is::anyVctrOrExpression<Src>
constexpr auto operator- (typename std::remove_cvref_t<Src>::value_type single, Src&& vec)
{
    return SubtractVecFromSingle<extentOf<Src>, Src> (single, std::forward<Src> (vec));
}

//==============================================================================
/** Subtracts a single value from a vector like type */
template <size_t extent, class SrcType>
class SubtractSingleFromVec : ExpressionTemplateBase
{
public:
    using value_type = ValueType<SrcType>;

    using Expression = ExpressionTypes<value_type, SrcType>;

    template <class Src>
    constexpr SubtractSingleFromVec (Src&& b, typename Expression::CommonSrcElement::Type a)
        : src (std::forward<Src> (b)),
          single (a),
          asSSE (Expression::SSE::broadcast (a)),
          asNeon (Expression::Neon::broadcast (a))
    {
    }

    constexpr const auto& getStorageInfo() const { return src.getStorageInfo(); }

    constexpr size_t size() const { return src.size(); }

    VCTR_FORCEDINLINE constexpr value_type operator[] (size_t i) const
    {
        return src[i] - single;
    }

    constexpr bool isNotAliased (const void* other) const
    {
        return src.isNotAliased (other);
    }

    VCTR_FORCEDINLINE const value_type* evalNextVectorOpInExpressionChain (value_type* dst) const
    requires is::suitableForAccelerateRealOrComplexFloatVectorOp<SrcType, value_type, detail::dontPreferIfIppAndAccelerateAreAvailable>
    {
        Expression::Accelerate::add (src.evalNextVectorOpInExpressionChain (dst), -single, dst, size());
        return dst;
    }

    VCTR_FORCEDINLINE const value_type* evalNextVectorOpInExpressionChain (value_type* dst) const
    requires is::suitableForIppRealOrComplexFloatVectorOp<SrcType, value_type, detail::preferIfIppAndAccelerateAreAvailable>
    {
        Expression::IPP::sub (src.evalNextVectorOpInExpressionChain (dst), single, dst, sizeToInt (size()));
        return dst;
    }

    //==============================================================================
    // AVX Implementation
    VCTR_FORCEDINLINE VCTR_TARGET ("avx") AVXRegister<value_type> getAVX (size_t i) const
    requires (archX64 && has::getAVX<SrcType> && Expression::allElementTypesSame && Expression::CommonElement::isFloatingPoint)
    {
        return Expression::AVX::sub (src.getAVX (i), Expression::AVX::fromSSE (asSSE, asSSE));
    }

    VCTR_FORCEDINLINE VCTR_TARGET ("avx2") AVXRegister<value_type> getAVX (size_t i) const
    requires (archX64 && has::getAVX<SrcType> && Expression::allElementTypesSame && Expression::CommonElement::isInt)
    {
        return Expression::AVX::sub (src.getAVX (i), Expression::AVX::fromSSE (asSSE, asSSE));
    }
    //==============================================================================
    // SSE Implementation
    VCTR_FORCEDINLINE VCTR_TARGET ("sse4.1") SSERegister<value_type> getSSE (size_t i) const
    requires (archX64 && has::getSSE<SrcType> && Expression::allElementTypesSame)
    {
        return Expression::SSE::sub (src.getSSE (i), asSSE);
    }

    //==============================================================================
    // Neon Implementation
    NeonRegister<value_type> getNeon (size_t i) const
    requires (archARM && has::getNeon<SrcType> && Expression::allElementTypesSame)
    {
        return Expression::Neon::sub (src.getNeon (i), asNeon);
    }

private:
    SrcType src;

    const typename Expression::CommonSrcElement::Type single;
    const typename Expression::SSESrc asSSE;
    const typename Expression::NeonSrc asNeon;
};

/** Returns an expression that subtracts a given vector-like source to a given single value */
template <class Src>
requires is::anyVctrOrExpression<Src>
constexpr auto operator- (Src&& vec, typename std::remove_cvref_t<Src>::value_type single)
{
    return SubtractSingleFromVec<extentOf<Src>, Src> (std::forward<Src> (vec), single);
}

} // namespace vctr
