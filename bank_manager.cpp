#include "bank_manager.h"
#include "bank_customer.h"
#include <iostream>
#include <chrono>
#include <map>
#include <algorithm>
#include <limits>

long days_since_for_bank(const string& date_str) {
    tm t = {};
    istringstream ss(date_str);
    ss >> get_time(&t, "%Y-%m-%d");
    if (ss.fail()) { return -1; }
    auto transaction_time = chrono::system_clock::from_time_t(mktime(&t));
    auto now = chrono::system_clock::now();
    return chrono::duration_cast<chrono::hours>(now - transaction_time).count() / 24;
}

void BankManager::showBankAdminMenu(vector<Buyer*>& buyers, vector<Transaction*>& transactions) {
    int choice = 0;
    while (choice != 5) {
        cout << "\nBank Admin Menu\n";
        cout << "1. List all transactions within a week\n";
        cout << "2. List all bank customers\n";
        cout << "3. List all dormant accounts (no transaction within a month)\n";
        cout << "4. List top N users with most transactions today\n";
        cout << "5. Back to Main Menu\n";
        cout << "Select an option: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: listTransactionsLastWeek(transactions); break;
            case 2: listAllCustomers(buyers); break;
            case 3: listDormantAccounts(buyers, transactions); break;
            case 4: listTopUsersToday(buyers, transactions); break;
            case 5: cout << "Returning to main menu...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    }
}

void BankManager::listTransactionsLastWeek(const vector<Transaction*>& transactions) {
    cout << "\nTransactions in the Last 7 Days\n";
    bool found = false;
    for (const auto* t : transactions) {
        if (days_since_for_bank(t->getDate()) <= 7) {
            cout << "ID: " << t->getId() << ", Buyer: " << t->getBuyerId()
                << ", Seller: " << t->getSellerId() << ", Amount: " << t->getAmount()
                << ", Date: " << t->getDate() << ", Status: " << t->getStatus() << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No transactions found in the last 7 days.\n";
    }
}

void BankManager::listAllCustomers(const vector<Buyer*>& buyers) {
    cout << "\nAll Bank Customers\n";
    bool found = false;
    for (const auto* b : buyers) {
        if (b->hasBankAccount()) {
            b->getAccount()->printInfo();
            found = true;
        }
    }
    if (!found) {
        cout << "No customers with a bank account found.\n";
    }
}

void BankManager::listDormantAccounts(const vector<Buyer*>& buyers, const vector<Transaction*>& transactions) {
    cout << "\nDormant Accounts (No transactions in the last 30 days)-\n";
    bool any_dormant_found = false;
    for (const auto* b : buyers) {
        if (!b->hasBankAccount()) continue;

        bool isActive = false;
        for (const auto* t : transactions) {
            if (days_since_for_bank(t->getDate()) <= 30) {
                if (t->getBuyerId() == b->getId() || t->getSellerId() == b->getId()) {
                    isActive = true;
                    break;
                }
            }
        }

        if (!isActive) {
            cout << "Dormant Customer: " << b->getName() << " (ID: " << b->getId() << ")\n";
            any_dormant_found = true;
        }
    }
    if (!any_dormant_found) {
        cout << "No dormant accounts found.\n";
    }
}

void BankManager::listTopUsersToday(const vector<Buyer*>& buyers, const vector<Transaction*>& transactions) {
    int n;
    cout << "Enter number of top users to show (N): ";
    cin >> n;

    map<int, int> userFrequency;
    for (const auto* t : transactions) {
        if (t->isToday()) {
            userFrequency[t->getBuyerId()]++;
            userFrequency[t->getSellerId()]++;
        }
    }

    if (userFrequency.empty()) {
        cout << "No transactions today.\n";
        return;
    }

    vector<pair<int, int>> sortedUsers(userFrequency.begin(), userFrequency.end());
    sort(sortedUsers.begin(), sortedUsers.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    cout << "\nTop " << n << " Most Active Users Today\n";
    int rank = 1;
    for (const auto& p : sortedUsers) {
        if (rank > n) break;

        string userName = "Unknown";
        for (const auto* b : buyers) {
            if (b->getId() == p.first) {
                userName = b->getName();
                break;
            }
        }
        cout << rank++ << ". User: " << userName << " (ID: " << p.first << "), Transactions: " << p.second << endl;
    }
}