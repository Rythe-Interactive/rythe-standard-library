#pragma once
#include "../rsl_core.hpp"

namespace rsl::internal
{
    template <typename Func>
    struct deferred_executor
    {
        template <typename F>
        deferred_executor(F&& func)
            : m_func(func)
        {}

        ~deferred_executor() { m_func(); }

    private:
        Func m_func;
    };

    template <typename Func>
    deferred_executor(Func&& func) -> deferred_executor<remove_cvr_t<Func>>;
} // namespace rsl::internal

#define rythe_defer_execution rsl::internal::deferred_executor RYTHE_ANONYMOUS_NAME(deferredExecutionAtLine) = [&]()
