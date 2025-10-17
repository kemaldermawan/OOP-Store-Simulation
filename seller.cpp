#include "seller.h"
#include "buyer.h"

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
