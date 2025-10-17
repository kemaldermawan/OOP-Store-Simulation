#ifndef STORE_MANAGER_H
#define STORE_MANAGER_H

#include <vector>
#include "buyer.h"
#include "seller.h"
#include "transaction.h"
using namespace std;

class StoreManager {
public:
    static void listAllStores(const vector<Buyer*>& buyers);
    static Seller* selectStore(const vector<Buyer*>& buyers);
    static void browseStoreInventory(Seller* seller);
};

#endif // STORE_MANAGER_H