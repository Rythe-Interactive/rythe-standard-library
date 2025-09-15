#define RYTHE_VALIDATE

#include <rsl/heap_allocator>

namespace
{
	class test_heap_allocator : private rsl::heap_allocator
	{
	public:
		using value_type = void;
		rsl::id_type id = 1012234ull;

		using rsl::heap_allocator::heap_allocator;
		explicit constexpr test_heap_allocator(const rsl::id_type _id) noexcept
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

#include <rsl/array>

#include <catch2/catch_test_macros.hpp>

namespace
{
	[[maybe_unused]] constexpr int CONST1 = 123456;
	[[maybe_unused]] constexpr int CONST2 = 234567;
	[[maybe_unused]] constexpr int CONST3 = 345678;
	[[maybe_unused]] constexpr int CONST4 = 456789;
	[[maybe_unused]] constexpr int CONST5 = 567891;
	[[maybe_unused]] constexpr int CONST6 = 678912;

	struct test1
	{
		int value = CONST1;
		test1() = default;
		test1(const int i)
			: value(i)
		{
		}
	};

	struct test2
	{
		int value = CONST2;
		test2() = default;
		test2(const int i)
			: value(i)
		{
		}
	};

	struct test3
	{
		int value = CONST3;
		test3() = default;
		test3(const int i)
			: value(i)
		{
		}
	};

	struct test4
	{
		int value = CONST4;
		test4() = default;
		test4(const int i)
			: value(i)
		{
		}
	};
} // namespace

TEST_CASE("dynamic_array", "[containers]")
{
	using namespace rsl;

	SECTION("construction")
	{
		{
			rsl::dynamic_array<int> list{};
			CHECK(list.get_allocator().id == 1012234_id);
		}
		{
			test_heap_allocator alloc{1234_id};
			rsl::dynamic_array<int, test_heap_allocator> list{alloc};
			CHECK(list.get_allocator().id == 1234_id);
		}
		{
			default_pmu_allocator alloc;
			allocator_storage<polymorphic_allocator> store(&alloc);
			rsl::dynamic_array<int, polymorphic_allocator> list{store};
			CHECK((&(list.get_allocator())) == &alloc);
		}
	}

	SECTION("reserve") {}
	SECTION("resize") {}
	SECTION("emplace") {}
	SECTION("copy/move") {}
}
