#include "store_manager.h"
#include <iostream>
#include <chrono>
#include <map>
#include <algorithm>

using namespace std;

long days_since(const string& date_str) {
    tm t = {};
    istringstream ss(date_str);
    ss >> get_time(&t, "%Y-%m-%d");
    auto transaction_time = chrono::system_clock::from_time_t(mktime(&t));
    auto now = chrono::system_clock::now();
    return chrono::duration_cast<chrono::hours>(now - transaction_time).count() / 24;
}

void StoreManager::showStoreAdminMenu(vector<Buyer*>& buyers, vector<Transaction*>& transactions) {
    int choice = 0;
    while (choice != 6) {
        cout << "\nStore Admin Menu\n";
        cout << "1. List transactions of the latest k days\n";
        cout << "2. List all paid but not completed transactions\n";
        cout << "3. List all most m frequent item transactions\n";
        cout << "4. List all most active buyers\n";
        cout << "5. List all most active sellers\n";
        cout << "6. Back to Main Menu\n";
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
            case 1: listTransactionsLastKDays(transactions); break;
            case 2: listUncompletedTransactions(transactions); break;
            case 3: listMostFrequentItems(transactions); break;
            case 4: listMostActiveBuyers(buyers, transactions); break;
            case 5: listMostActiveSellers(buyers, transactions); break;
            case 6: cout << "Returning to main menu...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    }
}

void StoreManager::listTransactionsLastKDays(const vector<Transaction*>& transactions) {
    int k;
    cout << "Enter number of days (k): ";
    cin >> k;
    cout << "\nTransactions in the Last " << k << " Days\n";
    for (const auto* t : transactions) {
        if (days_since(t->getDate()) <= k) {
            cout << "ID: " << t->getId() << ", Buyer: " << t->getBuyerId()
                << ", Seller: " << t->getSellerId() << ", Amount: " << t->getAmount()
                << ", Date: " << t->getDate() << ", Status: " << t->getStatus() << endl;
        }
    }
}

void StoreManager::listUncompletedTransactions(const vector<Transaction*>& transactions) {
    cout << "\nPaid But Not Completed Transactions\n";
    cout << "Note: This assumes 'Paid' status means not yet completed.\n";
    for (const auto* t : transactions) {
        if (t->getStatus() == "Paid") {
            cout << "ID: " << t->getId() << ", Buyer: " << t->getBuyerId()
                << ", Seller: " << t->getSellerId() << ", Amount: " << t->getAmount()
                << ", Date: " << t->getDate() << endl;
        }
    }
}

void StoreManager::listMostFrequentItems(const vector<Transaction*>& transactions) {
    int m;
    cout << "Enter number of top items to show (m): ";
    cin >> m;

    map<int, pair<string, int>> itemFrequency; 
    for (const auto* t : transactions) {
        for (const auto& item : t->getItems()) {
            if (itemFrequency.find(item.itemId) == itemFrequency.end()) {

                itemFrequency[item.itemId] = {"Item " + to_string(item.itemId), 0};
            }
            itemFrequency[item.itemId].second += item.quantity;
        }
    }

    vector<pair<int, pair<string, int>>> sortedItems(itemFrequency.begin(), itemFrequency.end());
    sort(sortedItems.begin(), sortedItems.end(), [](const auto& a, const auto& b) {
        return a.second.second > b.second.second;
    });

    cout << "\nTop " << m << " Most Frequent Items \n";
    int count = 0;
    for (const auto& p : sortedItems) {
        if (++count > m) break;
        cout << count << ". Item ID: " << p.first << ", Total Quantity Sold: " << p.second.second << endl;
    }
}

void StoreManager::listMostActiveBuyers(const vector<Buyer*>& buyers, const vector<Transaction*>& transactions) {
    map<int, int> buyerFrequency;
    for (const auto* t : transactions) {
        buyerFrequency[t->getBuyerId()]++;
    }

    vector<pair<int, int>> sortedBuyers(buyerFrequency.begin(), buyerFrequency.end());
    sort(sortedBuyers.begin(), sortedBuyers.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    cout << "\nMost Active Buyers \n";
    int rank = 1;
    for (const auto& p : sortedBuyers) {
        string buyerName = "Unknown";
        for (const auto* b : buyers) {
            if (b->getId() == p.first) {
                buyerName = b->getName();
                break;
            }
        }
        cout << rank++ << ". Buyer: " << buyerName << " (ID: " << p.first << "), Transactions: " << p.second << endl;
    }
}

void StoreManager::listMostActiveSellers(const vector<Buyer*>& buyers, const vector<Transaction*>& transactions) {
    map<int, int> sellerFrequency;
    for (const auto* t : transactions) {
        sellerFrequency[t->getSellerId()]++;
    }

    vector<pair<int, int>> sortedSellers(sellerFrequency.begin(), sellerFrequency.end());
    sort(sortedSellers.begin(), sortedSellers.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    cout << "\nMost Active Sellers\n";
    int rank = 1;
    for (const auto& p : sortedSellers) {
        string storeName = "Unknown";
        for (const auto* b : buyers) {
            if (b->isSeller() && b->getSeller()->getSellerId() == p.first) {
                storeName = b->getSeller()->getSellerName();
                break;
            }
        }
        cout << rank++ << ". Store: " << storeName << " (Seller ID: " << p.first << "), Transactions: " << p.second << endl;
    }
}

void StoreManager::listUnpaidTransactions(const vector<Transaction*>& transactions) {
    cout << "\n--- Unpaid Transactions ---\n";
    for (const auto* t : transactions) {
        if (t->getStatus() == "Unpaid" || t->getStatus() == "unpaid") {
            cout << "ID: " << t->getId() << ", Buyer: " << t->getBuyerId()
                << ", Seller: " << t->getSellerId() << ", Amount: " << t->getAmount()
                << ", Date: " << t->getDate() << endl;
        }
    }
}