#pragma once

namespace rsl
{
    struct file_traits
    {
        bool isFile{};
        bool isDirectory{};
        bool isValidPath{};

        bool canBeWritten{};
        bool canBeRead{};
        bool canBeCreated{};

        bool exists{};
    };

    constexpr static file_traits invalid_file_traits{ false, false, false, false, false, false, false };

    struct filesystem_traits
    {
        bool isReadonly{};
        bool isValid{};
    };

    constexpr static filesystem_traits invalid_filesystem_traits{ false, false };
}
