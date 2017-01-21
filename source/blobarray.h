#pragma once

/*
	Binary Large OBject
	
	With a BlobArray you can efficiently send multiple objects of the same type.
	It is also recommended to use if you want to send one single instance of an object.
*/

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

	uint8_t* read(uint32_t nIndex){
		if (m_nBlobCount == 0 || nIndex > m_nBlobCount - 1)
			return nullptr;

		return m_pData + (m_nBlobSize * nIndex);
	}

	uint32_t getTotalSize() { return m_nBlobSize * m_nBlobCount; }
	uint32_t getSize() { return m_nBlobSize; }
	uint32_t getCount() { return m_nBlobCount; }
};