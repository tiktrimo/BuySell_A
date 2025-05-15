#include "gmock/gmock.h"
#include "kiwer_api.cpp"
#include "nemo_api.cpp"
#include <string>

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
	virtual void getPrice(std::string stockCode) = 0;

    // login�� ���ؼ� user�� �߰��մϴ�. match���� �ʴ� user�� ���� ���¿����� login����, ������ ����
    virtual void addUser(std::string id, std::string password) = 0;
};

class KiwiDriver : public IStockBrockerDriver
{
    bool login(std::string id, std::string password) { return false; };
    void buy(std::string stockCode, int price, int count){};
    void sell(std::string stockCode, int price, int count){
        Kiwer.sell(stockCode, price, count);
    };
    void getPrice(std::string stockCode){};

    // login�� ���ؼ� user�� �߰��մϴ�. match���� �ʴ� user�� ���� ���¿����� login����, ������ ����
    void addUser(std::string id, std::string password){};
private:
    KiwerAPI Kiwer;
};

class NemoDriver : public IStockBrockerDriver
{
    bool login(std::string id, std::string password) { return false; };
    void buy(std::string stockCode, int price, int count){};
    void sell(std::string stockCode, int price, int count){
        Nemo.sellingStock(stockCode, price, count);
    };
    void getPrice(std::string stockCode){};

    // login�� ���ؼ� user�� �߰��մϴ�. match���� �ʴ� user�� ���� ���¿����� login����, ������ ����
    void addUser(std::string id, std::string password){};
private:
    NemoAPI Nemo;
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

TEST_F(KiwiDriverTest, SellOutputTest) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    driver->sell("AAPL", 5, 100);

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    EXPECT_EQ(output, "AAPL : Sell stock ( 100 * 5)\n");
}
TEST_F(NemoDriverTest, SellOutputTest) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    driver->sell("AAPL", 5, 100);

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    EXPECT_EQ(output, "[NEMO]AAPL sell stock ( price : 5 ) * ( count : 100)\n");
}

int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}