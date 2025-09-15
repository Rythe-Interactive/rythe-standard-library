#pragma once

namespace rsl
{
    template <typename T, typename = void>
    struct has_is_transparent : false_type {};

    template <typename T>
    struct has_is_transparent<T, void_t<typename T::is_transparent>> : true_type {};

    template <typename T, typename = void>
    struct has_view_hash_identical : false_type {};

    template <typename T>
    struct has_view_hash_identical<T, void_t<typename T::view_hash_identical>> : true_type {};

    namespace internal
    {
        template <typename Key>
        concept has_key_view_alternative = container_like<Key> && requires { typename Key::const_view_type; } &&
                has_view_v<Key, typename Key::const_view_type()> && convertible_to<Key, typename Key::const_view_type> &&
                (!has_view_hash_identical<Key>::value || Key::view_hash_identical);

        template <typename KeyType, typename Hasher, bool HasIsTransparent = has_is_transparent<Hasher>::value>
        struct hasher_wrapper;

        template <typename KeyType, typename Hasher>
        struct hasher_wrapper<KeyType, Hasher, false>
        {
            Hasher hasher{};

            [[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
            {
                id_type hash = hasher(val);
                hash *= 0xc4ceb9fe1a85ec53ull;
                hash ^= hash >> 33;
                return hash;
            }
        };

        template <typename KeyType, typename Hasher>
        struct hasher_wrapper<KeyType, Hasher, true>
        {
            using is_transparent = typename Hasher::is_transparent;

            Hasher hasher{};

            [[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
            {
                id_type hash = hasher(val);
                hash *= 0xc4ceb9fe1a85ec53ull;
                hash ^= hash >> 33;
                return hash;
            }
        };

        template <typename KeyType, typename T>
        struct hasher_wrapper<KeyType, ::rsl::hash<T>, false>
        {
            ::rsl::hash<T> hasher{};

            [[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
            {
                return hasher(val);
            }
        };

        template <typename KeyType, typename T>
        struct hasher_wrapper<KeyType, fast_hash<T>, false>
        {
            fast_hash<T> hasher{};

            [[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
            {
                return hasher(val);
            }
        };

        template <typename KeyType, typename T>
        struct hasher_wrapper<KeyType, protected_hash<T>, false>
        {
            protected_hash<T> hasher{};

            [[nodiscard]] [[rythe_always_inline]] constexpr id_type hash(const KeyType& val) const noexcept
            {
                return hasher(val);
            }
        };

        template <typename, typename>
        struct can_hash_key_view_alternative
        {
            constexpr static bool value = false;
        };

        template <has_key_view_alternative KeyType, template<typename> typename Hasher>
        struct can_hash_key_view_alternative<KeyType, Hasher<KeyType>>
        {
            constexpr static bool value = true;
        };

        template <typename KeyType, typename Hasher, bool UseViewAlternative>
        struct select_hasher_wrapper;

        template <has_key_view_alternative KeyType, template<typename> typename Hasher>
        struct select_hasher_wrapper<KeyType, Hasher<KeyType>, true>
        {
            using type = hasher_wrapper<typename KeyType::const_view_type, Hasher<typename KeyType::const_view_type>>;
        };

        template <typename KeyType, typename Hasher>
        struct select_hasher_wrapper<KeyType, Hasher, false>
        {
            using type = hasher_wrapper<KeyType, Hasher>;
        };

    } // namespace internal
}     // namespace rsl
