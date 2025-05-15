#include "gmock/gmock.h"
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
	virtual void login(std::string id, std::string password) = 0;
	virtual void buy(std::string stockCode, int price, int count) = 0;
	virtual void sell(std::string stockCode, int price, int count) = 0;
	virtual int getPrice(std::string stockCode) = 0;

    // login�� ���ؼ� user�� �߰��մϴ�. match���� �ʴ� user�� ���� ���¿����� login����, ������ ����
    virtual void addUser(std::string id, std::string password) = 0;


};

class KiwiDriver : public IStockBrockerDriver
{
    int getPrice(std::string stockCode) override {
        return Kiwer.currentPrice(stockCode);
    }

    // login�� ���ؼ� user�� �߰��մϴ�. match���� �ʴ� user�� ���� ���¿����� login����, ������ ����
    void addUser(std::string id, std::string password){};
    void login(std::string id, std::string password)
    {
        Kiwer.login(id, password);
    };
    void buy(std::string stockCode, int price, int count){};
    void sell(std::string stockCode, int price, int count){};

private:
    KiwerAPI Kiwer;
};
class NemoDriver : public IStockBrockerDriver
{
    void login(std::string id, std::string password)
    {
        Nemo.certification(id, password);
    };
    void buy(std::string stockCode, int price, int count) {};
    void sell(std::string stockCode, int price, int count) {};
    int getPrice(std::string stockCode) override {
        return Nemo.getMarketPrice(stockCode, 1);
    }
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
TEST_F(NemoDriverTest, NemoLoginTestSuccess) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    driver->login("username", "password");

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    EXPECT_EQ(output, "[NEMO]username login GOOD\n");
}

TEST_F(KiwiDriverTest, KiwiLoginTestSuccess) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    driver->login("username", "password");

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    EXPECT_EQ(output, "username login success\n");
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