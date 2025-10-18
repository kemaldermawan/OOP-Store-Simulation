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
    void setSellerId(int id);
    void setDisplay(bool display);

    void alterItemById(int itemId, const string& newName, int newStock, double newPrice);
    void updatePriceStock(int itemId, double newPrice, int newStock);

    json toJson() const;
    void fromJson(const json& j);
};

#endif 