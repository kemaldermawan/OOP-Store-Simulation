#include "store_manager.h"
#include <iostream>

using namespace std;

// Menampilkan semua store
void StoreManager::listAllStores(const vector<Buyer*>& buyers) {
    cout << "\n=== Stores List ===\n";
    int idx = 0;
    for (auto b : buyers) {
        if (b->isSeller()) {
            cout << "Store " << ++idx << ": " << b->getSeller()->getSellerName() << " (Owner: " << b->getName() << ")\n";
        }
    }
    if (idx == 0) cout << "(No stores yet)\n";
}
