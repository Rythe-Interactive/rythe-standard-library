#pragma once
#include "../../rsl_core.hpp"
#include "../util/storage.hpp"
#include "../vector/predefined.hpp"

namespace rsl::math
{
    template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, size_type RowIdx, storage_mode Mode>
    struct row;

    template <size_type RowCount, size_type ColCount, size_type RowIdx, storage_mode Mode>
    struct row<bool, RowCount, ColCount, RowIdx, Mode>;

    template <arithmetic_type Scalar, size_type RowCount, size_type RowIdx, storage_mode Mode>
    struct row<Scalar, RowCount, 1, RowIdx, Mode>;

    template <size_type RowCount, size_type RowIdx, storage_mode Mode>
    struct row<bool, RowCount, 1, RowIdx, Mode>;

    struct uniform_matrix_signal;
    struct identity_matrix_signal;

    template <arithmetic_type Scalar, storage_mode Mode>
    struct quaternion;

    template <arithmetic_type Scalar, size_type RowCount, size_type ColCount, storage_mode Mode>
    struct matrix;

    template <arithmetic_type Scalar, storage_mode Mode>
    struct matrix<Scalar, 2, 2, Mode>;

    template <arithmetic_type Scalar, storage_mode Mode>
    struct matrix<Scalar, 3, 3, Mode>;

    template <arithmetic_type Scalar, storage_mode Mode>
    struct matrix<Scalar, 4, 4, Mode>;
} // namespace rsl::math
