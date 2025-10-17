#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <vector>
#include <string>
#include "buyer.h"
#include "seller.h"
#include "bank_customer.h"
#include "transaction.h"
#include "json.hpp"
#include <iomanip>
using namespace std;
using json = nlohmann::json;

class DataManager {
public:
    static void loadAllData(vector<Buyer*>& buyers);
    static void saveAllData(const vector<Buyer*>& buyers);
};

#endif // DATA_MANAGER_H