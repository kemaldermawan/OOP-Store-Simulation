#include "data_manager.h"
#include <fstream>
#include <iostream>
using namespace std;

void DataManager::saveAllData(const vector<Buyer*>& buyers) {
    json j;
    for (auto b : buyers) {
        j["buyers"].push_back(b->toJson());
    }
    ofstream file("database.json");
    file << setw(4) << j;
    file.close();
}

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