#include "TestHarness.h"

// To add your own unit tests
// 
// 1) Rename this file using a name that describes the software entity under test. E.g., if you were testing std::vector
//    rename this file as VectorTest.cpp
// 2) Add additional test files as needed. E.g., if you were testing a class named Animal and another class named Zoo you
//    would have two test files. AnimalTest.cpp and ZooTest.cpp
// 3) Remove the sample tests from this file

#include <string>
#include <vector>

TEST(equality, strings)
{
	std::string hello("Hello");

	CHECK_EQUAL("Hello", hello);
}

TEST(equality, wstring)
{
	std::wstring hello(L"Hello");

	CHECK_WEQUAL(L"Hello", hello);
}

TEST(equality, integralType)
{
	int x = 34;

	CHECK_EQUAL(34, x);

	// Use CHECK_EQUAL for integral types. This example just shows that CHECK_WEQUAL works for int types as well.
	CHECK_WEQUAL(34, x);
}

TEST(empty, vector)
{
	std::vector<int> emptyVector;

	// Test passes if expression is true
	CHECK(emptyVector.empty());
}

TEST(equality, double)
{
	// float types are inexact representions. To compare and get reliable results use CHECK_DOUBLES_EQUAL which works for double and float
	CHECK_DOUBLES_EQUAL(0.001, 0.0099999, 0.01);
	CHECK_DOUBLES_EQUAL(0.001f, 0.0099999f, 0.01f);

	// For more information on comparing floats see: https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
}

TEST(fail, vector)
{
	std::vector<std::string> names = { "Wally", "Dilbert", "Asok", "Alice", "Pointy-haired Boss" };

	if (names.empty())
	{
		CHECK_FAIL("names are not empty")
	}

	// This would be a better test of the same function
	CHECK(!names.empty())
}
