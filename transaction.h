#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include <sstream>
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

    std::tm getDateAsTm() const {
        std::tm t = {};
        std::istringstream ss(date);
        ss >> std::get_time(&t, "%Y-%m-%d"); // format sesuai string date
        return t;
    }

    bool isToday() const {
        std::time_t now = std::time(nullptr);
        std::tm* nowTm = std::localtime(&now);
        std::tm t = getDateAsTm();
        return (t.tm_year == nowTm->tm_year &&
                t.tm_mon == nowTm->tm_mon &&
                t.tm_mday == nowTm->tm_mday);
    }

    bool isLastMonth() const {
        std::time_t now = std::time(nullptr);
        std::tm* nowTm = std::localtime(&now);
        std::tm t = getDateAsTm();
        int lastMonth = nowTm->tm_mon - 1;
        int year = nowTm->tm_year;
        if (lastMonth < 0) {
            lastMonth = 11;
            year -= 1;
        }
        return (t.tm_year == year && t.tm_mon == lastMonth);
    }
};

#endif // TRANSACTION_H