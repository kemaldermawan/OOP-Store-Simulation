#include "transaction.h"

Transaction::Transaction() : id(0), buyerId(0), sellerId(0), amount(0.0), date(""), status("pending") {}

Transaction::Transaction(int id, int buyerId, int sellerId, double amount, const string& date, const string& status)
    : id(id), buyerId(buyerId), sellerId(sellerId), amount(amount), date(date), status(status) {}

int Transaction::getId() const { return id; }
int Transaction::getBuyerId() const { return buyerId; }
int Transaction::getSellerId() const { return sellerId; }
double Transaction::getAmount() const { return amount; }
string Transaction::getDate() const { return date; }
string Transaction::getStatus() const { return status; }

json Transaction::toJson() const {
    json j;
    j["id"] = id;
    j["buyerId"] = buyerId;
    j["sellerId"] = sellerId;
    j["amount"] = amount;
    j["date"] = date;
    j["status"] = status;
    return j;
}

void Transaction::fromJson(const json& j) {
    id = j.value("id", 0);
    buyerId = j.value("buyerId", 0);
    sellerId = j.value("sellerId", 0);
    amount = j.value("amount", 0.0);
    date = j.value("date", string(""));
    status = j.value("status", string("pending"));
}