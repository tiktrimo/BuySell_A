#include "gmock/gmock.h"
#include <string>
#include "kiwer_api.cpp"
#include "nemo_api.cpp"

using namespace testing;

// Interface정의 및 구현. 다른 파일로 이동하셔도 좋을 것 같습니다.

class IStockBrockerDriver
{
public:
	virtual ~IStockBrockerDriver() = default;
	virtual void login(std::string id, std::string password) = 0;
	virtual void buy(std::string stockCode, int price, int count) = 0;
	virtual void sell(std::string stockCode, int price, int count) = 0;
	virtual int getPrice(std::string stockCode) = 0;
};

class KiwiDriver : public IStockBrockerDriver
{
public:
    void login(std::string id, std::string password) override {}
    void buy(std::string stockCode, int price, int count) override {}
    void sell(std::string stockCode, int price, int count) override {
        api.sell(stockCode, count, price);
    }
    int getPrice(std::string stockCode) override {}

private:
    KiwerAPI api;
};

class NemoDriver : public IStockBrockerDriver
{
public:
    void login(std::string id, std::string password) override {}
    void buy(std::string stockCode, int price, int count) override {}
    void sell(std::string stockCode, int price, int count) override {
        api.sellingStock(stockCode, price, count);
    }
    int getPrice(std::string stockCode) override {}

private:
    NemoAPI api;
};

class MockDriver : public IStockBrockerDriver
{
public:
	MOCK_METHOD(void, login, (std::string id, std::string password), (override));
	MOCK_METHOD(void, buy, (std::string stockCode, int price, int count), (override));
	MOCK_METHOD(void, sell, (std::string stockCode, int price, int count), (override));
	MOCK_METHOD(int, getPrice, (std::string stockCode), (override));
};

// Unit Test Code. 계속 추가하겠습니다.

TEST(MockDriverTest, LoginCalledWithCorrectArguments) {
    MockDriver mock;
    EXPECT_CALL(mock, login(StrEq("username"), StrEq("password"))).Times(1);

    mock.login("username", "password");
}

TEST(MockDriverTest, BuyCalledWithCorrectArguments) {
    MockDriver mock;
    EXPECT_CALL(mock, buy(StrEq("AAPL"), 200, 10)).Times(1);

    mock.buy("AAPL", 200, 10);
}

TEST(MockDriverTest, SellCalledWithCorrectArguments) {
    MockDriver mock;
    EXPECT_CALL(mock, sell(StrEq("AAPL"), 200, 10)).Times(1);

    mock.sell("AAPL", 200, 10);
}

TEST(MockDriverTest, GetPriceCalledWithCorrectStockCode) {
    MockDriver mock;
    EXPECT_CALL(mock, getPrice(StrEq("AAPL"))).Times(1);

    mock.getPrice("AAPL");
}

int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}