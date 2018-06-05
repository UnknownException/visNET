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
		uint32_t m_nBlobSize;
		uint32_t m_nBlobCount;
		uint8_t* m_pData;
#ifndef _visNET_STRICT_REALLOC
		uint32_t m_nBlobLimit;
#endif

	public:
		BlobArray(){
			m_nBlobSize = sizeof(T);
			m_nBlobCount = 0;

#ifdef _visNET_STRICT_REALLOC
			m_pData = new uint8_t[m_nBlobSize];
#else
			m_nBlobLimit = 10; // Initialize with 10 open slots
			m_pData = new uint8_t[m_nBlobSize * m_nBlobLimit];
#endif 
		}

		virtual ~BlobArray(){
			if (m_pData)
				delete[] m_pData;
		}

		void add(T& item){
			add(&item, 1);
		}

		void add(T* items, uint32_t nCount)
		{
#ifdef _visNET_STRICT_REALLOC
			if (m_nBlobCount != 0 || nCount > 1)
				m_pData = (uint8_t*)realloc(m_pData, (m_nBlobCount + nCount) * m_nBlobSize);
#else
			uint32_t nDiff = 0;
			if (m_nBlobCount + nCount > m_nBlobLimit)
				nDiff = (m_nBlobCount + nCount) - m_nBlobLimit;

			if (nDiff > 0)
			{
				m_nBlobLimit = m_nBlobCount + nDiff; // Make room for the additional blobs
				m_nBlobLimit = (uint32_t)((m_nBlobLimit + 10) * _visNET_LOOSE_REALLOC); // Add 10 slots, and an additional percentage of the current slots

				m_pData = (uint8_t*)realloc(m_pData, m_nBlobLimit * m_nBlobSize);
			}
#endif

			memcpy(m_pData + (m_nBlobSize * m_nBlobCount), reinterpret_cast<uint8_t*>(items), m_nBlobSize * nCount);
			m_nBlobCount += nCount;
		}

		T* get(uint32_t nIndex){
			if (m_nBlobCount == 0 || nIndex > m_nBlobCount - 1)
				return nullptr;

			return (T*)(m_pData + (m_nBlobSize * nIndex));
		}

		uint32_t getArraySize() { return m_nBlobSize * m_nBlobCount; }
		uint32_t getBlobSize() { return m_nBlobSize; }
		uint32_t getBlobCount() { return m_nBlobCount; }
	};
}

#ifdef _visNET_STRICT_REALLOC
	#undef _visNET_STRICT_REALLOC
#endif

#ifdef _visNET_LOOSE_REALLOC
	#undef _visNET_LOOSE_REALLOC
#endif