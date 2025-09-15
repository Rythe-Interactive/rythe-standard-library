#include <catch2/catch_test_macros.hpp>

#define RYTHE_VALIDATE

#include <rsl/containers>

TEST_CASE("string","[containers]")
{
	using namespace rsl;
	SECTION("construction")
	{
		{//operator=
			rsl::dynamic_string s;
			s = "hello world"_ds;
			CHECK(!s.empty());
			CHECK(s.size() == 11);
		}

		{//copy constructor
			rsl::dynamic_string s = dynamic_string::from_array("hello world");
			rsl::dynamic_string s2(s);
			CHECK(!s.empty());
			CHECK(!s2.empty());
			CHECK(s.size() == s2.size());
			CHECK(s == s2);
		}

		{//capacity constructor
			rsl::dynamic_string s = dynamic_string::create_reserved(10);
			CHECK(s.empty());
			CHECK(s.capacity() == 10);
		}

		{//char array constructor
            constexpr char str[5] = {'h','e','l','l','o'};
			rsl::dynamic_string s = dynamic_string::from_array(str);
			CHECK(!s.empty());
			CHECK(s.capacity() == 5);
		}
	}

	SECTION("search")
	{
		constexpr string_view str = "abcdeffedcbaabcdeffedcba"_sv;

	    {
		    size_type offset = rsl::linear_search_sequence(str, "effed"_sv, 0);
		    CHECK(offset == 4);
		    CHECK(str[offset] == 'e');
	    }
	    {
		    size_type offset = rsl::reverse_linear_search_sequence(str, "effed"_sv, 0);
		    CHECK(offset == 16);
		    CHECK(str[offset] == 'e');
	    }
	    {
		    size_type offset = rsl::linear_search_sequence(str, "effed"_sv, 10);
		    CHECK(offset == 16);
		    CHECK(str[offset] == 'e');
	    }
		{
			size_type offset = rsl::linear_search(str, 'd', 0);
			CHECK(offset == 3);
			CHECK(str[offset] == 'd');
		}
	    {
		    size_type offset = rsl::linear_search(str, 'd', 10);
		    CHECK(offset == 15);
		    CHECK(str[offset] == 'd');
	    }
		{
			size_type offset = rsl::linear_search(str, 'c', 0);
			CHECK(offset == 2);
			CHECK(str[offset] == 'c');
		}
		{
			size_type offset = rsl::linear_search(str, 'c', 5);
			CHECK(offset == 9);
			CHECK(str[offset] == 'c');
		}
		{
			size_type offset = rsl::linear_search_collection(str, "efb"_sv, 0);
			CHECK(offset == 1);
			CHECK(str[offset] == 'b');
		}

		{
			size_type offset = rsl::linear_search_not_eq(str, 'd', 0);
			CHECK(offset == 0);
			CHECK(str[offset] == 'a');
		}
		{
			size_type offset = rsl::linear_search_not_eq(str, 'f', 5);
			CHECK(offset == 7);
			CHECK(str[offset] == 'e');
		}
		{
			size_type offset = rsl::linear_search_not_eq(str, 'a', 0);
			CHECK(offset == 1);
			CHECK(str[offset] == 'b');
		}
		{
			size_type offset = rsl::linear_search_outside_collection(str, "abcde"_sv, 0);
			CHECK(offset == 5);
			CHECK(str[offset] == 'f');
		}

		{
			size_type offset = rsl::reverse_linear_search(str, 'd', 0);
			CHECK(offset == 20);
			CHECK(str[offset] == 'd');
		}
		{
			size_type offset = rsl::reverse_linear_search(str, 'c', 0);
			CHECK(offset == 21);
			CHECK(str[offset] == 'c');
		}
		{
			size_type offset = rsl::reverse_linear_search_collection(str, "efb"_sv, 0);
			CHECK(offset == 22);
			CHECK(str[offset] == 'b');
		}

		{
			size_type offset = rsl::reverse_linear_search_not_eq(str, 'd', 0);
			CHECK(offset == 23);
			CHECK(str[offset] == 'a');
		}
		{
			size_type offset = rsl::reverse_linear_search_not_eq(str, 'a', 0);
			CHECK(offset == 22);
			CHECK(str[offset] == 'b');
		}
		{
			size_type offset = rsl::reverse_linear_search_outside_collection(str, "abcde"_sv, 0);
			CHECK(offset == 18);
			CHECK(str[offset] == 'f');
		}
	}

    SECTION("hashed string")
	{
	    {//operator=
	        rsl::hashed_string s;
	        s = "hello world"_hs;
	        CHECK(!s.empty());
	        CHECK(s.size() == 11);
	        CHECK(s.hash == 17498481775468162579ull);
	    }

	    {//copy constructor
	        rsl::hashed_string s = hashed_string::from_array("hello world");
	        rsl::hashed_string s2(s);
	        CHECK(!s.empty());
	        CHECK(!s2.empty());
	        CHECK(s.size() == s2.size());
	        CHECK(s == s2);
	        CHECK(s.hash == s2.hash);
	        CHECK(s.hash == 17498481775468162579ull);
	    }

	    {//capacity constructor
	        rsl::hashed_string s = hashed_string::create_reserved(10);
	        CHECK(s.empty());
	        CHECK(s.capacity() == 10);
	        CHECK(s.hash == 0);
	    }

	    {//char array constructor
            constexpr char str[5] = {'h','e','l','l','o'};
	        rsl::hashed_string s = hashed_string::from_array(str);
	        CHECK(!s.empty());
	        CHECK(s.capacity() == 5);
	        CHECK(s.hash == 2188375479838694330ull);
	    }
	}
}
