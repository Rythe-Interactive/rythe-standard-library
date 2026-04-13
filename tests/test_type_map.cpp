#define RYTHE_VALIDATE

#include <rsl/allocator>

namespace
{
	class test_heap_allocator : public rsl::heap_allocator
	{
	public:
		using value_type = void;
		rsl::id_type id = 1012234;

		using rsl::heap_allocator::heap_allocator;
		explicit constexpr test_heap_allocator(rsl::id_type _id) noexcept
			: id(_id)
		{
		}

		using rsl::heap_allocator::allocate;
		using rsl::heap_allocator::deallocate;
		using rsl::heap_allocator::reallocate;
		using rsl::heap_allocator::is_valid;
	};
} // namespace

#define RSL_DEFAULT_ALLOCATOR_OVERRIDE test_heap_allocator

#include <rsl/type_map>

#include <catch2/catch_test_macros.hpp>

namespace
{
	constexpr int CONST1 = 123456;
	constexpr int CONST2 = 234567;
	constexpr int CONST3 = 345678;
	constexpr int CONST4 = 456789;
	constexpr int CONST5 = 567891;
	constexpr int CONST6 = 678912;

	struct test1
	{
		int value = CONST1;
		test1() = default;
		test1(int i)
			: value(i)
		{
		}
	};

	struct test2
	{
		int value = CONST2;
		test2() = default;
		test2(int i)
			: value(i)
		{
		}
	};

	struct test3
	{
		int value = CONST3;
		test3() = default;
		test3(int i)
			: value(i)
		{
		}
	};

	struct test4
	{
		int value = CONST4;
		test4() = default;
		test4(int i)
			: value(i)
		{
		}
	};
} // namespace

TEST_CASE("type_map", "[containers]")
{
	using namespace rsl;
	{
		constexpr auto name = type_name<test1>();
		REQUIRE(name == "test1");

		constexpr id_type id = type_id<test1>();
		REQUIRE(id != 0u);
	}
	SECTION("construction")
	{
		{
            rsl::type_map map{};
            CHECK(static_cast<test_heap_allocator&>(map.get_allocator()).id == 1012234_id);
		}
		{
			test_heap_allocator alloc{1234};
            rsl::basic_type_map<> map{ { &alloc } };
            CHECK(static_cast<test_heap_allocator&>(map.get_allocator()).id == 1234_id);
		}
		{
			default_allocator alloc;
            allocator_storage store(&alloc);
			rsl::basic_type_map<> map{store};
			REQUIRE((&(map.get_allocator())) == &alloc);
		}
	}

	SECTION("emplace")
	{
		rsl::type_map map{};

		{
			auto [result, emplaced] = map.try_emplace<test1>();
			REQUIRE(result.value == CONST1);
			REQUIRE(emplaced);
		}

		REQUIRE(map.has<test1>());
		REQUIRE(!map.has<test2>());
		REQUIRE(!map.has<test3>());
		REQUIRE(!map.has<test4>());

		{
			auto [result, emplaced] = map.try_emplace<test1>();
			REQUIRE(result.value == CONST1);
			REQUIRE(!emplaced);
		}

		REQUIRE(map.has<test1>());
		REQUIRE(!map.has<test2>());
		REQUIRE(!map.has<test3>());
		REQUIRE(!map.has<test4>());

		{
			auto [result, emplaced] = map.try_emplace<test2>(CONST6);
			REQUIRE(result.value == CONST6);
			REQUIRE(emplaced);
		}

		REQUIRE(map.has<test1>());
		REQUIRE(map.has<test2>());
		REQUIRE(!map.has<test3>());
		REQUIRE(!map.has<test4>());

		{
			auto [result, emplaced] = map.try_emplace<test2>(CONST5);
			REQUIRE(result.value == CONST6);
			REQUIRE(!emplaced);
		}

		REQUIRE(map.has<test1>());
		REQUIRE(map.has<test2>());
		REQUIRE(!map.has<test3>());
		REQUIRE(!map.has<test4>());

		{
			test3& result = map.emplace<test3>();
			REQUIRE(result.value == CONST3);
		}

		REQUIRE(map.has<test1>());
		REQUIRE(map.has<test2>());
		REQUIRE(map.has<test3>());
		REQUIRE(!map.has<test4>());

		{
			test4& result = map.emplace<test4>(CONST5);
			REQUIRE(result.value == CONST5);
		}

		REQUIRE(map.has<test1>());
		REQUIRE(map.has<test2>());
		REQUIRE(map.has<test3>());
		REQUIRE(map.has<test4>());

		map.clear();

		REQUIRE(!map.has<test1>());
		REQUIRE(!map.has<test2>());
		REQUIRE(!map.has<test3>());
		REQUIRE(!map.has<test4>());
	}

	SECTION("get") {}
	SECTION("erase") {}
	SECTION("get_or_emplace") {}
	SECTION("emplace_or_replace") {}
}
