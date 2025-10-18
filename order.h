#ifndef ORDER_H
#define ORDER_H

#include <vector>
#include "item.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

struct OrderItem {
    int itemId;
    int quantity;
    double price;

    json toJson() const {
        json j;
        j["itemId"] = itemId;
        j["quantity"] = quantity;
        j["price"] = price;
        return j;
    }

    void fromJson(const json& j) {
        itemId = j.value("itemId", 0);
        quantity = j.value("quantity", 0);
        price = j.value("price", 0.0);
    }
};

#endif