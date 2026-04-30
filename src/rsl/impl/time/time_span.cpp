#include "time_span.hpp"

namespace rsl
{
    const time_span time_span::zero{ 0 };

    const time_span time_span::infinity{ math::limits<int64>::max };
}
