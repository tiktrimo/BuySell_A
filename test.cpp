#include "gmock/gmock.h"

TEST(a, b)
{
	EXPECT_EQ(1, 1);
}

int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}