#include "buyer.h"
#include "bank_customer.h"
#include "seller.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

Buyer::Buyer() : id(0), name(""), account(nullptr), sellerAccount(nullptr) {}

Buyer::Buyer(int id, const string& name)
    : id(id), name(name), account(nullptr), sellerAccount(nullptr) {}

Buyer::~Buyer() {
}

int Buyer::getId() const { return id; }
string Buyer::getName() const { return name; }

void Buyer::setAccount(BankCustomer* acc) { account = acc; }
bool Buyer::hasBankAccount() const { return account != nullptr; }
BankCustomer* Buyer::getAccount() const { return account; }

void Buyer::setSeller(Seller* s) { sellerAccount = s; }
bool Buyer::isSeller() const { return sellerAccount != nullptr; }
Seller* Buyer::getSeller() const { return sellerAccount; }


void Buyer::addToCart(int itemId, const string& itemName, int quantity, double price, int sellerId) {
    for (auto& c : cart) {
        if (c.itemId == itemId && c.sellerId == sellerId) {
            c.quantity += quantity;
            return;
        }
    }
    CartItem newItem{itemId, itemName, quantity, price, sellerId};
    cart.push_back(newItem);
}

void Buyer::listCartItems() const {
    if (cart.empty()) {
        cout << "Your cart is empty.\n";
        return;
    }

    cout << left << setw(8) << "ID" << setw(20) << "Name"
        << setw(10) << "Qty" << setw(15) << "Price" << "SellerID\n";

    for (const auto& c : cart) {
        cout << left << setw(8) << c.itemId
            << setw(20) << c.itemName
            << setw(10) << c.quantity
            << "Rp " << setw(12) << c.price
            << c.sellerId << "\n";
    }
    cout << "Total: Rp " << getCartTotal() << "\n";
}

const vector<CartItem>& Buyer::getCart() const { return cart; }
void Buyer::clearCart() { cart.clear(); }

double Buyer::getCartTotal() const {
    double total = 0.0;
    for (const auto& c : cart) total += c.price * c.quantity;
    return total;
}

void Buyer::removeFromCart(int itemId, int sellerId) {
    cart.erase(
        remove_if(cart.begin(), cart.end(),
            [itemId, sellerId](const CartItem& c) {
                return c.itemId == itemId && c.sellerId == sellerId;
            }),
        cart.end()
    );
}



double Buyer::getBalance() const {
    return account ? account->getBalance() : 0.0;
}

void Buyer::setBalance(double amount) {
    if (account) account->setBalance(amount);
}

bool Buyer::reduceBalance(double amount) {
    if (!account) return false;
    double current = account->getBalance();
    if (current < amount) return false;
    account->setBalance(current - amount);
    return true;
}

void Buyer::increaseBalance(double amount) {
    if (account) account->setBalance(account->getBalance() + amount);
}


void Buyer::addTransaction(Transaction* t) {
    transactions.push_back(t);
}

vector<Transaction*> Buyer::getTransactions() const {
    return transactions;
}

Transaction* Buyer::findTransactionById(int transactionId) {
    for (auto* t : transactions) {
        if (t->getId() == transactionId) return t;
    }
    return nullptr;
}

vector<Transaction*> Buyer::getTransactionsToday() const {
    vector<Transaction*> result;
    for (auto t : transactions)
        if (t->isToday()) result.push_back(t);
    return result;
}

vector<Transaction*> Buyer::getTransactionsLastMonth() const {
    vector<Transaction*> result;
    for (auto t : transactions)
        if (t->isLastMonth()) result.push_back(t);
    return result;
}


json Buyer::toJson() const {
    json j;
    j["id"] = id;
    j["name"] = name;

    if (account)
        j["bankAccount"] = account->toJson();

    if (sellerAccount) {
        j["seller"] = json{
            {"id", sellerAccount->getSellerId()},
            {"storeName", sellerAccount->getSellerName()},
            {"items", json::array()}
        };
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