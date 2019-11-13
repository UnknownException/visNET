#include "pch.h"

TEST(Structure_BlobArray, CreateBlobArrayInt32)
{
	// Assign
	// Act
	visNET::BlobArray<int32_t> blob;

	// Assert
	EXPECT_EQ(0, blob.dataSize());
	EXPECT_EQ(0, blob.size());
	EXPECT_EQ(sizeof(int32_t), blob.itemSize());
}

TEST(Structure_BlobArray, AddValueToBlobArrayInt32)
{
	// Assign
	visNET::BlobArray<int32_t> blob;

	// Act
	blob.add(123);

	// Assert
	EXPECT_EQ(sizeof(int32_t) * 1, blob.dataSize());
	EXPECT_EQ(1, blob.size());
	EXPECT_EQ(sizeof(int32_t), blob.itemSize());
}

TEST(Structure_BlobArray, GetValueFromBlobArrayInt32)
{
	// Assign
	visNET::BlobArray<int32_t> blob;
	blob.add(123);

	// Act
	int32_t result = *blob.getPtr(0);

	// Assert
	EXPECT_EQ(123, result);
}

TEST(Structure_BlobArray, GetNullPtrFromInvalidBlobArrayInt32Index)
{
	// Assign
	visNET::BlobArray<int32_t> blob;
	blob.add(123);

	// Act
	int32_t* flipOver = blob.getPtr(-1);
	int32_t* edgeCase = blob.getPtr(1);

	// Assert
	EXPECT_EQ(nullptr, flipOver);
	EXPECT_EQ(nullptr, edgeCase);
}

TEST(Structure_BlobArray, AddArrayToBlobArrayInt32)
{
	constexpr int32_t arraySize = 10000;

	// Assign
	int32_t itemArray[arraySize];
	for (int32_t i = 0; i < arraySize; ++i)
		itemArray[i] = i;

	visNET::BlobArray<int32_t> blob;
	
	// Act
	blob.add(itemArray, arraySize);

	// Assert
	EXPECT_EQ(sizeof(int32_t) * arraySize, blob.dataSize());
	EXPECT_EQ(arraySize, blob.size());
	EXPECT_EQ(sizeof(int32_t), blob.itemSize());
}

TEST(Structure_BlobArray, GetAllValuesFromBlobArrayInt32FilledByArray)
{
	constexpr int32_t arraySize = 10000;

	// Assign
	int32_t itemArray[arraySize];
	for (int32_t i = 0; i < arraySize; ++i)
		itemArray[i] = i;

	visNET::BlobArray<int32_t> blob;
	blob.add(itemArray, arraySize);

	// Act
	int32_t resultArray[arraySize];
	for (int32_t i = 0; i < arraySize; ++i)
		resultArray[i] = *blob.getPtr(i);

	// Assert
	for(int32_t i = 0; i < arraySize; ++i)
		EXPECT_EQ(itemArray[i], resultArray[i]);
}
