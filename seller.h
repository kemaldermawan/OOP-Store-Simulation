#ifndef SELLER_H
#define SELLER_H

#include <string>
#include <vector>
#include "item.h"
#include "transaction.h"
#include "bank_customer.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Buyer;

class Seller {
private:
    Buyer* owner;
    BankCustomer* account;
    int sellerId;
    string sellerName;
    vector<Item> items;
    vector<Transaction*> transactions;

public:
    Seller();
    Seller(Buyer& buyer, int sellerId, const string& sellerName);
    ~Seller();

    int getSellerId() const;
    string getSellerName() const;
    Buyer* getOwner() const;
    Item* getItemById(int id);
    vector<Item>& getItems();
    const vector<Transaction*>& getTransactions() const;

    void addNewItem(int id, const string& name, int qty, double price);
    void updateItemQuantity(int itemId, int newQty);
    void updateItemPrice(int itemId, double newPrice);

    void addTransaction(Transaction* t);
    double getBalance() const;
    void setBalance(double amount);
    void setAccount(BankCustomer* acc);
    BankCustomer* getAccount() const;

    json toJson() const;
    void fromJson(const json& j);
};

#endif