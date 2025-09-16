#define RYTHE_VALIDATE

#include <rsl/memory>

#include <catch2/catch_test_macros.hpp>

namespace
{
	struct alignas(16) test_struct
	{
		rsl::f32 x, y, z;
		rsl::byte _padding[4];
	};
} // namespace

TEST_CASE("memory resource", "[memory]") {}

TEST_CASE("reference counter", "[memory]")
{
	SECTION("manual reference counter")
	{
		rsl::manual_reference_counter refCounter;

		REQUIRE(refCounter.count() == 0);
		REQUIRE(!refCounter.is_occupied());
		REQUIRE(refCounter.is_free());

		rsl::size_type index = refCounter.borrow();

		REQUIRE(index == 0);
		REQUIRE(refCounter.count() == 1);
		REQUIRE(refCounter.is_occupied());
		REQUIRE(!refCounter.is_free());

		rsl::size_type index2 = refCounter.borrow();

		REQUIRE(index2 == 1);
		REQUIRE(refCounter.count() == 2);
		REQUIRE(refCounter.is_occupied());
		REQUIRE(!refCounter.is_free());

		refCounter.release();

		for (rsl::size_type i = 0; i < 135; i++)
		{
			refCounter.borrow();
			REQUIRE(refCounter.count() == (2 + i));
		}

		REQUIRE(refCounter.count() == 136);
		REQUIRE(refCounter.is_occupied());
		REQUIRE(!refCounter.is_free());

		index = refCounter.count();
		for (; index != 0; index--)
		{
			refCounter.release();
		}

		REQUIRE(refCounter.count() == 0);
		REQUIRE(!refCounter.is_occupied());
		REQUIRE(refCounter.is_free());
	}

	SECTION("basic reference counter")
	{
		SECTION("basic functionality and copy")
		{
			rsl::reference_counter refCounter;

			REQUIRE(!refCounter.is_armed());
			REQUIRE(refCounter.count() == 0);
			REQUIRE(!refCounter.is_occupied());
			REQUIRE(refCounter.is_free());

			refCounter.arm();

			REQUIRE(refCounter.is_armed());
			REQUIRE(refCounter.count() == 1);
			REQUIRE(!refCounter.is_occupied());
			REQUIRE(refCounter.is_free());

			rsl::size_type index = refCounter.borrow();

			REQUIRE(index == 1);
			REQUIRE(refCounter.count() == 2);
			REQUIRE(refCounter.is_occupied());
			REQUIRE(!refCounter.is_free());

			rsl::size_type index2 = refCounter.borrow();

			REQUIRE(index2 == 2);
			REQUIRE(refCounter.count() == 3);
			REQUIRE(refCounter.is_occupied());
			REQUIRE(!refCounter.is_free());

			refCounter.release();

			REQUIRE(refCounter.count() == 2);
			REQUIRE(refCounter.is_occupied());
			REQUIRE(!refCounter.is_free());

			{
				rsl::reference_counter cpy = refCounter;

				REQUIRE(refCounter.count() == 3);
				REQUIRE(cpy.count() == 3);

				REQUIRE(refCounter.is_occupied());
				REQUIRE(!refCounter.is_free());
				REQUIRE(cpy.is_occupied());
				REQUIRE(!cpy.is_free());

				cpy.release();

				REQUIRE(refCounter.count() == 2);
				REQUIRE(cpy.count() == 2);

				REQUIRE(refCounter.is_occupied());
				REQUIRE(!refCounter.is_free());
				REQUIRE(cpy.is_occupied());
				REQUIRE(!cpy.is_free());
			}

			REQUIRE(refCounter.count() == 1);
			REQUIRE(!refCounter.is_occupied());
			REQUIRE(refCounter.is_free());

			REQUIRE(refCounter.is_armed());

			refCounter.disarm();

			REQUIRE(!refCounter.is_armed());
			REQUIRE(refCounter.count() == 0);
			REQUIRE(!refCounter.is_occupied());
			REQUIRE(refCounter.is_free());

			refCounter.arm();

			REQUIRE(refCounter.is_armed());
			REQUIRE(refCounter.count() == 1);
			REQUIRE(!refCounter.is_occupied());
			REQUIRE(refCounter.is_free());

			rsl::reference_counter tmp = refCounter;

			REQUIRE(refCounter.count() == 2);
			REQUIRE(refCounter.is_occupied());
			REQUIRE(!refCounter.is_free());

			REQUIRE(refCounter.is_armed());

			refCounter.disarm();

			REQUIRE(!refCounter.is_armed());
			REQUIRE(refCounter.count() == 0);
			REQUIRE(!refCounter.is_occupied());
			REQUIRE(refCounter.is_free());
		}

		SECTION("move")
		{
			rsl::reference_counter refCounter(rsl::arm_signal);
			REQUIRE(refCounter.count() == 1);
			REQUIRE(refCounter.is_armed());

			rsl::reference_counter cpy;
			REQUIRE(cpy.count() == 0);
			REQUIRE(!cpy.is_armed());

			cpy.arm();

			REQUIRE(cpy.is_armed());
			REQUIRE(cpy.count() == 1);

			refCounter.disarm();
			REQUIRE(!refCounter.is_armed());
			REQUIRE(refCounter.count() == 0);
			cpy.disarm();
			REQUIRE(!cpy.is_armed());
			REQUIRE(cpy.count() == 0);

			{
				rsl::reference_counter moveSrc(rsl::arm_signal);

				REQUIRE(moveSrc.count() == 1);
				REQUIRE(!moveSrc.is_occupied());
				REQUIRE(moveSrc.is_free());

				cpy = moveSrc;
				REQUIRE(moveSrc.count() == 2);
				REQUIRE(cpy.count() == 2);

				refCounter = std::move(moveSrc);

				REQUIRE(!moveSrc.is_armed());
				REQUIRE(moveSrc.count() == 0);
				REQUIRE(!moveSrc.is_occupied());
				REQUIRE(moveSrc.is_free());

				REQUIRE(refCounter.is_armed());
				REQUIRE(cpy.count() == 2);
				REQUIRE(refCounter.count() == 2);
				REQUIRE(refCounter.is_occupied());
				REQUIRE(!refCounter.is_free());
			}

			REQUIRE(refCounter.is_armed());
			REQUIRE(cpy.count() == 2);
			REQUIRE(refCounter.count() == 2);
			REQUIRE(refCounter.is_occupied());
			REQUIRE(!refCounter.is_free());

			cpy.disarm();
			REQUIRE(!cpy.is_armed());
			REQUIRE(cpy.count() == 0);
			REQUIRE(!cpy.is_occupied());
			REQUIRE(cpy.is_free());

			REQUIRE(refCounter.count() == 1);
			REQUIRE(refCounter.is_armed());
			REQUIRE(!refCounter.is_occupied());
			REQUIRE(refCounter.is_free());

			refCounter.disarm();
			REQUIRE(!refCounter.is_armed());
			REQUIRE(refCounter.count() == 0);
			REQUIRE(!refCounter.is_occupied());
			REQUIRE(refCounter.is_free());
		}
	}
}

