#include "gmock/gmock.h"
#include <string>
#include "kiwer_api.cpp"
#include "nemo_api.cpp"
#include "AutoTradingSystem.cpp"
using namespace testing;
using namespace std;


class MockStockBrockerDriver : public IStockBrockerDriver {
public:
    MOCK_METHOD(void, login, (std::string id, std::string password), (override));
    MOCK_METHOD(void, buy, (std::string stockCode, int price, int count), (override));
    MOCK_METHOD(void, sell, (std::string stockCode, int price, int count), (override));
    MOCK_METHOD(int, getPrice, (std::string stockCode), (override));
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

TEST(AutoTradingSystemTest, BuyNiceTimingwiehCurrentPrice) {

    AutoTradingSystem autoSystem;
    MockStockBrockerDriver mockDriver;

    // mock 객체를 시스템에 주입
    autoSystem.setStockBroker(&mockDriver);

    // getPrice 호출 시 특정 값 반환하도록 설정
    EXPECT_CALL(mockDriver, getPrice("AAPL"))
        .Times(AnyNumber())
        .WillOnce(Return(123))
        .WillOnce(Return(124))
        .WillOnce(Return(125))
        .WillOnce(Return(126))
        .WillOnce(Return(127));

    int ret = autoSystem.buyNiceTiming("AAPL", 100);

    EXPECT_EQ(ret, 127);
}

TEST(AutoTradingSystemTest, BuyNiceTimingwiehUserPrice) {

    AutoTradingSystem autoSystem;
    MockStockBrockerDriver mockDriver;

    // mock 객체를 시스템에 주입
    autoSystem.setStockBroker(&mockDriver);

    // getPrice 호출 시 특정 값 반환하도록 설정
    EXPECT_CALL(mockDriver, getPrice("AAPL"))
        .Times(AnyNumber())
        .WillOnce(Return(123))
        .WillOnce(Return(124))
        .WillOnce(Return(125))
        .WillOnce(Return(126))
        .WillOnce(Return(127));

    int ret = autoSystem.buyNiceTiming("AAPL", 135);

    EXPECT_EQ(ret, 135);
}

TEST(AutoTradingSystemTest, BuyNiceTimingFail) {

    AutoTradingSystem autoSystem;
    MockStockBrockerDriver mockDriver;

    // mock 객체를 시스템에 주입
    autoSystem.setStockBroker(&mockDriver);

    // getPrice 호출 시 특정 값 반환하도록 설정
    EXPECT_CALL(mockDriver, getPrice("AAPL"))
        .Times(AnyNumber())
        .WillOnce(Return(123))
        .WillOnce(Return(124))
        .WillOnce(Return(125))
        .WillOnce(Return(122))
        .WillOnce(Return(127));

    int ret = autoSystem.buyNiceTiming("AAPL", 135);

    EXPECT_EQ(ret, -1);
}
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
