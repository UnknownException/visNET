#pragma once

/*
	Binary Large OBject

	With a BlobArray you can efficiently send multiple objects of the same type.
	It is also recommended to use if you want to send one single instance of an object.
*/

#include <type_traits>

namespace visNET {
	template <class T>
	class BlobArray {
		static_assert(std::is_pod<T>::value, "Generic for blobarray has to be POD!");
		std::vector<T> m_Data;

	public:
		BlobArray() {
		}

		virtual ~BlobArray() {
		}

		void add(const T& item) {
			m_Data.push_back(item);
		}

		void add(const T* items, uint32_t count)
		{
			int currentItemCount = m_Data.size();
			m_Data.resize(currentItemCount + count);

			memcpy(&m_Data[currentItemCount], items, sizeof(T) * count);
		}

		// Returns a copy of the item stored at the given index
		// Throws an exception if index is out of range
		T get(uint32_t index)
		{
			if (index >= m_Data.size())
				std::exception("Index out of range");

			return m_Data[index];
		}

		// Returns a pointer to the item stored at the given index
		// Unsafe: Only valid as long as blobarray is valid
		T* getPtr(uint32_t index)
		{
			if (index >= m_Data.size())
				return nullptr;

			return &m_Data[index];
		}

		// Returns a copy of the vector
		std::vector<T> get() { return m_Data; }

		// Returns a pointer to the vector
		// Unsafe: Only valid as long as blobarray is valid
		std::vector<T>* getPtr() { return &m_Data; }

		// Size of generic T
		uint32_t itemSize() { return sizeof(T); }
		// Amount of items stored in the array
		uint32_t size() { return m_Data.size(); }
		// Total size in bytes
		uint32_t dataSize() { return itemSize() * size(); }
	};
}
