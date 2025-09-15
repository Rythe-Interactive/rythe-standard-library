#define RYTHE_VALIDATE

#include <rsl/heap_allocator>

namespace
{
	class test_heap_allocator : private rsl::heap_allocator
	{
	public:
		using value_type = void;
		rsl::id_type id = 1012234;

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

#include <rsl/containers>

#include <catch2/catch_test_macros.hpp>

namespace
{
	constexpr int const1 = 123456;
	// constexpr int const2 = 234567;
	constexpr int const3 = 345678;
	constexpr int const4 = 456789;
	constexpr int const5 = 567891;
	constexpr int const6 = 678912;

	constexpr float key1 = 1.23456f;
	constexpr float key2 = 2.34567f;
	constexpr float key3 = 3.45678f;
	constexpr float key4 = 4.56789f;
	// constexpr float key5 = 5.67891f;
	// constexpr float key6 = 6.78912f;

	struct test_struct
	{
		int value = 0;
		test_struct() = default;
		test_struct(const int i)
			: value(i)
		{
		}
	};
} // namespace

TEST_CASE("dynamic_map", "[containers]")
{
	using namespace rsl;

	SECTION("construction")
	{
		{
			rsl::dynamic_map<float32, test_struct> map{};
			CHECK(map.get_allocator().id == 1012234);
		}
		{
			test_heap_allocator alloc{1234};
			rsl::dynamic_map<float32, test_struct> map{alloc};
			CHECK(map.get_allocator().id == 1234);
		}
		{
			default_pmu_allocator alloc;
			allocator_storage<polymorphic_allocator> store(&alloc);
			rsl::dynamic_map<float32, test_struct, hash_map_flags::default_flags, polymorphic_allocator> map{store};
			CHECK((&(map.get_allocator())) == &alloc);
		}
	}

	SECTION("emplace")
	{
		rsl::dynamic_map<float32, test_struct> map{};

		{
			auto [result, emplaced] = map.try_emplace(key1, const1);
			CHECK(result.value == const1);
			CHECK(emplaced);
		}

		CHECK(map.contains(key1));
		CHECK(!map.contains(key2));
		CHECK(!map.contains(key3));
		CHECK(!map.contains(key4));

		{
			auto [result, emplaced] = map.try_emplace(key1, const1);
			CHECK(result.value == const1);
			CHECK(!emplaced);
		}

		CHECK(map.contains(key1));
		CHECK(!map.contains(key2));
		CHECK(!map.contains(key3));
		CHECK(!map.contains(key4));

		{
			auto [result, emplaced] = map.try_emplace(key2, const6);
			CHECK(result.value == const6);
			CHECK(emplaced);
		}

		CHECK(map.contains(key1));
		CHECK(map.contains(key2));
		CHECK(!map.contains(key3));
		CHECK(!map.contains(key4));

		{
			auto [result, emplaced] = map.try_emplace(key2, const5);
			CHECK(result.value == const6);
			CHECK(!emplaced);
		}

		CHECK(map.contains(key1));
		CHECK(map.contains(key2));
		CHECK(!map.contains(key3));
		CHECK(!map.contains(key4));

		{
			test_struct& result = map.emplace(key3, const3);
			CHECK(result.value == const3);
		}

		CHECK(map.contains(key1));
		CHECK(map.contains(key2));
		CHECK(map.contains(key3));
		CHECK(!map.contains(key4));

		{
			test_struct& result = map.emplace(key3, const4);
			CHECK(result.value == const3);
		}

		CHECK(map.contains(key1));
		CHECK(map.contains(key2));
		CHECK(map.contains(key3));
		CHECK(!map.contains(key4));

		{
			test_struct& result = map.emplace_or_replace(key3, const4);
			CHECK(result.value == const4);
		}

		CHECK(map.contains(key1));
		CHECK(map.contains(key2));
		CHECK(map.contains(key3));
		CHECK(!map.contains(key4));

		{
			test_struct& result = map.emplace(key4, const5);
			CHECK(result.value == const5);
		}

		CHECK(map.contains(key1));
		CHECK(map.contains(key2));
		CHECK(map.contains(key3));
		CHECK(map.contains(key4));

		map.clear();

		CHECK(!map.contains(key1));
		CHECK(!map.contains(key2));
		CHECK(!map.contains(key3));
		CHECK(!map.contains(key4));
	}

	SECTION("find") {}
	SECTION("get") {}
	SECTION("erase") {}
	SECTION("get_or_emplace") {}
	SECTION("emplace_or_replace") {}

    SECTION("view alternative access")
	{
		rsl::dynamic_map<dynamic_string, int32> map{};

	    map.emplace("Hi"_ds, 4);
	    map.emplace("Hey"_ds, 7);
	    map.emplace("Hello"_ds, 11);
	    map.emplace("Bye"_ds, 13);
	    map.emplace("Cioa"_ds, 5);
	    map.emplace("Cya"_ds, 9);

	    CHECK(map.contains("Hi"_sv));
	    {
		    int32* result = map.find("Hi"_sv);
		    REQUIRE(result != nullptr);
		    CHECK(*result == 4);
	    }

	    CHECK(map.contains("Hey"_sv));
	    {
		    int32* result = map.find("Hey"_sv);
		    REQUIRE(result != nullptr);
		    CHECK(*result == 7);
	    }

	    CHECK(map.contains("Hello"_sv));
	    {
		    int32* result = map.find("Hello"_sv);
		    REQUIRE(result != nullptr);
		    CHECK(*result == 11);
	    }

	    CHECK(map.contains("Bye"_sv));
	    {
		    int32* result = map.find("Bye"_sv);
		    REQUIRE(result != nullptr);
		    CHECK(*result == 13);
	    }

	    CHECK(map.contains("Cioa"_sv));
	    {
		    int32* result = map.find("Cioa"_sv);
		    REQUIRE(result != nullptr);
		    CHECK(*result == 5);
	    }

	    CHECK(map.contains("Cya"_sv));
	    {
		    int32* result = map.find("Cya"_sv);
		    REQUIRE(result != nullptr);
		    CHECK(*result == 9);
	    }
	}
}
