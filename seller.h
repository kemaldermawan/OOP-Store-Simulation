#ifndef SELLER_H
#define SELLER_H

#include <string>
#include <vector>
#include "item.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class Buyer; // forward declaration

class Seller {
private:
    Buyer* owner;
    int sellerId;
    string sellerName;
    vector<Item> items;

public:
    Seller();
    Seller(Buyer& buyer, int sellerId, const string& sellerName);
    ~Seller();

    // Getter 
    int getSellerId() const;
    string getSellerName() const;
    Buyer* getOwner() const;
    vector<Item>& getItems();

    // Item Management
    void addNewItem(int id, const string& name, int qty, double price);

    void updateItemQuantity(int itemId, int newQty);

    void updateItemPrice(int itemId, double newPrice);

    // Serialization
    json toJson() const;
    void fromJson(const json& j);
};

#endif