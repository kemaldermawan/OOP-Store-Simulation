#include "transaction.h"
#include <iostream>
#include <chrono>
using namespace std;


Transaction::Transaction()
    : id(0),
    buyerId(0),
    sellerId(0),
    transactionId(0),
    amount(0.0),
    date(""),
    status("unpaid") {}

Transaction::Transaction(int id, int buyerId, int sellerId, double amount, const string& date, const string& status, const vector<OrderItem>& items)
    : id(id),
    buyerId(buyerId),
    sellerId(sellerId),
    transactionId(id),
    amount(amount),
    items(items),
    date(date),
    status(status) {}

int Transaction::getId() const { return id; }
int Transaction::getTransactionId() const { return transactionId; }
int Transaction::getBuyerId() const { return buyerId; }
int Transaction::getSellerId() const { return sellerId; }
double Transaction::getAmount() const { return amount; }
string Transaction::getDate() const { return date; }
string Transaction::getStatus() const { return status; }
const vector<OrderItem>& Transaction::getItems() const { return items; }

void Transaction::setStatus(const string& s) { status = s; }
void Transaction::setBuyer(int bId) { buyerId = bId; }
void Transaction::setSeller(int sId) { sellerId = sId; }

void Transaction::setDateToNow() {
    time_t now = time(nullptr);
    tm* nowTm = localtime(&now);
    ostringstream oss;
    oss << put_time(nowTm, "%Y-%m-%d");
    date = oss.str();
}

int Transaction::generateId() {
    static int counter = 1000;
    return counter++;
}

json Transaction::toJson() const {
    json j;
    j["id"] = id;
    j["transactionId"] = transactionId;
    j["buyerId"] = buyerId;
    j["sellerId"] = sellerId;
    j["amount"] = amount;
    j["date"] = date;
    j["status"] = status;

    json items_json = json::array();
    for (const auto& item : items) {
        items_json.push_back(item.toJson());
    }
    j["items"] = items_json;

    return j;
}

void Transaction::fromJson(const json& j) {
    id = j.value("id", 0);
    transactionId = j.value("transactionId", 0);
    buyerId = j.value("buyerId", 0);
    sellerId = j.value("sellerId", 0);
    amount = j.value("amount", 0.0);
    date = j.value("date", string(""));
    status = j.value("status", string("unpaid"));

    items.clear();
    if (j.contains("items")) {
        for (const auto& item_json : j["items"]) {
            OrderItem item;
            item.fromJson(item_json);
            items.push_back(item);
        }
    }
}

tm Transaction::getDateAsTm() const {
    tm t = {};
    istringstream ss(date);
    ss >> get_time(&t, "%Y-%m-%d");
    return t;
}

bool Transaction::isToday() const {
    time_t now = time(nullptr);
    tm* nowTm = localtime(&now);
    tm t = getDateAsTm();
    return (t.tm_year == nowTm->tm_year &&
            t.tm_mon == nowTm->tm_mon &&
            t.tm_mday == nowTm->tm_mday);
}

bool Transaction::isLastMonth() const {
    time_t now = time(nullptr);
    tm* nowTm = localtime(&now);
    tm t = getDateAsTm();

    int lastMonth = nowTm->tm_mon - 1;
    int year = nowTm->tm_year;
    if (lastMonth < 0) {
        lastMonth = 11;
        year -= 1;
    }
    return (t.tm_year == year && t.tm_mon == lastMonth);
}