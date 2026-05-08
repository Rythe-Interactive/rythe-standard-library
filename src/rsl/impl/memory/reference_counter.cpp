#include "reference_counter.hpp"

namespace rsl
{
    void manual_reference_counter::reset() noexcept
    {
        m_count = 0;
        this->on_reset();
    }
}
