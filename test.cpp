#include "gmock/gmock.h"
#include <string>

using namespace testing;

// Interface정의 및 구현. 다른 파일로 이동하셔도 좋을 것 같습니다.

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
    void sell(std::string stockCode, int price, int count){};
    void getPrice(std::string stockCode){};

    // login�� ���ؼ� user�� �߰��մϴ�. match���� �ʴ� user�� ���� ���¿����� login����, ������ ����
    void addUser(std::string id, std::string password){};
};

class NemoDriver : public IStockBrockerDriver
{
    bool login(std::string id, std::string password) { return false; };
    void buy(std::string stockCode, int price, int count){};
    void sell(std::string stockCode, int price, int count){};
    void getPrice(std::string stockCode){};

    // login�� ���ؼ� user�� �߰��մϴ�. match���� �ʴ� user�� ���� ���¿����� login����, ������ ����
    void addUser(std::string id, std::string password){};
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

<<<<<<< HEAD
void selectStockBroker(void) {}

// Unit Test Code. 계속 추가하겠습니다.

TEST(MockDriverTest, LoginCalledWithCorrectArguments) {
    MockDriver mock;
    EXPECT_CALL(mock, login(StrEq("username"), StrEq("password"))).Times(1);

    mock.login("username", "password");
=======
// Unit Test Code. ��� �߰��ϰڽ��ϴ�.
TEST_F(KiwiDriverTest, LoginTestSuccess) {
    driver->addUser("username", "password");
    bool loginResultSuccess = driver->login("username", "password");
    EXPECT_EQ(loginResultSuccess, 1);
>>>>>>> 796f1dfec2bcc28642033f64dfddb8f9beb9e302
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

int main()
{
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
