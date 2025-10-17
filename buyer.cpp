    #include "buyer.h" 
    #include "bank_customer.h"
    #include "seller.h"
    #include <iostream>

    Buyer::Buyer() : id(0), name(""), account(nullptr), sellerAccount(nullptr) {}

    Buyer::Buyer(int id, const string& name) : id(id), name(name), account(nullptr), sellerAccount(nullptr) {}

    Buyer::~Buyer() {
        // Note: ownership of account and seller handled externally in main (deleted there)
    }

    int Buyer::getId() const {
        return id;
    }

    string Buyer::getName() const {
        return name;
    }

    void Buyer::setAccount(BankCustomer* acc) {
        account = acc;
    }

    bool Buyer::hasBankAccount() const {
        return account != nullptr;
    }

    BankCustomer* Buyer::getAccount() const {
        return account;
    }

    void Buyer::setSeller(Seller* s) {
        sellerAccount = s;
    }

    bool Buyer::isSeller() const {
        return sellerAccount != nullptr;
    }

    Seller* Buyer::getSeller() const {
        return sellerAccount;
    }

    json Buyer::toJson() const {
        json j;
        j["id"] = id;
        j["name"] = name;
        if (account) j["bankAccount"] = account->toJson();
        if (sellerAccount) j["seller"] = json{
            {"id", sellerAccount->getSellerId()},
            {"storeName", sellerAccount->getSellerName()},
            {"items", json::array()}
        };
        if (sellerAccount) {
            for (const auto& it : sellerAccount->getItems()) {
                j["seller"]["items"].push_back(it.toJson());
            }
        }
        return j;
    }

    void Buyer::fromJson(const json& j) {
        id = j.value("id", 0);
        name = j.value("name", string(""));
        account = nullptr;
        sellerAccount = nullptr;
        if (j.contains("bankAccount")) {
            BankCustomer* acc = new BankCustomer();
            acc->fromJson(j["bankAccount"]);
            account = acc;
        }
        if (j.contains("seller")) {
            // create seller and populate items
            auto sdata = j["seller"];
            Seller* s = new Seller(*this, sdata.value("id", 0), sdata.value("storeName", string("")));
            if (sdata.contains("items")) {
                for (auto& it : sdata["items"]) {
                    Item item;
                    item.fromJson(it);
                    s->getItems().push_back(item);
                }
            }
            sellerAccount = s;
        }
    }

    void Buyer::addTransaction(Transaction* t) {
    transactions.push_back(t);
}

vector<Transaction*> Buyer::getTransactionsToday() const {
    vector<Transaction*> result;
    for (auto t : transactions) {
        if (t->isToday()) result.push_back(t);
    }
    return result;
}

vector<Transaction*> Buyer::getTransactionsLastMonth() const {
    vector<Transaction*> result;
    for (auto t : transactions) {
        if (t->isLastMonth()) result.push_back(t);
    }
    return result;
}