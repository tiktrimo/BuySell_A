#include "gmock/gmock.h"
#include <string>

using namespace testing;

// Interface정의 및 구현. 다른 파일로 이동하셔도 좋을 것 같습니다.

class IStockBrockerDriver
{
public:
	virtual ~IStockBrockerDriver() = default;
	virtual void login(std::string id, std::string password) = 0;
	virtual void buy(std::string stockCode, int price, int count) = 0;
	virtual void sell(std::string stockCode, int price, int count) = 0;
	virtual void getPrice(std::string stockCode) = 0;
};

class KiwiDriver : public IStockBrockerDriver
{

};

class NemoDriver : public IStockBrockerDriver
{

};

class MockDriver : public IStockBrockerDriver
{
public:
	MOCK_METHOD(void, login, (std::string id, std::string password), (override));
	MOCK_METHOD(void, buy, (std::string stockCode, int price, int count), (override));
	MOCK_METHOD(void, sell, (std::string stockCode, int price, int count), (override));
	MOCK_METHOD(void, getPrice, (std::string stockCode), (override));
};

// Unit Test Code. 계속 추가하겠습니다.

TEST(MockDriverTest, LoginCalledWithCorrectArguments) {
    std::unique_ptr<MockDriver> driver = std::make_unique<MockDriver>();
    EXPECT_CALL(*driver, login(StrEq("username"), StrEq("password"))).Times(1);

    driver->login("username", "password");
}

TEST(MockDriverTest, BuyCalledWithCorrectArguments) {
    std::unique_ptr<MockDriver> driver = std::make_unique<MockDriver>();
    EXPECT_CALL(*driver, buy(StrEq("AAPL"), 200, 10)).Times(1);

    driver->buy("AAPL", 200, 10);
}

TEST(MockDriverTest, SellCalledWithCorrectArguments) {
    std::unique_ptr<MockDriver> driver = std::make_unique<MockDriver>();
    EXPECT_CALL(*driver, sell(StrEq("AAPL"), 200, 10)).Times(1);

    driver->sell("AAPL", 200, 10);
}

TEST(MockDriverTest, GetPriceCalledWithCorrectStockCode) {
    std::unique_ptr<MockDriver> driver = std::make_unique<MockDriver>();
    EXPECT_CALL(*driver, getPrice(StrEq("AAPL"))).Times(1);

    driver->getPrice("AAPL");
}



int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}