#include "base.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(TestBlobArray)
	{
	public:		
		TEST_METHOD(FillArray)
		{
			auto pArray = new visNET::BlobArray<uint32_t>();
			// Asure that the reserving size for blobarray instances is 4 bytes
			Assert::AreEqual((uint32_t)sizeof(uint32_t), pArray->getBlobSize());

			// Add 32 numbers ranging from 0 to 31
			uint32_t testCount = 32;
			for(uint32_t i = 0; i < testCount; ++i)
				pArray->add(i);

			// Assure that there are 32 int instances in blobarray
			Assert::AreEqual(testCount, pArray->getBlobCount());

			// Assure that the claimed datasize is correct
			Assert::AreEqual(testCount * (uint32_t)sizeof(uint32_t), pArray->getArraySize());
		}

		TEST_METHOD(GetItemWithIndex)
		{
			const uint32_t GETINDEXPOINTER_TESTVAL1 = 44;
			const uint32_t GETINDEXPOINTER_TESTVAL2 = 45;

			visNET::BlobArray<uint32_t> blob;
			// Asure that the reserving size for blobarray instances is 4 bytes
			Assert::AreEqual((uint32_t)sizeof(uint32_t), blob.getBlobSize());

			uint32_t val[] = { GETINDEXPOINTER_TESTVAL1, GETINDEXPOINTER_TESTVAL2 };
			blob.add(val, 2);

			// Validate index pointer
			uint32_t* idxPtr = blob.get(1);
			Assert::IsNotNull(idxPtr);

			// Retrieve second value
			uint32_t res = *blob.get(1);
			Assert::AreEqual(static_cast<uint32_t>(GETINDEXPOINTER_TESTVAL2), res);
	
			// Retrieve first value
			res = *blob.get(0);
			Assert::AreEqual(static_cast<uint32_t>(GETINDEXPOINTER_TESTVAL1), res);

			// Retrieve invalid pointer; Should return nullptr
			idxPtr = blob.get(2);
			Assert::IsNull(idxPtr);

			// Retrieve invalid pointer; Should flipover due to unsigned integer casting
			idxPtr = blob.get(-1);
			Assert::IsNull(idxPtr);
		}
	};
}