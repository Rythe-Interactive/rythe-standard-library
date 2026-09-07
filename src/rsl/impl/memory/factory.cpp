#include "factory.hpp"

namespace rsl
{
    void* type_erased_factory::construct(void* ptr, const size_type count) const
    {
        return m_constructFunc ? (m_constructFunc)(ptr, count) : nullptr;
    }

    void* type_erased_factory::copy(void* dst, const void* src, const size_type count) const
    {
        return m_copyFunc ? (m_copyFunc)(dst, src, count) : nullptr;
    }

    void* type_erased_factory::move(void* dst, void* src, const size_type count) const
    {
        return m_moveFunc ? (m_moveFunc)(dst, src, count) : nullptr;
    }

    void type_erased_factory::destroy(void* ptr, const size_type count) const noexcept
    {
        rsl_assert_high_impact(m_destroyFunc);
        (m_destroyFunc)(ptr, count);
    }

    size_type type_erased_factory::type_size() const noexcept
    {
        return m_typeSize;
    }

    bool type_erased_factory::trivial_copy() const noexcept
    {
        return m_triviallyCopyable;
    }

    id_type type_erased_factory::type_id() const noexcept
    {
        return m_typeId;
    }

    bool type_erased_factory::is_valid() const noexcept
    {
        return m_constructFunc     != nullptr &&
               m_moveFunc          != nullptr &&
               m_destroyFunc       != nullptr &&
               m_typeSize          != 0       &&
               m_typeId            != invalid_id;

    }
} // namespace rsl
