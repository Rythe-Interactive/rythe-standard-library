#pragma once

#include "../../rsl_core.hpp"
#include "../../memory/unique_object.hpp"

#include "random_number_engine.hpp"

namespace rsl::math
{
    class random_number_generator
    {
        static unique_object<random_number_engine_base> m_engine;

    public:
        template <
            typename T, typename... Args,
            std::enable_if_t<std::is_base_of_v<random_number_engine_base, T>, bool> = true>
        static void set_rng_engine(Args&&... args);

        static void set_rng_engine(unique_object<random_number_engine_base>&& newEngine) noexcept;

        static void set_seed(size_type seed) noexcept;

        static void set_seed(std::seed_seq& seeds) noexcept;

        static void discard(size_type n) noexcept;

        template <typename Integer = int32>
        [[nodiscard]] static Integer generate_random_integer() noexcept;

        template <typename Float = float32>
        [[nodiscard]] static Float generate_random_float() noexcept;

        [[nodiscard]] static uint_max generate() noexcept;
    };
} // namespace rsl::math

#include "randomnumbergenerator.inl"
