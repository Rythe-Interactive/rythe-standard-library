#pragma once
#include "../defines.hpp"

namespace rsl
{
#pragma region ////////////////////////////////// Special builtin attributes ///////////////////////////////////

    struct custom_attribute {};

    struct auto_add_attribute {};

    struct restrict_functions {};
    struct restrict_classes {};
    struct restrict_variables {};

    struct [[rsl_reflect(auto_add_attribute(restrict_functions))]] restrict_function_signature {};

#pragma endregion

    struct [[rsl_reflect(custom_attribute)]] dont_serialize {};
}
