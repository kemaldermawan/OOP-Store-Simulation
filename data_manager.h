#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <vector>
#include <string>
#include <iomanip>
#include "buyer.h"
#include "seller.h"
#include "transaction.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class DataManager {
private:
    vector<Buyer*> buyers;
    vector<Seller*> sellers;
    vector<Transaction*> transactions;

public:
    static void loadAllData(vector<Buyer*>& buyers);
    static void saveAllData(const vector<Buyer*>& buyers);

    void saveTransaction(const Transaction& t);
    int getNextTransactionId();
    string getCurrentDate() const;

    Buyer* findBuyerById(int id);
    Seller* findSellerById(int id);
    void displayAllSellers() const;

    const vector<Buyer*>& getBuyers() const { return buyers; }
    const vector<Seller*>& getSellers() const { return sellers; }
    const vector<Transaction*>& getTransactions() const { return transactions; }

    void saveAll();
};

#endif