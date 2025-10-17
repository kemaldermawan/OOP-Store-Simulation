#ifndef BANK_CUSTOMER_H 
#define BANK_CUSTOMER_H 
#include <string> 
#include <vector> 
#include <iostream> 
#include <iomanip> 
#include "json.hpp" 

using namespace std; 
using json = nlohmann::json; 

class Buyer;
class Transaction;

class BankCustomer { 
    private: 
        int id; 
        string name; 
        string address; 
        string phone; 
        string email; 
        double balance; 
        vector<string> history; // Riwayat transaksi 
        Buyer* owner; // Pointer to the owning Buyer
        vector<Transaction*> transactions;
        
    public: 
        BankCustomer(); 
        BankCustomer(int id, 
            const string& name, 
            double initialBalance); 

            int getId() const;
            string getName() const; 
            string getAddress() const; 
            string getPhone() const; 
            string getEmail() const; 
            double getBalance() const; 
            vector<string> getHistory() const; 
            
            void setName(const string& n); 
            void setAddress(const string& a); 
            void setPhone(const string& p); 
            void setEmail(const string& e); 
            void setBalance(double b); 

            void deposit(double amount); 
            bool withdraw(double amount); 
            void printInfo() const; 
            void printHistory() const; 
            
            vector<Transaction*> getTransactionsToday() const;
            vector<Transaction*> getTransactionsLastMonth() const;

            void setOwner(Buyer* b) { owner = b; }
            void addTransaction(Transaction* t);

            // Serialization 
            json toJson() const; 
            void fromJson(const json& j);
        }; 
        
        #endif // BANK_CUSTOMER_H