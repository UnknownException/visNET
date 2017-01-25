#pragma once

/*
	Binary Large OBject
	
	With a BlobArray you can efficiently send multiple objects of the same type.
	It is also recommended to use if you want to send one single instance of an object.
*/
namespace visNET{
	class BlobArray{
		uint32_t m_nBlobSize;
		uint32_t m_nBlobCount;
		uint8_t* m_pData;

	public:
		BlobArray(uint32_t blobSize){
			m_nBlobSize = blobSize;
			m_nBlobCount = 0;
			m_pData = new uint8_t[blobSize];
		}

		virtual ~BlobArray(){
			if (m_pData)
				delete[] m_pData;
		}

		template <typename T>
		void add(T* pData, uint32_t nCount = 1){
			if (m_nBlobCount != 0 || nCount > 1)
				m_pData = (uint8_t*)realloc(m_pData, (m_nBlobCount + nCount) * m_nBlobSize);

			memcpy(m_pData + (m_nBlobSize * m_nBlobCount), reinterpret_cast<uint8_t*>(pData), m_nBlobSize * nCount);
			m_nBlobCount += nCount;
		}

		uint8_t* getIndexPtr(uint32_t nIndex){
			if (m_nBlobCount == 0 || nIndex > m_nBlobCount - 1)
				return nullptr;

			return m_pData + (m_nBlobSize * nIndex);
		}

		template <typename T>
		bool read(uint32_t nIndex, T* pBuffer, uint32_t nAmount = 1){
			// The amount to read must always be one or more
			if (nAmount == 0) 
				return false;

			// Check if the index is valid and retrieve the address
			uint8_t* idxPtr = getIndexPtr(nIndex); 
			if (!idxPtr)
				return false;

			// Check if the requested amount to read is not forcing the application to read out of bounds
			if (nIndex + nAmount > m_nBlobCount) 
				return false;

			memcpy(pBuffer, idxPtr, nAmount * m_nBlobSize);

			return true;
		}

		uint32_t getArraySize() { return m_nBlobSize * m_nBlobCount; }
		uint32_t getBlobSize() { return m_nBlobSize; }
		uint32_t getCount() { return m_nBlobCount; }
	};
}