
namespace rsl::rfl
{
    constexpr void reflection::add_registry(pointer<reflection_registry> reg)
    {
        m_registries.push_back(reg);
    }

    constexpr array_view<pointer<reflection_registry>> reflection::registries() noexcept
    {
        return m_registries;
    }
} // namespace rsl::rfl
