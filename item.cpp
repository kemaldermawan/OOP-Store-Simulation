#include "item.h"
#include <iostream>
using namespace std;

Item::Item()
    : id(0), name(""), quantity(0), price(0.0), idDisplay(false), sellerId(0) {}

Item::Item(int id, const string& name, int quantity, double price)
    : id(id), name(name), quantity(quantity), price(price),  idDisplay(false), sellerId(0) {}

int Item::getId() const { 
    return id; 
}

string Item::getName() const { 
    return name; 
}

int Item::getQuantity() const { 
    return quantity; 
}

double Item::getPrice() const { 
    return price; 
}

int Item::getSellerId() const { 
    return sellerId; 
}

void Item::setId(int newId) { 
    id = newId; 
}

void Item::setName(const string& newName) { 
    name = newName; 
}

void Item::setQuantity(int newQuantity) { 
    quantity = newQuantity; 
}

void Item::setPrice(double newPrice) { 
    price = newPrice; 
}

void Item::setSellerId(int id) { 
    sellerId = id; 
}

void Item::setDisplay(bool display) { 
    idDisplay = display; 
}

void Item::alterItemById(int itemId, const string& newName, int newQuantity, double newPrice) {
    if (id == itemId) {
        name = newName;
        quantity = newQuantity;
        price = newPrice;
    }
}

void Item::updatePriceStock(int itemId, double newPrice, int newStock) {
    if (id == itemId) {
        price = newPrice;
        quantity = newStock;
    }
}

json Item::toJson() const {
    json j;
    j["id"] = id;
    j["name"] = name;
    j["quantity"] = quantity;
    j["price"] = price;
    j["sellerId"] = sellerId;
    return j;
}

void Item::fromJson(const json& j) {
    id = j.value("id", 0);
    name = j.value("name", string(""));
    quantity = j.value("quantity", 0);
    price = j.value("price", 0.0);
    sellerId = j.value("sellerId", 0);
}