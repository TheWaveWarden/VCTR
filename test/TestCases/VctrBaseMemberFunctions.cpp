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

#include <vctr_test_utils/vctr_test_common.h>

TEST_CASE ("subSpan", "[VctrBaseMemberFunctions]")
{
    auto v = UnitTestValues<int32_t>::template vector<100, 0>();
    auto a = UnitTestValues<int32_t>::template array<100, 1>();
    const auto cv = UnitTestValues<int32_t>::template vector<100, 2>();
    const auto ca = UnitTestValues<int32_t>::template array<100, 3>();

    //==============================================================================
    // Span of a non-const Vector with compile time constant startIdx
    auto vStaticOffset1 = v.template subSpan<1>();
    static_assert (std::same_as<decltype (vStaticOffset1)::element_type, int32_t>);
    static_assert (vStaticOffset1.getExtent() == std::dynamic_extent);
    REQUIRE (vStaticOffset1.size() == 99);
    REQUIRE (vStaticOffset1[0] == v[1]);

    // Span of a non-const Vector with compile time constant startIdx and length
    auto vStaticOffset2 = v.template subSpan<1, 80>();
    static_assert (std::same_as<decltype (vStaticOffset2)::element_type, int32_t>);
    static_assert (vStaticOffset2.getExtent() == 80);
    REQUIRE (vStaticOffset2.size() == 80);
    REQUIRE (vStaticOffset2[0] == v[1]);

    // Span of a non-const Array with compile time constant startIdx
    auto aStaticOffset1 = a.template subSpan<1>();
    static_assert (std::same_as<decltype (aStaticOffset1)::element_type, int32_t>);
    static_assert (aStaticOffset1.getExtent() == 99);
    REQUIRE (aStaticOffset1.size() == 99);
    REQUIRE (aStaticOffset1[0] == a[1]);

    // Span of a non-const Array with compile time constant startIdx and length
    auto aStaticOffset2 = a.template subSpan<1, 80>();
    static_assert (std::same_as<decltype (aStaticOffset2)::element_type, int32_t>);
    static_assert (aStaticOffset2.getExtent() == 80);
    REQUIRE (aStaticOffset2.size() == 80);
    REQUIRE (aStaticOffset2[0] == a[1]);

    // Span of a const Vector with compile time constant startIdx
    auto cvStaticOffset1 = cv.template subSpan<1>();
    static_assert (std::same_as<decltype (cvStaticOffset1)::element_type, const int32_t>);
    static_assert (cvStaticOffset1.getExtent() == std::dynamic_extent);
    REQUIRE (cvStaticOffset1.size() == 99);
    REQUIRE (cvStaticOffset1[0] == cv[1]);

    // Span of a const Vector with compile time constant startIdx
    auto cvStaticOffset2 = cv.template subSpan<1, 80>();
    static_assert (std::same_as<decltype (cvStaticOffset2)::element_type, const int32_t>);
    static_assert (cvStaticOffset2.getExtent() == 80);
    REQUIRE (cvStaticOffset2.size() == 80);
    REQUIRE (cvStaticOffset2[0] == cv[1]);

    // Span of a const Array with compile time constant startIdx
    auto caStaticOffset1 = ca.template subSpan<1>();
    static_assert (std::same_as<decltype (caStaticOffset1)::element_type, const int32_t>);
    static_assert (caStaticOffset1.getExtent() == 99);
    REQUIRE (caStaticOffset1.size() == 99);
    REQUIRE (caStaticOffset1[0] == ca[1]);

    // Span of a const Array with compile time constant startIdx
    auto caStaticOffset2 = ca.template subSpan<1, 80>();
    static_assert (std::same_as<decltype (caStaticOffset2)::element_type, const int32_t>);
    static_assert (caStaticOffset2.getExtent() == 80);
    REQUIRE (caStaticOffset2.size() == 80);
    REQUIRE (caStaticOffset2[0] == ca[1]);

    //==============================================================================
    // Span of a non-const Vector with compile time constant startIdx
    auto vDynamicOffset1 = v.subSpan (1);
    static_assert (std::same_as<decltype (vDynamicOffset1)::element_type, int32_t>);
    static_assert (vDynamicOffset1.getExtent() == std::dynamic_extent);
    REQUIRE (vDynamicOffset1.size() == 99);
    REQUIRE (vDynamicOffset1[0] == v[1]);

    // Span of a non-const Vector with compile time constant startIdx and length
    auto vDynamicOffset2 = v.subSpan (1, 80);
    static_assert (std::same_as<decltype (vDynamicOffset2)::element_type, int32_t>);
    static_assert (vDynamicOffset2.getExtent() == std::dynamic_extent);
    REQUIRE (vDynamicOffset2.size() == 80);
    REQUIRE (vDynamicOffset2[0] == v[1]);

    // Span of a non-const Array with compile time constant startIdx
    auto aDynamicOffset1 = a.subSpan (1);
    static_assert (std::same_as<decltype (aDynamicOffset1)::element_type, int32_t>);
    static_assert (aDynamicOffset1.getExtent() == std::dynamic_extent);
    REQUIRE (aDynamicOffset1.size() == 99);
    REQUIRE (aDynamicOffset1[0] == a[1]);

    // Span of a non-const Array with compile time constant startIdx and length
    auto aDynamicOffset2 = a.subSpan (1, 80);
    static_assert (std::same_as<decltype (aDynamicOffset2)::element_type, int32_t>);
    static_assert (aDynamicOffset2.getExtent() == std::dynamic_extent);
    REQUIRE (aDynamicOffset2.size() == 80);
    REQUIRE (aDynamicOffset2[0] == a[1]);

    // Span of a const Vector with compile time constant startIdx
    auto cvDynamicOffset1 = cv.subSpan (1);
    static_assert (std::same_as<decltype (cvDynamicOffset1)::element_type, const int32_t>);
    static_assert (cvDynamicOffset1.getExtent() == std::dynamic_extent);
    REQUIRE (cvDynamicOffset1.size() == 99);
    REQUIRE (cvDynamicOffset1[0] == cv[1]);

    // Span of a const Vector with compile time constant startIdx
    auto cvDynamicOffset2 = cv.subSpan (1, 80);
    static_assert (std::same_as<decltype (cvDynamicOffset2)::element_type, const int32_t>);
    static_assert (cvDynamicOffset2.getExtent() == std::dynamic_extent);
    REQUIRE (cvDynamicOffset2.size() == 80);
    REQUIRE (cvDynamicOffset2[0] == cv[1]);

    // Span of a const Array with compile time constant startIdx
    auto caDynamicOffset1 = ca.subSpan (1);
    static_assert (std::same_as<decltype (caDynamicOffset1)::element_type, const int32_t>);
    static_assert (caDynamicOffset1.getExtent() == std::dynamic_extent);
    REQUIRE (caDynamicOffset1.size() == 99);
    REQUIRE (caDynamicOffset1[0] == ca[1]);

    // Span of a const Array with compile time constant startIdx
    auto caDynamicOffset2 = ca.subSpan (1, 80);
    static_assert (std::same_as<decltype (caDynamicOffset2)::element_type, const int32_t>);
    static_assert (caDynamicOffset2.getExtent() == std::dynamic_extent);
    REQUIRE (caDynamicOffset2.size() == 80);
    REQUIRE (caDynamicOffset2[0] == ca[1]);

    //==============================================================================
    // Spans of a spans
    const auto& s1 = vStaticOffset1;
    auto spanStaticOffset1 = s1.template subSpan<1>();
    static_assert (std::same_as<decltype (spanStaticOffset1)::element_type, int32_t>);
    static_assert (spanStaticOffset1.getExtent() == std::dynamic_extent);
    REQUIRE (spanStaticOffset1.size() == 98);
    REQUIRE (spanStaticOffset1[0] == vStaticOffset1[1]);

    const auto& s2 = vStaticOffset2;
    auto spanStaticOffset2 = s2.template subSpan<1>();
    static_assert (std::same_as<decltype (spanStaticOffset2)::element_type, int32_t>);
    static_assert (spanStaticOffset2.getExtent() == 79);
    REQUIRE (spanStaticOffset2.size() == 79);
    REQUIRE (spanStaticOffset2[0] == vStaticOffset2[1]);

    const auto& s3 = cvStaticOffset1;
    auto spanStaticOffset3 = s3.template subSpan<1>();
    static_assert (std::same_as<decltype (spanStaticOffset3)::element_type, const int32_t>);
    static_assert (spanStaticOffset3.getExtent() == std::dynamic_extent);
    REQUIRE (spanStaticOffset3.size() == 98);
    REQUIRE (spanStaticOffset3[0] == cvStaticOffset1[1]);
}

