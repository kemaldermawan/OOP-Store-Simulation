#ifndef BUYER_H
#define BUYER_H

#include <string>
#include <vector>
#include "transaction.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class BankCustomer;
class Seller;

class Buyer {
private:
    int id;
    string name;
    BankCustomer* account;
    Seller* sellerAccount;
    vector<Transaction*> transactions;

public:
    Buyer();
    Buyer(int id, const string& name);
    ~Buyer();

    int getId() const;
    string getName() const;

    void setAccount(BankCustomer* acc);
    bool hasBankAccount() const;
    BankCustomer* getAccount() const;

    void setSeller(Seller* s);
    bool isSeller() const;
    Seller* getSeller() const;

    void addTransaction(Transaction* t);
    vector<Transaction*> getTransactionsToday() const;
    vector<Transaction*> getTransactionsLastMonth() const;

    // Serialization
    json toJson() const;
    void fromJson(const json& j);
};

#endif // BUYER_H