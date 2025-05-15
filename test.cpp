#include "gmock/gmock.h"
#include <string>
#include "kiwer_api.cpp"
#include "nemo_api.cpp"
using namespace testing;
using namespace std;

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
    
    void login(std::string id, std::string password)
    {
        Kiwer.login(id, password);
    };
    void buy(std::string stockCode, int price, int count){
        Kiwer.buy(stockCode, count, price);
    };
    void sell(std::string stockCode, int price, int count){
        Kiwer.sell(stockCode, count, price);
    };
    int getPrice(std::string stockCode) {
        return Kiwer.currentPrice(stockCode);
    }
private:
    KiwerAPI Kiwer;
};

class NemoDriver : public IStockBrockerDriver
{
    void login(std::string id, std::string password)
    {
        Nemo.certification(id, password);
    };
    void buy(std::string stockCode, int price, int count){
        Nemo.purchasingStock(stockCode, price, count);
    };
    void sell(std::string stockCode, int price, int count){
        Nemo.sellingStock(stockCode, price, count);
    };
    int getPrice(std::string stockCode) {
        return Nemo.getMarketPrice(stockCode, 1);
    }
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
    void setPrice(int price1, int price2) {
        this->sellPrice1 = price1;
        this->sellPrice2 = price2;
    }
    string sellNiceTiming(string stockCode, int count) {
       // int price1 = StockBrockerDriver->getPrice(stockCode);
        //int price2 = StockBrockerDriver->getPrice(stockCode);
        if (sellPrice1 > sellPrice2) {
            StockBrockerDriver->sell(stockCode, sellPrice2, count);
            return "sell";
        }
        else return "no sell";
    }
    IStockBrockerDriver* getStockBroker() {
        return StockBrockerDriver;
    }
   
    IStockBrockerDriver *StockBrockerDriver = nullptr;

private:
    int sellPrice1, sellPrice2;
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

class KiwerDriverTest : public StockBrokerDriverTest {
protected:
    std::unique_ptr<IStockBrockerDriver> createDriver() override {
        return std::make_unique<KiwerDriver>();
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

TEST_F(KiwerDriverTest, KiwiLoginTestSuccess) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    driver->login("username", "password");

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    EXPECT_EQ(output, "username login success\n");
}

TEST_F(KiwerDriverTest, BuyOutputTest) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    driver->buy("AAPL", 5, 100);

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    EXPECT_EQ(output, "AAPL : Buy stock ( 5 * 100)\n");
}
TEST_F(NemoDriverTest, BuyOutputTest) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    driver->buy("AAPL", 5, 100);

    std::cout.rdbuf(old);

    std::string output = buffer.str();
    EXPECT_EQ(output, "[NEMO]AAPL buy stock ( price : 5 ) * ( count : 100)\n");
}

TEST_F(NemoDriverTest, GetPriceTest) {


   int price = driver->getPrice("AAPL");

    EXPECT_THAT(price, Gt(0));
}
TEST_F(KiwerDriverTest, GetPriceTest) {


    int price = driver->getPrice("AAPL");

    EXPECT_THAT(price, Gt(0));
}


TEST_F(KiwerDriverTest, SellOutputTest) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    driver->sell("AAPL", 100, 5);

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

TEST(StockBrokerDriverTest, selectCorrectBroker) {
    AutoTradingSystem autoTradingSystem;
    
    EXPECT_EQ(0, autoTradingSystem.selectStockBroker("KIWER"));
}

TEST(StockBrokerDriverTest, selectWrongBroker) {
    AutoTradingSystem autoTradingSystem;
    autoTradingSystem.selectStockBroker("MANGO");
    EXPECT_THAT(autoTradingSystem.getStockBroker(), IsNull());
}


TEST(StockBrokerDriverTest, KIWERsellNiceTimingTest) {
    AutoTradingSystem autoTradingSystem;
    autoTradingSystem.selectStockBroker("KIWER");
    autoTradingSystem.setPrice(2, 1);
    
    EXPECT_EQ("sell", autoTradingSystem.sellNiceTiming("AAPL", 3));
}
TEST(StockBrokerDriverTest, KIWERNotsellNiceTimingTest) {
    AutoTradingSystem autoTradingSystem;
    autoTradingSystem.selectStockBroker("KIWER");
    autoTradingSystem.setPrice(1,2);
    
    EXPECT_EQ("no sell", autoTradingSystem.sellNiceTiming("AAPL", 3));
}
TEST(StockBrokerDriverTest, NEMOsellNiceTimingTest) {
    AutoTradingSystem autoTradingSystem;
    autoTradingSystem.selectStockBroker("NEMO");
    autoTradingSystem.setPrice(2, 1);
    
    EXPECT_EQ("sell", autoTradingSystem.sellNiceTiming("AAPL", 3));
}
TEST(StockBrokerDriverTest, NEMONotsellNiceTimingTest) {
    AutoTradingSystem autoTradingSystem;
    autoTradingSystem.selectStockBroker("NEMO");
    autoTradingSystem.setPrice(1,2);
    
    EXPECT_EQ("no sell", autoTradingSystem.sellNiceTiming("AAPL", 3));
}

int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
