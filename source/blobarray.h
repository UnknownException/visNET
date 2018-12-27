#pragma once

/*
	Binary Large OBject
	
	With a BlobArray you can efficiently send multiple objects of the same type.
	It is also recommended to use if you want to send one single instance of an object.
*/

// Changes memory allocation to strict (Less usage, more reallocations)
//#define _visNET_STRICT_REALLOC

#ifndef _visNET_STRICT_REALLOC
#define _visNET_LOOSE_REALLOC 1.1f // Allocate 10% above required
#endif

namespace visNET{
	template <class T>
	class BlobArray{
		uint32_t m_nItemSize;
		uint32_t m_nItemCount;
		uint8_t* m_pData;
#ifndef _visNET_STRICT_REALLOC
		uint32_t m_nItemLimit;
#endif

	public:
		BlobArray(){
			m_nItemSize = sizeof(T);
			m_nItemCount = 0;

#ifdef _visNET_STRICT_REALLOC
			m_pData = new uint8_t[m_nItemSize];
#else
			m_nItemLimit = 10; // Initialize with 10 open slots
			m_pData = new uint8_t[m_nItemSize * m_nItemLimit];
#endif 
		}

		virtual ~BlobArray(){
			if (m_pData)
				delete[] m_pData;
		}

		void add(T* item){
			add(item, 1);
		}

		void add(T item) {
			add(&item, 1);
		}

		void add(T* items, uint32_t itemCount)
		{
#ifdef _visNET_STRICT_REALLOC
			if (m_nItemCount != 0 || itemCount > 1)
				m_pData = (uint8_t*)extendsize(m_pData, m_nItemCount * m_nItemSize, (m_nItemCount + itemCount) * m_nItemSize);
#else
			uint32_t difference = 0;
			if (m_nItemCount + itemCount > m_nItemLimit)
				difference = (m_nItemCount + itemCount) - m_nItemLimit;

			if (difference > 0)
			{
				uint32_t oldLimit = m_nItemLimit;

				m_nItemLimit = m_nItemCount + difference; // Make room for the additional blobs
				m_nItemLimit = static_cast<uint32_t>((m_nItemLimit + 10) * _visNET_LOOSE_REALLOC); // Add 10 slots, and an additional percentage of the current slots

				m_pData = static_cast<uint8_t*>(extendsize(m_pData, oldLimit * m_nItemSize, m_nItemLimit * m_nItemSize));
			}
#endif

			memcpy(m_pData + (m_nItemSize * m_nItemCount), reinterpret_cast<uint8_t*>(items), m_nItemSize * itemCount);
			m_nItemCount += itemCount;
		}

		T* get(uint32_t index){
			if (m_nItemCount == 0 || index > m_nItemCount - 1)
				return nullptr;

			return reinterpret_cast<T*>(m_pData + (m_nItemSize * index));
		}

		uint32_t getSize() { return m_nItemSize * m_nItemCount; }
		uint32_t getItemSize() { return m_nItemSize; }
		uint32_t getItemCount() { return m_nItemCount; }

	private:
		// Temporary fix
		void* extendsize(void* block, uint32_t size, uint32_t requested)
		{
			if (requested < size)
				return nullptr;

			uint8_t* ptr = new uint8_t[requested];
			memset(ptr, 0, requested);

			memcpy(ptr, block, size);
			delete[] static_cast<uint8_t*>(block);

			return ptr;
		}
	};
}

#ifdef _visNET_STRICT_REALLOC
	#undef _visNET_STRICT_REALLOC
#endif

#ifdef _visNET_LOOSE_REALLOC
	#undef _visNET_LOOSE_REALLOC
#endif