#include "seller.h"
#include "buyer.h"
#include <iostream>

Seller::Seller() : owner(nullptr), sellerId(0), sellerName("") {}

Seller::Seller(Buyer& buyer, int sellerId, const string& sellerName) : owner(&buyer), sellerId(sellerId), sellerName(sellerName) {}

Seller::~Seller() {}

int Seller::getSellerId() const { return sellerId; }

string Seller::getSellerName() const { return sellerName; }

Buyer* Seller::getOwner() const { return owner; }

vector<Item>& Seller::getItems() { return items; }

void Seller::addNewItem(int id, const string& name, int qty, double price) {
    Item newItem(id, name, qty, price);
    items.push_back(newItem);
}

void Seller::updateItemQuantity(int itemId, int newQty) {
    for (auto &item : items) {
        if (item.getId() == itemId) {
            item.setQuantity(newQty);
            cout << "Item " << item.getName() << " quantity updated to " << newQty << endl;
            return;
        }
    }
    cout << "Item with ID " << itemId << " not found.\n";
}

void Seller::updateItemPrice(int itemId, double newPrice) {
    for (auto &item : items) {
        if (item.getId() == itemId) {
            item.setPrice(newPrice);
            std::cout << "Item " << item.getName() << " price updated to Rp " << newPrice << std::endl;
            return;
        }
    }
    std::cout << "Item with ID " << itemId << " not found.\n";
}

Item* Seller::getItemById(int id) {
    for (auto& item : items) {
        if (item.getId() == id) return &item;
    }
    return nullptr;
}

json Seller::toJson() const {
    json j;
    j["id"] = sellerId;
    j["storeName"] = sellerName;
    for (const auto& it : items) j["items"].push_back(it.toJson());
    return j;
}

void Seller::fromJson(const json& j) {
    sellerId = j.value("id", 0);
    sellerName = j.value("storeName", string(""));
    items.clear();
    for (const auto& it : j["items"]) {
        Item item;
        item.fromJson(it);
        items.push_back(item);
    }
}
