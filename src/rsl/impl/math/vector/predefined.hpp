#pragma once
#include "../../rsl_core.hpp"
#include "../util/storage.hpp"

namespace rsl::math
{
    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type... args>
    struct swizzle;

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type s0>
    struct swizzle<Scalar, Size, Mode, s0>;

    template <size_type Size, storage_mode Mode, size_type s0>
    struct swizzle<bool, Size, Mode, s0>;

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type s0, size_type s1>
    struct swizzle<Scalar, Size, Mode, s0, s1>;

    template <size_type Size, storage_mode Mode, size_type s0, size_type s1>
    struct swizzle<bool, Size, Mode, s0, s1>;

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type s0, size_type s1, size_type s2>
    struct swizzle<Scalar, Size, Mode, s0, s1, s2>;

    template <size_type Size, storage_mode Mode, size_type s0, size_type s1, size_type s2>
    struct swizzle<bool, Size, Mode, s0, s1, s2>;

    template <
        arithmetic_type Scalar, size_type Size, storage_mode Mode, size_type s0, size_type s1, size_type s2,
        size_type s3>
    struct swizzle<Scalar, Size, Mode, s0, s1, s2, s3>;

    template <size_type Size, storage_mode Mode, size_type s0, size_type s1, size_type s2, size_type s3>
    struct swizzle<bool, Size, Mode, s0, s1, s2, s3>;

    namespace internal
    {
        template <arithmetic_type Scalar, size_type size, storage_mode Mode, bool isSigned>
        struct vector_additions;

        template <arithmetic_type Scalar, storage_mode Mode, bool isSigned>
        struct vector_additions<Scalar, 2, Mode, isSigned>;

        template <arithmetic_type Scalar, storage_mode Mode>
        struct vector_additions<Scalar, 2, Mode, true>;

        template <arithmetic_type Scalar, storage_mode Mode, bool isSigned>
        struct vector_additions<Scalar, 3, Mode, isSigned>;

        template <arithmetic_type Scalar, storage_mode Mode>
        struct vector_additions<Scalar, 3, Mode, true>;

        template <arithmetic_type Scalar, storage_mode Mode, bool isSigned>
        struct vector_additions<Scalar, 4, Mode, isSigned>;

        template <arithmetic_type Scalar, storage_mode Mode>
        struct vector_additions<Scalar, 4, Mode, true>;
    } // namespace internal

    template <arithmetic_type Scalar, size_type Size, storage_mode Mode>
    struct vector;

    template <arithmetic_type Scalar, storage_mode Mode>
    struct vector<Scalar, 1, Mode>;

    template <storage_mode Mode>
    struct vector<bool, 1, Mode>;

    template <arithmetic_type Scalar, storage_mode Mode>
    struct vector<Scalar, 2, Mode>;

    template <storage_mode Mode>
    struct vector<bool, 2, Mode>;

    template <arithmetic_type Scalar, storage_mode Mode>
    struct vector<Scalar, 3, Mode>;

    template <storage_mode Mode>
    struct vector<bool, 3, Mode>;

    template <arithmetic_type Scalar, storage_mode Mode>
    struct vector<Scalar, 4, Mode>;

    template <storage_mode Mode>
    struct vector<bool, 4, Mode>;
} // namespace rsl::math
