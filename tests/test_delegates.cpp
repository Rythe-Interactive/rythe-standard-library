#define RYTHE_VALIDATE

#include <rsl/containers>
#include <rsl/delegate>
#include <rsl/primitives>

#include <catch2/catch_test_macros.hpp>

namespace
{
	rsl::size_type counter;

	void func()
	{
		counter++;
	}

	void func2()
	{
		counter *= 2;
	}

	void funcParam(rsl::uint32& i)
	{
		counter += i;
		i++;
	}

	void funcParam2(rsl::uint32& i)
	{
		counter += i;
		i *= 2;
	}

	struct Object
	{
		rsl::dynamic_array<rsl::size_type> ints;

		void memberFunc()
		{
			for (rsl::size_type& i : ints)
			{
				i++;
				counter += i;
			}
		}

		void memberFunc2()
		{
			for (rsl::size_type& i : ints)
			{
				i += 2;
			}
		}

		void constMemberFunc() const
		{
			for (rsl::size_type i : ints)
			{
				counter += i;
			}
		}

		void constMemberFunc2() const
		{
			for (rsl::size_type i : ints)
			{
				counter += i - 2;
			}
		}
	};

	rsl::dynamic_array<rsl::size_type> createVec(rsl::size_type idx)
	{
		rsl::dynamic_array<rsl::size_type> vec;
		for (rsl::size_type i = idx + 20; i > idx; i--)
		{
			vec.push_back(i);
		}
		return vec;
	}

	template <typename delegate_type, typename delegate_type2>
	void test_delegate_type()
	{
		delegate_type2 del2;

		CHECK(del2.empty());
		CHECK(del2 == nullptr);

		counter = 0;

		{
			del2 = &funcParam;
		}

		CHECK(!del2.empty());
		CHECK(del2 != nullptr);
		CHECK(counter == 0);

		{
			rsl::uint32 v = 3u;
			del2(v);
			CHECK(counter == 3);
			CHECK(v == 4);
		}

		counter = 0;

		delegate_type del;

		CHECK(del.empty());
		CHECK(del == nullptr);

		{
			del = &func;
		}

		CHECK(!del.empty());
		CHECK(del != nullptr);
		CHECK(counter == 0);

		del();

		CHECK(counter == 1);

		del.clear();
		CHECK(del.empty());
		CHECK(del == nullptr);

		counter = 0;

		{
			del = []() { counter++; };
		}

		CHECK(!del.empty());
		CHECK(counter == 0);

		del();

		CHECK(counter == 1);

		counter = 0;

		{
			rsl::dynamic_array<size_t> ints = createVec(100);
			del = [ints]()
			{
				for (size_t i : ints)
				{
					i++;
					counter += i;
				}
			};
		}

		CHECK(counter == 0);

		del();

		CHECK(counter == 2230);

		counter = 0;

		{
			Object obj{createVec(100)};
			del.template assign<Object, &Object::memberFunc>(obj);

			CHECK(counter == 0);

			del();

			CHECK(counter == 2230);
		}

		counter = 0;

		{
			const Object obj{createVec(100)};
			del.template assign<Object, &Object::constMemberFunc>(obj);

			CHECK(counter == 0);

			del();

			CHECK(counter == 2210);

			del();

			CHECK(counter == 4420);
		}

		counter = 0;
	}

#define EXECUTE_SHARED_DELEGATE_TESTS(delegateType)                                                                    \
	test_delegate_type<delegateType<void()>, delegateType<void(rsl::uint32&)>>()

} // namespace

TEST_CASE("delegate", "[delegates]")
{
	EXECUTE_SHARED_DELEGATE_TESTS(rsl::delegate);
}

