#include "data_manager.h"
#include <fstream>
#include <iostream>
#include <ctime>
using namespace std;

void DataManager::loadAllData(vector<Buyer*>& buyers) {
    ifstream file("database.json");
    if (!file.is_open()) {
        cout << "No existing database found. Starting fresh.\n";
        return;
    }

    json j;
    file >> j;
    file.close();

    if (j.contains("buyers")) {
        for (const auto& b : j["buyers"]) {
            Buyer* buyer = new Buyer();
            buyer->fromJson(b);
            buyers.push_back(buyer);
        }
    }
}

void DataManager::saveAllData(const vector<Buyer*>& buyers) {
    json j;
    for (auto* b : buyers) {
        j["buyers"].push_back(b->toJson());
    }

    ofstream file("database.json");
    file << setw(4) << j;
    file.close();
}

void DataManager::saveTransaction(const Transaction& t) {
    transactions.push_back(new Transaction(t));
    saveAll();
}

void DataManager::saveAll() {
    json j;

    for (auto* b : buyers)
        j["buyers"].push_back(b->toJson());

    for (auto* s : sellers)
        j["sellers"].push_back(s->toJson());

    for (auto* t : transactions)
        j["transactions"].push_back(t->toJson());

    ofstream file("database.json");
    file << setw(4) << j;
    file.close();

    cout << "Database successfully updated.\n";
}

Buyer* DataManager::findBuyerById(int id) {
    for (auto* b : buyers)
        if (b->getId() == id)
            return b;
    return nullptr;
}

Seller* DataManager::findSellerById(int id) {
    for (auto* s : sellers)
        if (s->getSellerId() == id)
            return s;
    return nullptr;
}

void DataManager::displayAllSellers() const {
    cout << "LIST OF REGISTERED SELLERS\n";
    cout << "ID\tStore Name\n";

    for (auto* s : sellers)
        cout << s->getSellerId() << "\t" << s->getSellerName() << endl;
}

int DataManager::getNextTransactionId() {
    return transactions.empty() ? 1 : transactions.back()->getId() + 1;
}

string DataManager::getCurrentDate() const {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
    return string(buffer);
}