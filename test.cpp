#include "gmock/gmock.h"
#include "kiwer_api.cpp"
#include "nemo_api.cpp"
#include <string>

using namespace testing;
using namespace std;


//// Class정의 및 구현. 다른 파일로 이동하셔도 좋을 것 같습니다.
//class AutoTradingSystem
//{
//public:
//private:
//    IStockBrockerDriver* StockBrockerDriver;
//};

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
    void login(std::string id, std::string password)
    {
        Kiwer.login(id, password);
    };
    void buy(std::string stockCode, int price, int count){};
    void sell(std::string stockCode, int price, int count){};
    void getPrice(std::string stockCode){};
private:
    KiwerAPI Kiwer;
};

class NemoDriver : public IStockBrockerDriver
{
    void login(std::string id, std::string password)
    {
        Nemo.certification(id, password);
    };
    void buy(std::string stockCode, int price, int count){};
    void sell(std::string stockCode, int price, int count){};
    void getPrice(std::string stockCode){};
private:
    NemoAPI Nemo;
};

class AutoTradingSystem {
public:
    int selectStockBroker(string brokerName) {
        int ret = 0;

        if (brokerName == "KIWER") {
            StockBrockerDriver = new KiwiDriver;
        }
        else if (brokerName == "NEMO") {
            StockBrockerDriver = new NemoDriver;
        }
        else {
            ret = -1;
        }
        return ret;
    }

    IStockBrockerDriver* getStockBroker() {
        return StockBrockerDriver;
    }
   
    IStockBrockerDriver *StockBrockerDriver = nullptr;
};

// Test용 Fixture
class StockBrokerDriverTest : public::testing::Test {
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

// Unit Test Code. 계속 추가하겠습니다.
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

// Select StockBoroker TC
TEST(StockBrokerDriverTest, selectCorrectBroker) {
    AutoTradingSystem autoTradingSystem;
    
    EXPECT_EQ(0, autoTradingSystem.selectStockBroker("KIWER"));
}

TEST(StockBrokerDriverTest, selectWrongBroker) {
    AutoTradingSystem autoTradingSystem;

    EXPECT_THAT(autoTradingSystem.getStockBroker(), IsNull());
}


int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
