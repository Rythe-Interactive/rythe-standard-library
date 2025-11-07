#pragma once

namespace rsl
{
    struct range
    {
        size_type offset;
        size_type size;
    };

    struct byte_range
    {
        size_type offset;
        size_type size;

        [[nodiscard]] [[rythe_always_inline]] operator range() const noexcept { return { .offset = offset, .size = size }; }
    };

    struct unaligned_stride_range
    {
        size_type byteOffset;
        size_type count;
        size_type stride;

        [[nodiscard]] [[rythe_always_inline]] operator byte_range() const noexcept { return { .offset = byteOffset, .size = count * stride }; }
    };

    template<typename T>
    struct unaligned_typed_range
    {
        size_type byteOffset;
        size_type count;
        constexpr static size_type stride = sizeof(T);

        [[nodiscard]] [[rythe_always_inline]] operator byte_range() const noexcept { return { .offset = byteOffset, .size = count * stride }; }
        [[nodiscard]] [[rythe_always_inline]] operator unaligned_stride_range() const noexcept { return { .byteOffset = byteOffset, .count = count, .stride = stride }; }
    };

    struct stride_range
    {
        size_type offset;
        size_type count;
        size_type stride;

        [[nodiscard]] [[rythe_always_inline]] operator byte_range() const noexcept { return { .offset = offset * stride, .size = count * stride }; }
        [[nodiscard]] [[rythe_always_inline]] operator unaligned_stride_range() const noexcept { return { .byteOffset = offset * stride, .count = count, .stride = stride }; }
    };

    template<typename T>
    struct typed_range
    {
        size_type offset;
        size_type count;
        constexpr static size_type stride = sizeof(T);

        [[nodiscard]] [[rythe_always_inline]] operator byte_range() const noexcept { return { .offset = offset * stride, .size = count * stride }; }
        [[nodiscard]] [[rythe_always_inline]] operator unaligned_stride_range() const noexcept { return { .byteOffset = offset * stride, .count = count, .stride = stride }; }
        [[nodiscard]] [[rythe_always_inline]] operator stride_range() const noexcept { return { .offset = offset, .count = count, .stride = stride }; }
        [[nodiscard]] [[rythe_always_inline]] operator unaligned_typed_range<T>() const noexcept { return { .byteOffset = offset * stride, .count = count }; }
    };
}
