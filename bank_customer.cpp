#include "bank_customer.h" 
#include <iostream> 
#include <iomanip> 
#include <sstream> 

using namespace std; 

// Constructor
BankCustomer::BankCustomer() : id(0), name(""), address(""), phone(""), email(""), balance(0.0) {}

BankCustomer::BankCustomer(int id, const string& name, double initialBalance) : id(id), name(name), address(""), phone(""), email(""), balance(initialBalance) {
    history.push_back("Account created with initial deposit Rp " + to_string((long long)initialBalance));
}

// Getter
int BankCustomer::getId() const { return id; }
string BankCustomer::getName() const { return name; }
string BankCustomer::getAddress() const { return address; }
string BankCustomer::getPhone() const { return phone; }
string BankCustomer::getEmail() const { return email; }
double BankCustomer::getBalance() const { return balance; }
vector<string> BankCustomer::getHistory() const { return history; }

// Setter
void BankCustomer::setName(const string& n) { name = n; }
void BankCustomer::setAddress(const string& a) { address = a; }
void BankCustomer::setPhone(const string& p) { phone = p; }
void BankCustomer::setEmail(const string& e) { email = e; }
void BankCustomer::setBalance(double b) { balance = b; }

// Deposit function
void BankCustomer::deposit(double amount) {
    if (amount <= 0) {
        cout << "Invalid deposit amount." << endl;
        return;
    }
    balance += amount;
    history.push_back("Deposited Rp " + to_string((long long)amount));
    cout << "Deposit successful. New balance: Rp " << fixed << setprecision(0) << balance << endl;
}

// Withdraw function
bool BankCustomer::withdraw(double amount) {
    if (amount <= 0) {
        cout << "Invalid withdrawal amount." << endl;
        return false;
    }
    if (amount > balance) {
        cout << "Insufficient balance." << endl;
        return false;
    }
    balance -= amount;
    history.push_back("Withdrew Rp " + to_string((long long)amount));
    cout << "Withdrawal successful. New balance: Rp " << fixed << setprecision(0) << balance << endl;
    return true;
}

// Print account info
void BankCustomer::printInfo() const {
    cout << "\nBank Account Info" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Address: " << address << endl;
    cout << "Phone: " << phone << endl;
    cout << "Email: " << email << endl;
    cout << "Balance: Rp " << fixed << setprecision(0) << balance << endl;
}

// Print transaction history
void BankCustomer::printHistory() const {
    cout << "\nTransaction History" << endl;
    if (history.empty()) {
        cout << "(No transactions yet)" << endl;
    } else {
        for (const auto& h : history) {
            cout << "- " << h << endl;
        }
    }
}

// Serialization
json BankCustomer::toJson() const {
    json j;
    j["id"] = id;
    j["name"] = name;
    j["address"] = address;
    j["phone"] = phone;
    j["email"] = email;
    j["balance"] = balance;
    j["history"] = history;
    return j;
}

void BankCustomer::fromJson(const json& j) {
    id = j.value("id", 0);
    name = j.value("name", string(""));
    address = j.value("address", string(""));
    phone = j.value("phone", string(""));
    email = j.value("email", string(""));
    balance = j.value("balance", 0.0);
    history.clear();
    if (j.contains("history"))
        history = j.at("history").get<vector<string>>();
}