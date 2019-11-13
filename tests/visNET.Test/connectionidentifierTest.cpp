#include "pch.h"

TEST(Structure_ConnectionIdentifier, CreateConnectionIdentifier)
{
	// Assign
	// Act
	visNET::ConnectionIdentifier id(123);

	// Assert
	EXPECT_EQ(123, id.getValue());
	EXPECT_TRUE(id.isValid());
}

TEST(Structure_ConnectionIdentifier, CreateInvalidConnectionIdentifier)
{
	// Assign
	// Act
	visNET::ConnectionIdentifier id(0);

	// Assert
	EXPECT_FALSE(id.isValid());
}

TEST(Structure_ConnectionIdentifier, IncreaseConnectionIdentifier)
{
	// Assign
	visNET::ConnectionIdentifier id(100);

	// Act
	auto increasedId = id.increase();

	// Assert
	EXPECT_EQ(100, id.getValue());
	EXPECT_EQ(101, increasedId.getValue());
}

TEST(Structure_ConnectionIdentifier, CompareEqualIdentifiers)
{
	// Assign
	visNET::ConnectionIdentifier id1(234);
	visNET::ConnectionIdentifier id2(234);

	// Act
	bool areEqual = id1 == id2;
	bool areNotEqual = id1 != id2;

	// Assert
	EXPECT_TRUE(areEqual);
	EXPECT_FALSE(areNotEqual);
}

TEST(Structure_ConnectionIdentifier, CompareNotEqualIdentifiers)
{
	// Assign
	visNET::ConnectionIdentifier id1(234);
	visNET::ConnectionIdentifier id2(9000);

	// Act
	bool areEqual = id1 == id2;
	bool areNotEqual = id1 != id2;

	// Assert
	EXPECT_FALSE(areEqual);
	EXPECT_TRUE(areNotEqual);
}