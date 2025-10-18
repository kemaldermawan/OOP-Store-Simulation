#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "json.hpp"
#include "order.h"

using namespace std;
using json = nlohmann::json;

class Transaction {
private:
    int id;            
    int buyerId;    
    int sellerId;   
    int transactionId;      
    double amount;       
    string date;         
    string status;  
    vector<OrderItem> items;    

public:
    Transaction();
    Transaction(int id, int buyerId, int sellerId, double amount, const string& date, const string& status, const vector<OrderItem>& items);

    int getId() const;
    int getTransactionId() const;
    int getBuyerId() const;
    int getSellerId() const;
    double getAmount() const;
    string getDate() const;
    string getStatus() const;
    const vector<OrderItem>& getItems() const;

    void setStatus(const string& s);
    void setBuyer(int bId);
    void setSeller(int sId);
    void setDateToNow();

    static int generateId();

    json toJson() const;
    void fromJson(const json& j);

    std::tm getDateAsTm() const;
    bool isToday() const;
    bool isLastMonth() const;
};

#endif 