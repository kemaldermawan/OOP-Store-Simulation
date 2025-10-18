#ifndef BANK_MANAGER_H
#define BANK_MANAGER_H

#include "buyer.h"
#include "transaction.h"
#include <vector>
#include <string>

using namespace std;

class BankManager {
public:
    static void showBankAdminMenu(vector<Buyer*>& buyers, vector<Transaction*>& transactions);

private:
    static void listTransactionsLastWeek(const vector<Transaction*>& transactions);
    static void listAllCustomers(const vector<Buyer*>& buyers);
    static void listDormantAccounts(const vector<Buyer*>& buyers, const vector<Transaction*>& transactions);
    static void listTopUsersToday(const vector<Buyer*>& buyers, const vector<Transaction*>& transactions);
};

#endif