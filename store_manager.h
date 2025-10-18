#ifndef STORE_MANAGER_H
#define STORE_MANAGER_H

#include <vector>
#include "buyer.h"
#include "seller.h"
#include "transaction.h"
using namespace std;

class StoreManager {
public:
    static void showStoreAdminMenu(vector<Buyer*>& buyers, vector<Transaction*>& transactions);
private:
    static void listTransactionsLastKDays(const vector<Transaction*>& transactions);
    static void listUncompletedTransactions(const vector<Transaction*>& transactions);
    static void listMostFrequentItems(const vector<Transaction*>& transactions);
    static void listMostActiveBuyers(const vector<Buyer*>& buyers, const vector<Transaction*>& transactions);
    static void listMostActiveSellers(const vector<Buyer*>& buyers, const vector<Transaction*>& transactions);
    static void listUnpaidTransactions(const vector<Transaction*>& transactions);
};

#endif