#include "item.h"

Item::Item() : id(0), name(""), quantity(0), price(0.0), idDisplay(false) {}

Item::Item(int id, const string& name, int quantity, double price) : id(id), name(name), quantity(quantity), price(price), idDisplay(false) {}

int Item::getId() const { return id; }

const string& Item::getName() const { return name; }

int Item::getQuantity() const { return quantity; }

double Item::getPrice() const { return price; }

void Item::setId(int newId) { id = newId; }

void Item::setName(const string& newName) { name = newName; }

void Item::setQuantity(int newQuantity) { quantity = newQuantity; }

void Item::setPrice(double newPrice) { price = newPrice; }

void Item::setDisplay(bool display) { idDisplay = display; }

void Item::alterItemById(int itemId, const string& newName, int newQuantity, double newPrice) {
    if (id == itemId) {
        name = newName;
        quantity = newQuantity;
        price = newPrice;
    }
}

void Item::updatePriceQuantity(int itemId, double newPrice, int newQuantity) {
    if (id == itemId) {
        price = newPrice;
        quantity = newQuantity;
    }
}

json Item::toJson() const {
    json j;
    j["id"] = id;
    j["name"] = name;
    j["quantity"] = quantity;
    j["price"] = price;
    return j;
}

void Item::fromJson(const json& j) {
    id = j.value("id", 0);
    name = j.value("name", string(""));
    quantity = j.value("quantity", 0);
    price = j.value("price", 0.0);
}
