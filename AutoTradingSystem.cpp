#include <string>
#include "kiwer_api.cpp"
#include "nemo_api.cpp"

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

class KiwerDriver : public IStockBrockerDriver
{
public:

    void addUser(std::string id, std::string password) {};
    void login(std::string id, std::string password)
    {
        Kiwer.login(id, password);
    };
    void buy(std::string stockCode, int price, int count) {
        Kiwer.buy(stockCode, count, price);
    };
    void sell(std::string stockCode, int price, int count) {
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
    void buy(std::string stockCode, int price, int count) {
        Nemo.purchasingStock(stockCode, price, count);
    };
    void sell(std::string stockCode, int price, int count) {
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
            StockBrockerDriver = new KiwerDriver;
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
private:
    IStockBrockerDriver* StockBrockerDriver = nullptr;
};