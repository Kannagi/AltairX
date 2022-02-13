#ifndef BUFFER_T_HPP
#define BUFFER_T_HPP 1

template <typename T>
class buffer_t
{
	T* m_ptr = nullptr;
	T* m_end = nullptr;

public:
	buffer_t() = default;
	buffer_t(const buffer_t&) = delete;
	buffer_t(buffer_t&& b) noexcept : m_ptr(b.m_ptr), m_end(b.m_end)
	{
		b.m_ptr = nullptr;
		b.m_end = nullptr;
	}
	buffer_t(uint64_t count) : m_ptr(new T[count]), m_end(m_ptr + count)
	{
		if (m_ptr == nullptr)
			throw "out of memory error";
	}
	~buffer_t()
	{
		delete m_ptr;
		m_ptr = nullptr;
		m_end = nullptr;
	}

	buffer_t& operator=(const buffer_t&) = delete;
	buffer_t& operator=(buffer_t&& b) noexcept
	{
		m_ptr = b.m_ptr;
		m_end = b.m_end;

		b.m_ptr = nullptr;
		b.m_end = nullptr;

		return *this;
	}

	uint64_t size() const noexcept { return m_end - m_ptr; }
	T* data() noexcept { return m_ptr; }
	const T* data() const noexcept { return m_ptr; }

	T* begin() noexcept { return m_ptr; }
	T* end() noexcept { return m_end; }

	const T* begin() const noexcept { return m_ptr; }
	const T* end() const noexcept { return m_end; }

	const T* cbegin() const noexcept { return m_ptr; }
	const T* cend() const noexcept { return m_end; }

	template <typename IntegerT>
	T& operator[](IntegerT i)
	{
		return m_ptr[i];
	}
};

#endif  // BUFFER_T_HPP
