#include "unity.h"
#include "MPU6050.h"

TEST_CASE("ADD of 3 and 4 is 7", "[add]")
{
	const int a = 3;
	const int b = 4;

	TEST_ASSERT_EQUAL(7, add(a, b));
}
