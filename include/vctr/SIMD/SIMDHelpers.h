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

namespace vctr::detail
{
template <size_t value>
requires (is::powerOfTwoInt<value>)
constexpr size_t previousMultipleOf (size_t numElements)
{
    constexpr auto numMaskBits = std::bit_width (value) - 1;
    constexpr auto allBitsOne = size_t (-1);
    constexpr auto mask = allBitsOne << numMaskBits;

    return numElements & mask;
}

template <size_t value>
requires (is::powerOfTwoInt<value>)
constexpr size_t nextMultipleOf (size_t numElements)
{
    return (numElements + value - 1) & -value;
}

template <uintptr_t requiredAlignment = Config::maxSIMDRegisterSize>
inline bool isPtrAligned (const void* ptr)
{
    return reinterpret_cast<std::uintptr_t> (ptr) % requiredAlignment == 0;
}
} // namespace vctr::detail

namespace vctr
{

/** A helper class to describe some properties regarding the storage class wrapped in a VctrBase instance.

    VctrBase expects a suitable storage info to supply three member variables
    - memberAlignment:        Specifies the alignment requirement of the storage member of the VctrBase instance. The
                              default case is simply alignof (StorageType). Can be used to make memory aligned when the
                              StorageType wraps a stack memory buffer, such as std::array. Must be static constexpr.
    - dataIsSIMDAligned:      Specifies if the data is aligned for AVX access (x64) or Neon access (ARM).
    - hasSIMDExtendedStorage: Specifies if the actually allocated memory is extended with unused dummy values at the end
                              in a way that it is a multiple of the size of an AVX register (x64) or Neon register (ARM).

    There are template specialisations for std::vector with an AlignedAllocator and std::array, which define all
    variable as static constexpr in order to make this struct as lightweight as possible in most cases.

 */
template <has::sizeAndData StorageType>
struct StorageInfo
{
    StorageInfo (const StorageType& storage)
        : dataIsSIMDAligned (detail::isPtrAligned (storage.data())),
          hasSIMDExtendedStorage ((storage.size() * sizeof (typename StorageType::value_type)) % Config::maxSIMDRegisterSize == 0)
    {}

    template <is::storageInfo OtherStorageInfoType>
    StorageInfo (const OtherStorageInfoType& other)
        : dataIsSIMDAligned (other.dataIsSIMDAligned),
          hasSIMDExtendedStorage (other.hasSIMDExtendedStorage)
    {}

    static constexpr size_t memberAlignment = alignof (StorageType);

    bool dataIsSIMDAligned;

    bool hasSIMDExtendedStorage;
};

template <class ElementType, size_t alignmentInBytes>
struct StorageInfo<std::vector<ElementType, AlignedAllocator<ElementType, alignmentInBytes>>>
{
    constexpr StorageInfo (const auto&) {}

    static constexpr size_t memberAlignment = alignof (std::vector<ElementType, AlignedAllocator<ElementType, alignmentInBytes>>);

    static constexpr bool dataIsSIMDAligned = alignmentInBytes == Config::maxSIMDRegisterSize;

    /** This is ensured by the AlignedAllocator */
    static constexpr bool hasSIMDExtendedStorage = true;
};

template <class ElementType, size_t size>
struct StorageInfo<std::array<ElementType, size>>
{
    constexpr StorageInfo (const auto&) {}

    static constexpr size_t memberAlignment = Config::maxSIMDRegisterSize;

    /** This is ensured by the enforced member alignment */
    static constexpr bool dataIsSIMDAligned = true;

    static constexpr bool hasSIMDExtendedStorage = (size * sizeof (ElementType)) % Config::maxSIMDRegisterSize == 0;
};

/** A storage info type especially used to pass compile time constant traits when viewing externally owned memory via a
    Span in case we know that the viewed memory will fulfil certain traits.
 */
template <bool isDataSIMDAligned, bool isStorageSIMDExtended, size_t customMemberAlignment>
struct StaticStorageInfo
{
    constexpr StaticStorageInfo() = default;
    constexpr StaticStorageInfo (const StaticStorageInfo&) {}

    static constexpr bool dataIsSIMDAligned = isDataSIMDAligned;
    static constexpr bool hasSIMDExtendedStorage = isStorageSIMDExtended;
    static constexpr size_t memberAlignment = customMemberAlignment;
};

namespace detail
{

template <bool, bool>
struct ConstexprStorageInfoChecker
{
};
} // namespace detail

namespace is
{
template <class T>
concept constexprStorageInfo = requires (const T&) { detail::ConstexprStorageInfoChecker<T::dataIsSIMDAligned, T::hasSIMDExtendedStorage>(); };
}

template <class InfoA, class InfoB>
struct CombinedStorageInfo
{
    CombinedStorageInfo (const InfoA& a, const InfoB& b)
        : dataIsSIMDAligned (a.dataIsSIMDAligned && b.dataIsSIMDAligned),
          hasSIMDExtendedStorage (a.hasSIMDExtendedStorage && b.hasSIMDExtendedStorage)
    {}

    bool dataIsSIMDAligned;

    bool hasSIMDExtendedStorage;
};

template <is::constexprStorageInfo InfoA, is::constexprStorageInfo InfoB>
struct CombinedStorageInfo<InfoA, InfoB>
{
    constexpr CombinedStorageInfo (const InfoA&, const InfoB&) {}

    static constexpr bool dataIsSIMDAligned = InfoA::dataIsSIMDAligned && InfoB::dataIsSIMDAligned;

    static constexpr bool hasSIMDExtendedStorage = InfoA::hasSIMDExtendedStorage && InfoB::hasSIMDExtendedStorage;
};

} // namespace vctr
