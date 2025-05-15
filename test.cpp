#include "gmock/gmock.h"
#include <string>
#include "kiwer_api.cpp"
#include "nemo_api.cpp"
#include "AutoTradingSystem.cpp"
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


int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