TEST_CASE("managed resource", "[memory]")
{
	SECTION("construction") {}

	SECTION("reference counting") {}
}

TEST_CASE("memory pool", "[memory]")
{
	SECTION("construction")
	{
		rsl::memory_pool<test_struct> pool;

		REQUIRE(pool.capacity() == 0);
	}

	SECTION("allocation & reset")
	{
		rsl::memory_pool<test_struct> pool;

		REQUIRE(pool.capacity() == 0);
		REQUIRE(pool.size() == 0);

		auto* ptr = pool.allocate();

		REQUIRE(ptr);
		REQUIRE(
			pool.capacity() ==
			rsl::memory_pool<test_struct>::get_element_count(rsl::memory_pool<test_struct>::minimumBlockSize)
		);
		REQUIRE(pool.size() == 1);

		pool.reset();
		REQUIRE(pool.capacity() == 0);
		REQUIRE(pool.size() == 0);
	}

	SECTION("deallocation")
	{
		rsl::memory_pool<test_struct> pool;

		REQUIRE(pool.capacity() == 0);
		REQUIRE(pool.size() == 0);

		auto* ptr = pool.allocate();

		REQUIRE(ptr);
		REQUIRE(
			pool.capacity() ==
			rsl::memory_pool<test_struct>::get_element_count(rsl::memory_pool<test_struct>::minimumBlockSize)
		);
		REQUIRE(pool.size() == 1);

		pool.deallocate(ptr);

		REQUIRE(
			pool.capacity() ==
			rsl::memory_pool<test_struct>::get_element_count(rsl::memory_pool<test_struct>::minimumBlockSize)
		);
		REQUIRE(pool.size() == 0);

		pool.reset();
		REQUIRE(pool.capacity() == 0);
		REQUIRE(pool.size() == 0);
	}

	SECTION("external memory blocks") {}
}
