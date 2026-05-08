#include "randomnumbergenerator.hpp"

namespace rsl::math
{
    unique_object<random_number_engine_base> random_number_generator::m_engine = unique_object<rythe_default_engine>::create_in_place();

    void random_number_generator::set_rng_engine(unique_object<random_number_engine_base>&& newEngine) noexcept
    {
        m_engine = move(newEngine);
    }

    void random_number_generator::set_seed(const size_type seed) noexcept
    {
        m_engine->seed(seed);
    }

    void random_number_generator::set_seed(std::seed_seq& seeds) noexcept
    {
        m_engine->seed(seeds);
    }

    void random_number_generator::discard(const size_type n) noexcept
    {
        m_engine->discard(n);
    }

    [[nodiscard]] uint_max random_number_generator::generate() noexcept
    {
#if defined(LEGION_VALIDATE)
        if (!engine)
        {
            return 0; // should report an error with improved error system down the line.
        }
#endif
        return m_engine->generate();
    }
} // namespace rsl::math
