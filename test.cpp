#include "gmock/gmock.h"
#include <string>

using namespace testing;
using namespace std;

// Interface?뺤쓽 諛?援ы쁽. ?ㅻⅨ ?뚯씪濡??대룞?섏뀛??醫뗭쓣 寃?媛숈뒿?덈떎.

class IStockBrockerDriver
{
public:
	virtual ~IStockBrockerDriver() = default;
	virtual bool login(std::string id, std::string password) = 0;
	virtual void buy(std::string stockCode, int price, int count) = 0;
	virtual void sell(std::string stockCode, int price, int count) = 0;
	virtual void getPrice(std::string stockCode) = 0;

    // login을 위해서 user를 추가합니다. match되지 않는 user가 없는 상태에서는 login실패, 있으면 성공
    virtual void addUser(std::string id, std::string password) = 0;
};

class KiwiDriver : public IStockBrockerDriver
{
    bool login(std::string id, std::string password) { return false; };
    void buy(std::string stockCode, int price, int count){};
    void sell(std::string stockCode, int price, int count){};
    void getPrice(std::string stockCode){};

    // login을 위해서 user를 추가합니다. match되지 않는 user가 없는 상태에서는 login실패, 있으면 성공
    void addUser(std::string id, std::string password){};
};

class NemoDriver : public IStockBrockerDriver
{
    bool login(std::string id, std::string password) { return false; };
    void buy(std::string stockCode, int price, int count){};
    void sell(std::string stockCode, int price, int count){};
    void getPrice(std::string stockCode){};

    // login을 위해서 user를 추가합니다. match되지 않는 user가 없는 상태에서는 login실패, 있으면 성공
    void addUser(std::string id, std::string password){};
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

// Select StockBoroker TC
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
