#ifndef ITEM_H 
#define ITEM_H

#include <string>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

class Item {
private:
    int id;
    string name;
    int quantity;
    double price;
    bool idDisplay;
    int sellerId;

public:
    Item();
    Item(int id, const string& name, int quantity, double price);

    int getId() const;
    string getName() const;
    int getQuantity() const;
    double getPrice() const;
    int getSellerId() const;

    void setId(int newId);
    void setName(const string& newName);
    void setQuantity(int newQuantity);
    void setPrice(double newPrice);
    void setDisplay(bool display);
    void alterItemById(int itemId, const string& newName, int newQuantity, double newPrice);
    void updatePriceQuantity(int itemId, double newPrice, int newQuantity);

    // Serialization
    json toJson() const;
    void fromJson(const json& j);
};

#endif // ITEM_H