TEST_CASE ("forEach", "[VctrBaseMemberFunctions]")
{
    const auto loremIpsum = UnitTestValues<std::string>::template array<4, 1>();

    auto strings = loremIpsum;

    // In place for each function
    strings.forEach ([] (std::string& s) { for (auto& c : s) c = std::toupper (c); });
    REQUIRE (strings[0] == "LOREM");
    REQUIRE (strings[1] == "IPSUM");
    REQUIRE (strings[2] == "DOLOR");
    REQUIRE (strings[3] == "SIT");

    // Returning for each function
    strings.forEach ([] (const std::string& s) { return s + "."; });
    REQUIRE (strings[0] == "LOREM.");
    REQUIRE (strings[1] == "IPSUM.");
    REQUIRE (strings[2] == "DOLOR.");
    REQUIRE (strings[3] == "SIT.");

    // In place for each function with index
    strings.forEach ([] (std::string& s, size_t idx) { s = std::to_string (idx); });
    REQUIRE (strings[0] == "0");
    REQUIRE (strings[1] == "1");
    REQUIRE (strings[2] == "2");
    REQUIRE (strings[3] == "3");

    // Returning for each function with index
    strings.forEach ([] (const std::string& s, size_t idx) { return s + std::string (idx, '.'); });
    REQUIRE (strings[0] == "0");
    REQUIRE (strings[1] == "1.");
    REQUIRE (strings[2] == "2..");
    REQUIRE (strings[3] == "3...");

    // In place for each function with arguments
    strings.forEach ([] (std::string& s, size_t pos, size_t n) { s = s.substr (pos, n); }, 1, 2);
    REQUIRE (strings[0].empty());
    REQUIRE (strings[1] == ".");
    REQUIRE (strings[2] == "..");
    REQUIRE (strings[3] == "..");

    // Returning for each function with argument
    strings.forEach (std::plus(), "x");
    REQUIRE (strings[0] == "x");
    REQUIRE (strings[1] == ".x");
    REQUIRE (strings[2] == "..x");
    REQUIRE (strings[3] == "..x");

    const auto cv = UnitTestValues<double>::template vector<100, 2>();

    // Non mutating for each function
    double sum1 = 0.0;
    cv.forEach ([&] (double e) { sum1 += e; });

    double sum2 = 0.0;
    for (auto& e : cv)
        sum2 += e;

    REQUIRE (sum1 == sum2);

    sum1 = 0.0;
    sum2 = 0.0;

    // Non mutating for each function with idx
    cv.forEach ([&] (double e, size_t idx) { sum1 += e + double (idx); });
    for (size_t i = 0; i < cv.size(); ++i)
        sum2 += cv[i] + double (i);

    REQUIRE (sum1 == sum2);

    sum1 = 0.0;
    sum2 = 0.0;

    // Non mutating for each function with arg
    cv.forEach ([&] (double e, double x) { sum1 += e + x; }, -1.0);
    for (auto& e : cv)
        sum2 += e - 1.0;

    REQUIRE (sum1 == sum2);
}
