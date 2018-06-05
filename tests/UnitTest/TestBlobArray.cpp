#include "base.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(TestBlobArray)
	{
	public:		
		TEST_METHOD(FillArray)
		{
			visNET::BlobArray* pArray = new visNET::BlobArray(sizeof(uint32_t));
			// Asure that the reserving size for blobarray instances is 4 bytes
			Assert::AreEqual(sizeof(uint32_t), pArray->getBlobSize());

			// Add 32 numbers ranging from 0 to 31
			uint32_t testCount = 32;
			for(uint32_t i = 0; i < testCount; ++i)
				pArray->add(&i, 1);

			// Assure that there are 32 int instances in blobarray
			Assert::AreEqual(testCount, pArray->getCount());

			// Assure that the claimed datasize is correct
			Assert::AreEqual(testCount * sizeof(uint32_t), pArray->getArraySize());
		}

		TEST_METHOD(GetIndexPointer)
		{
#define GETINDEXPOINTER_TESTVAL1 44
#define GETINDEXPOINTER_TESTVAL2 45

			visNET::BlobArray* pArray = new visNET::BlobArray(sizeof(uint32_t));
			// Asure that the reserving size for blobarray instances is 4 bytes
			Assert::AreEqual(sizeof(uint32_t), pArray->getBlobSize());

			uint32_t val[] = { GETINDEXPOINTER_TESTVAL1, GETINDEXPOINTER_TESTVAL2 };
			pArray->add(val, 2);

			// Validate index pointer
			uint8_t* idxPtr = pArray->getIndexPtr(1);
			Assert::IsNotNull(idxPtr);

			// Retrieve second value
			uint32_t res = *pArray->getIndexPtr(1);
			Assert::AreEqual(static_cast<uint32_t>(GETINDEXPOINTER_TESTVAL2), res);
	
			// Retrieve first value
			res = *pArray->getIndexPtr(0);
			Assert::AreEqual(static_cast<uint32_t>(GETINDEXPOINTER_TESTVAL1), res);

			// Retrieve invalid pointer; Should return nullptr
			idxPtr = pArray->getIndexPtr(2);
			Assert::IsNull(idxPtr);

			// Retrieve invalid pointer; Should flipover due to unsigned integer casting
			idxPtr = pArray->getIndexPtr(-1);
			Assert::IsNull(idxPtr);

#undef GETINDEXPOINTER_TESTVAL2
#undef GETINDEXPOINTER_TESTVAL1
		}
	};
}