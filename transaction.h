#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class Transaction {
private:
    int id;
    int buyerId;
    int sellerId;
    double amount;
    string date; // simple string date, e.g., "2025-10-16"
    string status; // "paid", "completed", "cancelled"
public:
    Transaction();
    Transaction(int id, int buyerId, int sellerId, double amount, const string& date, const string& status);
    int getId() const;
    int getBuyerId() const;
    int getSellerId() const;
    double getAmount() const;
    string getDate() const;
    string getStatus() const;
    json toJson() const;
    void fromJson(const json& j);
};

#endif // TRANSACTION_H