#include "string.hpp"

namespace rsl
{
	namespace
	{
		size_type read_utf8(const utf8* readPos, uint32& output)
		{
			size_type width = 1u;
			uint32 character = static_cast<uint32>(*readPos);
			readPos++;

			if ((character & 0xe0u) == 0xc0u)
			{
				width = 2u;
				character &= 0x1fu;
			}
			else if ((character & 0xf0u) == 0xe0u)
			{
				width = 3u;
				character &= 0x0fu;
			}
			else if ((character & 0xf8u) == 0xf0u)
			{
				width = 4u;
				character &= 0x07u;
			}
			else if (character >= 128u)
			{
				return 1u;
			}

			for (size_t count = width - 1u; count > 0u; count--)
			{
				const utf8 currentChar = *readPos;
				if ((currentChar & 0xc0u) != 0x80u)
				{
					return width - count;
				}
				character = (character << 6u) | (currentChar & 0x3fu);
				readPos++;
			}

			output = character;
			return width;
		}

	    [[nodiscard]] bool write_utf8(dynamic_string& str, const uint32 character)
		{
            if (character < 0x80u)
            {
                str.push_back(static_cast<utf8>(character));
            }
            else if (character < 0x800u)
            {
                str.push_back(static_cast<utf8>((character >> 6u) & 31u) | 0xc0u);
                str.push_back(static_cast<utf8>(static_cast<utf8>(character & 63u) | 0x80u));
            }
            else if (character < 0x10000u)
            {
                str.push_back(static_cast<utf8>(static_cast<utf8>((character >> 12u) & 15u) | 0xe0u));
                str.push_back(static_cast<utf8>(static_cast<utf8>((character >> 6) & 63) | 0x80u));
                str.push_back(static_cast<utf8>(static_cast<utf8>(character & 63) | 0x80u));
            }
            else if (character < 0x200000u)
            {
                str.push_back(static_cast<utf8>(static_cast<utf8>((character >> 18u) & 7u) | 0xf0u));
                str.push_back(static_cast<utf8>(static_cast<utf8>((character >> 12u) & 63u) | 0x80u));
                str.push_back(static_cast<utf8>(static_cast<utf8>((character >> 6u) & 63u) | 0x80u));
                str.push_back(static_cast<utf8>(static_cast<utf8>(character & 63u) | 0x80u));
            }
            else
            {
                return false;
            }
            return true;
		}
	}

	dynamic_wstring to_utf16(const dynamic_string::const_view_type str)
	{
		dynamic_wstring result;
		result.reserve(str.size());

		const utf8* data = str.data();
		utf8 const * const end = data + str.size();

		while (data != end)
		{
			uint32 character = 0u;
			const size_type bytesRead = read_utf8(data, character);
			data += bytesRead;

			if (character >= 0x10000u)
			{
				character -= 0x10000u;
				result.push_back(static_cast<utf16>(((character >> 10u) & 0x3ffu) | 0xd800u));
				result.push_back(static_cast<utf16>((character & 0x3ffu) | 0xdc00u));
			}
			else
			{
				result.push_back(static_cast<utf16>(character));
			}
		}

		return result;
	}

    namespace internal
    {
        template<bool ReportErrors>
	    static dynamic_string to_utf8(const dynamic_wstring::const_view_type str, bool* succeeded)
        {
            dynamic_string result;
            result.reserve(str.size());

            const utf16* data = str.data();
            utf16 const * const end = data + str.size();
            while( data != end )
            {
                uint32 character = *data++;
                if( ( character & 0xf800u ) == 0xd800u )
                {
                    // surrogate pair
                    if constexpr (rythe_validate_low_impact)
                    {
                        if( ( character & 0xfc00u ) != 0xd800u || ( *data & 0xfc00u ) != 0xdc00u )
                        {
                            if constexpr (ReportErrors)
                            {
                                *succeeded = false;
                            }
                            return {};
                        }
                    }

                    character = ( ( character & 0x3ffu ) << 10u ) | ( *data++ & 0x3ffu ) | 0x10000u;
                }
                if (!write_utf8( result, character ))
                {
                    if constexpr (ReportErrors)
                    {
                        *succeeded = false;
                    }
                    return {};
                }
            }

            if constexpr (ReportErrors)
            {
                *succeeded = true;
            }

            return result;
        }
    }

    dynamic_string to_utf8(const dynamic_wstring::const_view_type str)
	{
	    return internal::to_utf8<false>(str, nullptr);
	}

    dynamic_string to_utf8(const dynamic_wstring::const_view_type str, bool& succeeded)
	{
	    return internal::to_utf8<true>(str, &succeeded);
	}
}
