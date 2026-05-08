#pragma once
#include "predefined.hpp"
#include "swizzle/swizzle_base.hpp"
#include "vector_additions.hpp"
#include "vector_base.hpp"

#include "vector1.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"

#include "swizzle/swizzle.inl"
#include "vector1.inl"
#include "vector2.inl"
#include "vector3.inl"
#include "vector4.inl"
#include "vector_additions.inl"
#include "vector_base.inl"

#ifdef RYTHE_PCH
namespace rsl::math
{
    template struct vector<float32, 1>;
    template struct vector<float64, 1>;
    template struct vector<int32, 1>;
    template struct vector<uint32, 1>;
    template struct vector<bool, 1>;

    template struct vector<float32, 2>;
    template struct vector<float64, 2>;
    template struct vector<int32, 2>;
    template struct vector<uint32, 2>;
    template struct vector<bool, 2>;

    template struct vector<float32, 3>;
    template struct vector<float64, 3>;
    template struct vector<int32, 3>;
    template struct vector<uint32, 3>;
    template struct vector<bool, 3>;

    template struct vector<float32, 4>;
    template struct vector<float64, 4>;
    template struct vector<int32, 4>;
    template struct vector<uint32, 4>;
    template struct vector<bool, 4>;
} // namespace rsl::math
#endif