TEST_CASE("multicast_delegate", "[delegates]")
{
	SECTION("Shared delegate tests")
	{
		EXECUTE_SHARED_DELEGATE_TESTS(rsl::multicast_delegate);
	}

	SECTION("Multicast delegate specific tests")
	{

		rsl::multicast_delegate<void(rsl::uint32&)> del2;

		CHECK(del2.empty());
		CHECK(del2 == nullptr);
		CHECK(del2.size() == 0);

		counter = 0;

		{
			del2 += &funcParam;
		}

		CHECK(del2.size() == 1);

		{
			del2 += &funcParam2;
		}

		CHECK(del2.size() == 2);

		CHECK(!del2.empty());
		CHECK(del2 != nullptr);
		CHECK(counter == 0);

		{
			rsl::uint32 v = 3u;
			del2(v);
			CHECK(counter == 7);
			CHECK(v == 8);
			del2(v);
			CHECK(counter == 24);
			CHECK(v == 18);
			del2(v);
			CHECK(counter == 61);
			CHECK(v == 38);
		}

		del2.remove(funcParam);
		CHECK(!del2.empty());
		CHECK(del2 != nullptr);
		CHECK(del2.size() == 1);

		del2.remove(funcParam);
		CHECK(!del2.empty());
		CHECK(del2 != nullptr);
		CHECK(del2.size() == 1);

		del2 -= funcParam2;
		CHECK(del2.empty());
		CHECK(del2 == nullptr);
		CHECK(del2.size() == 0);

		counter = 0;

		rsl::multicast_delegate<void()> del;

		{
			del += &func;
		}
		{
			del += &func2;
		}

		CHECK(counter == 0);

		del();
		CHECK(counter == 2);
		del();
		CHECK(counter == 6);
		del();
		CHECK(counter == 14);

		del.clear();
		CHECK(del.empty());
		CHECK(del == nullptr);
		CHECK(del.size() == 0);

		counter = 0;

		{
			del = []() { counter++; };
		}
		{
			auto lambda = []() { counter *= 3; };
			del += lambda;

			CHECK(!del.empty());
			CHECK(del != nullptr);
			CHECK(del.size() == 2);

			CHECK(counter == 0);

			del();
			CHECK(counter == 3);
			del();
			CHECK(counter == 12);
			del();
			CHECK(counter == 39);

			del -= lambda;

			CHECK(!del.empty());
			CHECK(del != nullptr);
			CHECK(del.size() == 1);
			CHECK(!del.contains(lambda));
		}

		del.clear();

		CHECK(del.empty());
		CHECK(del == nullptr);
		CHECK(del.size() == 0);

		counter = 0;

		{
			rsl::dynamic_array<size_t> ints = createVec(100);
			del = [&ints]()
			{
				for (size_t i : ints)
				{
					counter += i;
				}
			};
			del += [&ints]()
			{
				for (size_t& i : ints)
				{
					i++;
				}
			};

			CHECK(counter == 0);

			del();
			CHECK(counter == 2210);
			del();
			CHECK(counter == 4440);
			del();
			CHECK(counter == 6690);
		}

		counter = 0;

		{
			Object obj{createVec(100)};
			del.assign<Object, &Object::memberFunc>(obj);
			del.push_back<Object, &Object::memberFunc2>(obj);

			CHECK(counter == 0);

			del();
			CHECK(counter == 2230);
			del();
			CHECK(counter == 4520);
			del();
			CHECK(counter == 6870);
			del();
			CHECK(counter == 9280);

			del.pop_back();

			CHECK(!del.empty());
			CHECK(del != nullptr);
			CHECK(del.size() == 1);
			CHECK(del.contains<Object, &Object::memberFunc>(obj));
			CHECK(!del.contains<Object, &Object::memberFunc2>(obj));

			del.pop_back();

			CHECK(del.empty());
			CHECK(del == nullptr);
			CHECK(del.size() == 0);
			CHECK(!del.contains<Object, &Object::memberFunc>(obj));
			CHECK(!del.contains<Object, &Object::memberFunc2>(obj));
		}

		counter = 0;

		{
			const Object obj{createVec(100)};
			del.assign<Object, &Object::constMemberFunc>(obj);
			del.push_back<Object, &Object::constMemberFunc2>(obj);

			CHECK(counter == 0);

			del();
			CHECK(counter == 4380);
			del();
			CHECK(counter == 8760);
		}

		counter = 0;
	}
}
