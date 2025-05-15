#include "gmock/gmock.h"
#include "kiwer_api.cpp"
#include "nemo_api.cpp"
#include <string>
#include "kiwer_api.cpp"
#include "nemo_api.cpp"

using namespace testing;

//// Class���� �� ����. �ٸ� ���Ϸ� �̵��ϼŵ� ���� �� �����ϴ�.
//class AutoTradingSystem
//{
//public:
//private:
//    IStockBrockerDriver* StockBrockerDriver;
//};

// Interface���� �� ����. �ٸ� ���Ϸ� �̵��ϼŵ� ���� �� �����ϴ�.

class IStockBrockerDriver
{
public:
	virtual ~IStockBrockerDriver() = default;
	virtual bool login(std::string id, std::string password) = 0;
	virtual void buy(std::string stockCode, int price, int count) = 0;
	virtual void sell(std::string stockCode, int price, int count) = 0;
	virtual int getPrice(std::string stockCode) = 0;
    // login�� ���ؼ� user�� �߰��մϴ�. match���� �ʴ� user�� ���� ���¿����� login����, ������ ����
    virtual void addUser(std::string id, std::string password) = 0;
};

class KiwiDriver : public IStockBrockerDriver
{
private:
    KiwerAPI api;

public:
    bool login(std::string id, std::string password) override {}
    void buy(std::string stockCode, int price, int count) override {}
    void sell(std::string stockCode, int price, int count) override {
        api.sell(stockCode, count, price);
    }
    int getPrice(std::string stockCode) override {}
    void addUser(std::string id, std::string password){};
};

class NemoDriver : public IStockBrockerDriver
{
private:
    NemoAPI api;

public:
    bool login(std::string id, std::string password) override {}
    void buy(std::string stockCode, int price, int count) override {}
    void sell(std::string stockCode, int price, int count) override {
        api.sellingStock(stockCode, price, count);
    }
    int getPrice(std::string stockCode) override {}
};

class MockDriver : public IStockBrockerDriver
{
public:
	MOCK_METHOD(bool, login, (std::string id, std::string password), (override));
	MOCK_METHOD(void, buy, (std::string stockCode, int price, int count), (override));
	MOCK_METHOD(void, sell, (std::string stockCode, int price, int count), (override));
	MOCK_METHOD(int, getPrice, (std::string stockCode), (override));
};

// Test�� Fixture
class StockBrokerDriverTest : public ::testing::Test {
protected:
    std::unique_ptr<IStockBrockerDriver> driver;

    virtual std::unique_ptr<IStockBrockerDriver> createDriver() = 0;

    void SetUp() override {
        driver = createDriver();
    }
};

class KiwiDriverTest : public StockBrokerDriverTest {
protected:
    std::unique_ptr<IStockBrockerDriver> createDriver() override {
        return std::make_unique<KiwiDriver>();
    }
};

class NemoDriverTest : public StockBrokerDriverTest {
protected:
    std::unique_ptr<IStockBrockerDriver> createDriver() override {
        return std::make_unique<NemoDriver>();
    }
};

// Unit Test Code. ��� �߰��ϰڽ��ϴ�.
TEST_F(KiwiDriverTest, LoginTestSuccess) {
    driver->addUser("username", "password");
    bool loginResultSuccess = driver->login("username", "password");
    EXPECT_EQ(loginResultSuccess, 1);
}
TEST_F(KiwiDriverTest, LoginTestFail) {
    driver->addUser("username", "password");
    bool loginResultFail = driver->login("username", "password_wrong");
    EXPECT_EQ(loginResultFail, 0);
}
TEST_F(NemoDriverTest, LoginTestSuccess) {
    driver->addUser("username", "password");
    bool loginResultSuccess = driver->login("username", "password");
    EXPECT_EQ(loginResultSuccess, 1);
}
TEST_F(NemoDriverTest, LoginTestFail) {
    driver->addUser("username", "password");
    bool loginResultFail = driver->login("username", "password_wrong");
    EXPECT_EQ(loginResultFail, 0);
}

TEST_F(KiwiDriverTest, BuyOutputTest) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    driver->buy("AAPL", 5, 100);

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    EXPECT_EQ(output, "AAPL : Buy stock ( 100 * 5)");
}
TEST_F(NemoDriverTest, BuyOutputTest) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    driver->buy("AAPL", 5, 100);

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    EXPECT_EQ(output, "[NEMO]AAPL buy stock ( price : 5 ) * ( count : 100)");
}

int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}