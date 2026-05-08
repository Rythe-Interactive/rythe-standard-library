// swizzle 1
struct
{
    scalar x;
};
struct
{
    scalar u;
};
struct
{
    scalar r;
};
struct
{
    scalar s;
};

// swizzle 2
struct
{
    ::rsl::math::swizzle<scalar, 1, mode, 0, 0> xx;
};
struct
{
    ::rsl::math::swizzle<scalar, 1, mode, 0, 0> uu;
};
struct
{
    ::rsl::math::swizzle<scalar, 1, mode, 0, 0> rr;
};
struct
{
    ::rsl::math::swizzle<scalar, 1, mode, 0, 0> ss;
};

// swizzle 3
struct
{
    ::rsl::math::swizzle<scalar, 1, mode, 0, 0, 0> xxx;
};
struct
{
    ::rsl::math::swizzle<scalar, 1, mode, 0, 0, 0> uuu;
};
struct
{
    ::rsl::math::swizzle<scalar, 1, mode, 0, 0, 0> rrr;
};
struct
{
    ::rsl::math::swizzle<scalar, 1, mode, 0, 0, 0> sss;
};

// swizzle 4
struct
{
    ::rsl::math::swizzle<scalar, 1, mode, 0, 0, 0, 0> xxxx;
};
struct
{
    ::rsl::math::swizzle<scalar, 1, mode, 0, 0, 0, 0> uuuu;
};
struct
{
    ::rsl::math::swizzle<scalar, 1, mode, 0, 0, 0, 0> rrrr;
};
struct
{
    ::rsl::math::swizzle<scalar, 1, mode, 0, 0, 0, 0> ssss;
};
