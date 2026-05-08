#pragma once

namespace rsl::internal
{
    template <bool Large>
    constexpr size_type recommended_fingerprint_size = Large ? 16ull : 8ull;

    template <typename Rep>
    struct unpacked_psl
    {
        using storage_type = Rep;
        storage_type psl;
        storage_type fingerprint;
    };

    template <bool Large = false, size_type FingerprintSize = recommended_fingerprint_size<Large>>
    struct hash_map_bucket
    {
        using storage_type = uint32;
        using psl_type = unpacked_psl<storage_type>;

        static constexpr bool memset_zero = true;

        static_assert(FingerprintSize < 32);
        static constexpr size_type fingerprint_size = FingerprintSize; // bits

        // increment the Nth bit that skips all fingerprint bits.
        static constexpr storage_type psl_increment = 1 << fingerprint_size;
        static constexpr storage_type fingerprint_mask = psl_increment - 1; // 0x00FF
        static constexpr storage_type psl_mask = ~fingerprint_mask;         // 0xFF00

        storage_type pslAndFingerprint;
        storage_type index;
    };

    template <size_type FingerprintSize>
    struct hash_map_bucket<true, FingerprintSize>
    {
        using storage_type = uint64;
        using psl_type = unpacked_psl<storage_type>;

        static constexpr bool memset_zero = true;

        static_assert(FingerprintSize < 64);
        static constexpr size_type fingerprint_size = FingerprintSize; // bits

        // increment the Nth bit that skips all fingerprint bits.
        static constexpr storage_type psl_increment = 1 << fingerprint_size;
        static constexpr storage_type fingerprint_mask = psl_increment - 1; // 0x0000FFFF
        static constexpr storage_type psl_mask = ~fingerprint_mask;         // 0xFFFF0000

        storage_type pslAndFingerprint;
        storage_type index;
    };
} // namespace rsl::internal
