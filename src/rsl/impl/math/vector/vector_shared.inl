#ifndef SIZE_SPECIALIZATION
	#define SIZE_SPECIALIZATION Size
#endif // !SIZE_SPECIALIZATION

#ifndef SCALAR_SPECIALIZATION
	#define SCALAR_SPECIALIZATION Scalar
#endif // !SCALAR_SPECIALIZATION

#ifndef MODE_SPECIALIZATION
	#define MODE_SPECIALIZATION Mode
#endif // !MODE_SPECIALIZATION

#ifndef VECTOR_TEMPLATE
	#define VECTOR_TEMPLATE                                                                                            \
		template <                                                                                                     \
			arithmetic_type SCALAR_SPECIALIZATION, size_type SIZE_SPECIALIZATION, storage_mode MODE_SPECIALIZATION>
#endif // !VECTOR_TEMPLATE

namespace rsl::math
{
	VECTOR_TEMPLATE constexpr vector<SCALAR_SPECIALIZATION, SIZE_SPECIALIZATION, MODE_SPECIALIZATION>::vector() noexcept
		: data()
	{
		for (size_type i = 0; i < size; ++i)
		{
			data[i] = scalar(0);
		}
	}

	VECTOR_TEMPLATE constexpr vector<SCALAR_SPECIALIZATION, SIZE_SPECIALIZATION, MODE_SPECIALIZATION>::vector(scalar s
	) noexcept
		: data()
	{
		for (size_type i = 0; i < size; ++i)
		{
			data[i] = s;
		}
	}

	VECTOR_TEMPLATE template <typename VecType>
		requires not_same_as<SCALAR_SPECIALIZATION, typename VecType::scalar> || (VecType::size != SIZE_SPECIALIZATION)
	constexpr vector<SCALAR_SPECIALIZATION, SIZE_SPECIALIZATION, MODE_SPECIALIZATION>::vector(const VecType& other
	) noexcept
		: data()
	{
		if constexpr (size > VecType::size)
		{
			for (size_type i = 0; i < VecType::size; ++i)
			{
				data[i] = static_cast<scalar>(other.data[i]);
			}

			for (size_type i = VecType::size; i < size; ++i)
			{
				data[i] = static_cast<scalar>(0);
			}
		}
		else
		{
			for (size_type i = 0; i < size; ++i)
			{
				data[i] = static_cast<scalar>(other.data[i]);
			}
		}
	}

	VECTOR_TEMPLATE constexpr SCALAR_SPECIALIZATION&
	vector<SCALAR_SPECIALIZATION, SIZE_SPECIALIZATION, MODE_SPECIALIZATION>::operator[](size_type i) noexcept
	{
		rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
		return data[i];
	}

	VECTOR_TEMPLATE constexpr const SCALAR_SPECIALIZATION&
	vector<SCALAR_SPECIALIZATION, SIZE_SPECIALIZATION, MODE_SPECIALIZATION>::operator[](size_type i) const noexcept
	{
		rsl_assert_out_of_range_msg(i < size, "vector subscript out of range");
		return data[i];
	}
} // namespace rsl::math

#undef SIZE_SPECIALIZATION
#undef SCALAR_SPECIALIZATION
#undef VECTOR_TEMPLATE
