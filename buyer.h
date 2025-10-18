    #ifndef BUYER_H
    #define BUYER_H

    #include <string>
    #include <vector>
    #include "transaction.h"
    #include "item.h"
    #include "json.hpp"
    using namespace std;
    using json = nlohmann::json;

    class BankCustomer;
    class Seller;

    struct CartItem {
        int itemId;
        string itemName;
        int quantity;
        double price;
        int sellerId;
    };
    class Buyer {
    private:
        int id;
        string name;
        BankCustomer* account;
        Seller* sellerAccount;
        vector<Transaction*> transactions;
        vector<CartItem> cart;

    public:
        Buyer();
        Buyer(int id, const string& name);
        ~Buyer();

        int getId() const;
        string getName() const;

        void setAccount(BankCustomer* acc);
        bool hasBankAccount() const;
        BankCustomer* getAccount() const;

        void setSeller(Seller* s);
        bool isSeller() const;
        Seller* getSeller() const;

        void addToCart(int itemId, const string& itemName, int quantity, double price, int sellerId);
        void listCartItems() const;
        const vector<CartItem>& getCart() const;
        void clearCart();
        double getCartTotal() const;
        void removeFromCart(int itemId, int sellerId);

        void addTransaction(Transaction* t);
        vector<Transaction*> getTransactionsToday() const;
        vector<Transaction*> getTransactionsLastMonth() const;

        // Serialization
        json toJson() const;
        void fromJson(const json& j);
    };

    #endif // BUYER_H