#pragma once

namespace rsl
{
    template <typename T>
    T dynamic_library::get_symbol(const cstring symbolName) const
    {
        return bit_cast<T>(platform::get_symbol(*this, symbolName));
    }

    inline void dynamic_library::release()
    {
        platform::release_library(*this);
        m_handle = RYTHE_DYNAMIC_LIBRARY_HANDLE_DEFAULT_VALUE;
    }
}
