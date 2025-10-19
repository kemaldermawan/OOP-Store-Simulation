#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <fstream>
#include "bank_customer.h"
#include "buyer.h"
#include "seller.h"
#include "transaction.h"
#include "data_manager.h"
#include "store_manager.h"
#include "bank_manager.h"

using namespace std;

enum PrimaryPrompt { LOGIN, REGISTER, STORE_ADMIN, BANK_ADMIN, EXIT };
vector<Buyer*> allBuyers;
vector<Transaction*> allTransactions;
Buyer* currentBuyer = nullptr;

void menuBuyer(Buyer &buyer);
void menuSeller(Seller &seller, const vector<Buyer*>& allBuyers);

Buyer* findBuyerById(int id) {
    for (auto b : allBuyers) {
        if (b->getId() == id) return b;
    }
    return nullptr;
}

int nextTransactionId = 1;

string getTodayDate() {
    time_t t = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));
    return string(buf);
}

string todayDate = getTodayDate();

inline long days_since(const string& date_str) {
    tm t = {};
    istringstream ss(date_str);
    ss >> get_time(&t, "%Y-%m-%d");
    if (ss.fail()) { return -1; }
    auto transaction_time = chrono::system_clock::from_time_t(mktime(&t));
    auto now = chrono::system_clock::now();
    return chrono::duration_cast<chrono::hours>(now - transaction_time).count() / 24;
}

void loadApplicationData() {
    ifstream file("database.json");
    if (!file.is_open()) {
        cout << "(No existing database found. Starting fresh.)\n";
        return;
    }

    json j;
    file >> j;
    file.close();

    map<int, Transaction*> transactionMap;

    if (j.contains("transactions")) {
        for (const auto& t_json : j["transactions"]) {
            Transaction* t = new Transaction();
            t->fromJson(t_json);
            allTransactions.push_back(t);
            transactionMap[t->getId()] = t;
        }
    }

    if (j.contains("buyers")) {
        for (const auto& b_json : j["buyers"]) {
            Buyer* buyer = new Buyer();
            buyer->fromJson(b_json);
            allBuyers.push_back(buyer);

            for(Transaction* t : allTransactions){
                if(t->getBuyerId() == buyer->getId() || t->getSellerId() == buyer->getId()){
                    buyer->addTransaction(t);
                    if(buyer->isSeller() && t->getSellerId() == buyer->getId()){
                        buyer->getSeller()->addTransaction(t);
                    }
                }
            }
        }
    }
    cout << "(Loaded " << allBuyers.size() << " buyers and " << allTransactions.size() << " transactions from file)\n";
}

void menuBanking(Buyer &buyer) {
    int choice = 0;
    BankCustomer* account = buyer.getAccount();
    if (!account) {
        cout << "No bank account attached.\n";
        return;
    }

    while (choice != 5) {
        cout << "\nBanking Menu:\n";
        cout << "1. Check Balance\n";
        cout << "2. Transaction History\n";
        cout << "3. Deposit Money\n";
        cout << "4. Withdraw Money\n";
        cout << "5. Exit to Buyer Menu\n";
        cout << "Select an option: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                cout << "\nYour Current Balance: Rp "
                    << fixed << setprecision(0)
                    << account->getBalance() << endl;
                break;

            case 2: {
                int subChoice;
                cout << "1. Today\n2. Last Month\nSelect: ";
                cin >> subChoice;
                cin.ignore();
                
                vector<Transaction*> txs;
                if (subChoice == 1) txs = buyer.getTransactionsToday();
                else if (subChoice == 2) txs = buyer.getTransactionsLastMonth();

                cout << "\nTransactions:\n";
                for (auto t : txs) {
                    cout << "ID: " << t->getId()
                        << ", Amount: " << t->getAmount()
                        << ", Status: " << t->getStatus()
                        << ", Date: " << t->getDate() << endl;
                }
                break;
            }

            case 3: {
                double amount;
                cout << "Enter Deposit Amount: ";
                cin >> amount;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                account->deposit(amount);
                break;
            }

            case 4: {
                double amount;
                cout << "Enter Withdraw Amount: ";
                cin >> amount;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                account->withdraw(amount);
                break;
            }

            case 5:
                cout << "Returning to Buyer Menu...\n";
                return;

            default:
                cout << "Invalid choice, try again.\n";
        }
    }
}

void menuBuyer(Buyer &buyer) {
    int choice = 0;
    while (choice != 9) {
        cout << "\nBuyer Menu:\n";
        cout << "1. Check Account Status\n";
        cout << "2. Upgrade Account to Seller\n";
        cout << "3. Create Banking Account / Banking Functions\n";
        cout << "4. Browse Store\n";
        cout << "5. Order\n";
        cout << "6. Payment\n";
        cout << "7. Check Spending in Last K Days\n";
        cout << "8. Logout\n";
        cout << "9. Delete Account\n";
        cout << "Select an option: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                cout << "\nCheck Account Status selected.\n";
                cout << "Name: " << buyer.getName() << endl;
                if (buyer.hasBankAccount()) {
                    cout << "Bank Account: Exists\n";
                    cout << "Balance: Rp "
                        << fixed << setprecision(0)
                        << buyer.getAccount()->getBalance() << endl;
                } else {
                    cout << "Bank Account: Not created\n";
                }
                if (buyer.isSeller()) {
                    cout << "Seller Account: Exists\n";
                    cout << "Store Name: " << buyer.getSeller()->getSellerName() << endl;
                } else {
                    cout << "Seller Account: Not created\n";
                }
                break;

            case 2:
                cout << "\nUpgrade Account to Seller selected.\n";
                if (!buyer.hasBankAccount()) {
                    cout << "Cannot upgrade: No banking account linked.\n";
                } else if (buyer.isSeller()) {
                    cout << "Already a Seller.\n";
                } else {
                    string storeName, storeAddress, storePhone, storeEmail;
                    cout << "Enter Store Name: ";
                    getline(cin, storeName);
                    cout << "Enter Store Address: ";
                    getline(cin, storeAddress);
                    cout << "Enter Store Phone: ";
                    getline(cin, storePhone);
                    cout << "Enter Store Email: ";
                    getline(cin, storeEmail);

                    Seller* seller = new Seller(buyer, buyer.getId(), storeName);
                    buyer.setSeller(seller);
                    cout << "Seller Account created successfully.\n";
                }
                break;

            case 3:
                cout << "\nBanking Functions selected.\n";
                if (!buyer.hasBankAccount()) {
                    cout << "No bank account found. Create one now.\n";

                    string address, phone, email;
                    double deposit;
                    cout << "Enter Address: ";
                    getline(cin, address);
                    cout << "Enter Phone Number: ";
                    getline(cin, phone);
                    cout << "Enter Email: ";
                    getline(cin, email);
                    cout << "Enter Initial Deposit Amount: ";
                    cin >> deposit;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    BankCustomer* account = new BankCustomer(buyer.getId(), buyer.getName(), deposit);
                    account->setAddress(address);
                    account->setPhone(phone);
                    account->setEmail(email);

                    buyer.setAccount(account);
                    cout << "Banking Account created successfully.\n";
                } else {
                    menuBanking(buyer);
                }
                break;

                case 4: {
                    cout << "\nBrowse Store selected.\n";
                    currentBuyer = &buyer;

                    vector<Seller*> availableSellers;
                    for (auto bptr : allBuyers) {
                        if (bptr->isSeller()) availableSellers.push_back(bptr->getSeller());
                    }

                    if (availableSellers.empty()) {
                        cout << "No stores available.\n";
                        break;
                    }

                    cout << "\nAvailable Stores:\n";
                    for (size_t i = 0; i < availableSellers.size(); ++i) {
                        cout << i+1 << ". " << availableSellers[i]->getSellerName()
                            << " (Seller ID: " << availableSellers[i]->getSellerId() << ")\n";
                    }

                    int storeChoice;
                    cout << "\nSelect store by number (0 to cancel): ";
                    cin >> storeChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (storeChoice == 0) break;
                    if (storeChoice < 1 || storeChoice > static_cast<int>(availableSellers.size())) {
                        cout << "Invalid store selection.\n";
                        break;
                    }

                    Seller* selectedSeller = availableSellers[storeChoice - 1];
                    auto &itemsRef = selectedSeller->getItems();
                    if (itemsRef.empty()) {
                        cout << "This store has no items.\n";
                        break;
                    }

                    bool browsing = true;
                    while (browsing) {
                        cout << "\nStore Inventory: " << selectedSeller->getSellerName() << "\n";
                        cout << "ID\tName\t\tQuantity\tPrice\n";
                        for (const auto& it : itemsRef) {
                            cout << it.getId() << "\t" << it.getName()
                                << "\t\t" << it.getQuantity()
                                << "\t\tRp " << fixed << setprecision(0) << it.getPrice() << "\n";
                        }

                        int itemId;
                        cout << "\nEnter Item ID to view (0 to exit store): ";
                        cin >> itemId;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (itemId == 0) break;

                        Item* selItem = selectedSeller->getItemById(itemId);
                        if (!selItem) {
                            cout << "Item not found.\n";
                            continue;
                        }

                        cout << "\nItem Details:\n";
                        cout << "ID: " << selItem->getId() << "\n";
                        cout << "Name: " << selItem->getName() << "\n";
                        cout << "Quantity: " << selItem->getQuantity() << "\n";
                        cout << "Price: Rp " << fixed << setprecision(0) << selItem->getPrice() << "\n";

                        char add;
                        cout << "Add to cart? (y/n): ";
                        cin >> add;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (add == 'y' || add == 'Y') {
                            int qty;
                            cout << "Enter Quantity: ";
                            cin >> qty;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            if (qty <= 0) {
                                cout << "Invalid quantity.\n";
                            } else if (qty > selItem->getQuantity()) {
                                cout << "Not enough stock. Available: " << selItem->getQuantity() << "\n";
                            } else {
                                currentBuyer->addToCart(selItem->getId(), selItem->getName(), qty, selItem->getPrice(), selectedSeller->getSellerId());
                                cout << "Item added to cart successfully.\n";
                            }
                        }

                        char cont;
                        cout << "Continue browsing this store? (y/n): ";
                        cin >> cont;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (!(cont == 'y' || cont == 'Y')) browsing = false;
                    }
                    break;
                }

                case 5: {
                    cout << "\nOrder selected.\n";
                    if (!currentBuyer) {
                        cout << "No buyer logged in currently.\n";
                        break;
                    }

                    const vector<CartItem>& cart = currentBuyer->getCart();
                    if (cart.empty()) {
                        cout << "Your cart is empty.\n";
                        break;
                    }

                    cout << "\nYour Cart:\n";
                    currentBuyer->listCartItems();
                    double total = currentBuyer->getCartTotal();
                    cout << "Cart Total: Rp " << fixed << setprecision(0) << total << "\n";

                    cout << "\n1. Remove item from cart\n2. Checkout (generate invoice(s))\n3. Back\nSelect option: ";
                    int orderOpt;
                    cin >> orderOpt;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (orderOpt == 1) {
                        int remItemId, remSellerId;
                        cout << "Enter Item ID to remove: ";
                        cin >> remItemId;
                        cout << "Enter Seller ID of that item: ";
                        cin >> remSellerId;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        currentBuyer->removeFromCart(remItemId, remSellerId);
                        cout << "Item removed from cart if it existed.\n";
                    }
                    else if (orderOpt == 2) {
                        map<int, vector<CartItem>> itemsBySeller;
                        for (const auto& ci : cart) {
                            itemsBySeller[ci.sellerId].push_back(ci);
                        }

                        for (auto const& [sellerId, sellerItems] : itemsBySeller) {
                            Buyer* sellerOwner = findBuyerById(sellerId);
                            if (!sellerOwner || !sellerOwner->isSeller()) {
                                cout << "Seller ID " << sellerId << " not found; skipping.\n";
                                continue;
                            }
                            Seller* sellerObj = sellerOwner->getSeller();

                            double transactionAmount = 0;
                            vector<OrderItem> transactionItems;
                            bool stockSufficient = true;

                            for (const auto& ci : sellerItems) {
                                Item* itemPtr = sellerObj->getItemById(ci.itemId);
                                if (!itemPtr || ci.quantity > itemPtr->getQuantity()) {
                                    cout << "Stock for item " << ci.itemName << " is insufficient; skipping transaction with this seller.\n";
                                    stockSufficient = false;
                                    break;
                                }
                                transactionAmount += ci.price * ci.quantity;
                                transactionItems.push_back({ci.itemId, ci.quantity, ci.price});
                            }

                            if (stockSufficient) {
                                for (const auto& oi : transactionItems) {
                                    sellerObj->getItemById(oi.itemId)->setQuantity(sellerObj->getItemById(oi.itemId)->getQuantity() - oi.quantity);
                                }

                                Transaction* tx = new Transaction(
                                    nextTransactionId++,
                                    currentBuyer->getId(),
                                    sellerId,
                                    transactionAmount,
                                    todayDate,
                                    string("Unpaid"),
                                    transactionItems
                                );
                                allTransactions.push_back(tx);
                                currentBuyer->addTransaction(tx);
                                sellerObj->addTransaction(tx);
                            }
                        }

                        currentBuyer->clearCart();
                        cout << "Checkout complete. Invoice(s) created with status 'Unpaid'.\n";
                        cout << "Go to Payment menu (option 6) to pay invoices.\n";
                    }
                    break;
                }

                case 6: {
                    cout << "\nPayment selected.\n";
                    if (!currentBuyer) {
                        cout << "No buyer logged in.\n";
                        break;
                    }
                    BankCustomer* buyerAcc = currentBuyer->getAccount();
                    if (!buyerAcc) {
                        cout << "You have no bank account. Create one first.\n";
                        break;
                    }

                    vector<Transaction*> unpaid;
                    auto allTx = currentBuyer->getTransactionsToday();
                    auto allBuyerTxs = currentBuyer->getTransactions();
                    for (auto t : allBuyerTxs) {
                        if (t->getStatus() == "Unpaid" || t->getStatus() == "unpaid") unpaid.push_back(t);
                    }

                    if (unpaid.empty()) {
                        cout << "No unpaid invoices.\n";
                        break;
                    }

                    cout << "Unpaid Invoices:\n";
                    cout << "InvoiceID\tSellerID\tAmount\tDate\n";
                    for (auto t : unpaid) {
                        cout << t->getTransactionId() << "\t\t" << t->getSellerId()
                            << "\t\tRp " << fixed << setprecision(0) << t->getAmount()
                            << "\t" << t->getDate() << "\n";
                    }

                    cout << "\nEnter Invoice ID to pay (0 to cancel): ";
                    int invId;
                    cin >> invId;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (invId == 0) break;

                    Transaction* selectedTx = nullptr;
                    for (auto t : unpaid) {
                        if (t->getTransactionId() == invId) { selectedTx = t; break; }
                    }
                    if (!selectedTx) {
                        cout << "Invoice not found.\n";
                        break;
                    }

                    cout << "\nInvoice Details:\n";
                    cout << "InvoiceID: " << selectedTx->getTransactionId() << "\n";
                    cout << "SellerID: " << selectedTx->getSellerId() << "\n";
                    cout << "Amount: Rp " << fixed << setprecision(0) << selectedTx->getAmount() << "\n";
                    cout << "Date: " << selectedTx->getDate() << "\n";
                    cout << "Your current balance: Rp " << fixed << setprecision(0) << buyerAcc->getBalance() << "\n";

                    cout << "Proceed with payment? (y/n): ";
                    char conf; cin >> conf; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (!(conf=='y' || conf=='Y')) { cout << "Payment cancelled.\n"; break; }

                    double amount = selectedTx->getAmount();
                    if (buyerAcc->getBalance() < amount) {
                        cout << "Insufficient balance.\n";
                        break;
                    }

                    Buyer* sellerBuyer = findBuyerById(selectedTx->getSellerId());
                    if (!sellerBuyer) {
                        cout << "Seller not found.\n";
                        break;
                    }
                    BankCustomer* sellerAcc = sellerBuyer->getAccount();
                    if (!sellerAcc) {
                        cout << "Seller has no bank account. Cannot pay.\n";
                        break;
                    }

                    bool withdrawOk = buyerAcc->withdraw(amount);
                    if (!withdrawOk) {
                        cout << "Withdrawal failed.\n";
                        break;
                    }
                    sellerAcc->deposit(amount);

                    selectedTx->setStatus("Paid");

                    cout << "Payment successful. Rp " << fixed << setprecision(0) << amount
                        << " transferred to Seller ID " << selectedTx->getSellerId() << "\n";

                    DataManager::saveAllData(allBuyers);
                    break;
                }

            case 7: {
                cout << "\nCheck Spending selected\n";
                int k;
                cout << "Enter number of days (k): ";
                cin >> k;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                double totalSpending = 0.0;
                for (const auto* t : buyer.getTransactions()) {
                    if ((t->getStatus() == "Paid" || t->getStatus() == "Completed") && days_since(t->getDate()) <= k) {
                        totalSpending += t->getAmount();
                    }
                }

                cout << "Total spending in the last " << k << " days: Rp " << fixed << setprecision(0) << totalSpending << endl;
                break;
            }

            case 8:
                cout << "Logout selected.\n";
                choice = 9;
                break;

            case 9:
                cout << "Delete Account selected.\n";
                cout << "Are you sure you want to delete your account? This action cannot be undone. (y/n): ";
                char confirm;
                cin >> confirm;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (confirm == 'y' || confirm == 'Y') {
                    auto it = std::find(allBuyers.begin(), allBuyers.end(), &buyer);
                    if (it != allBuyers.end()) {

                        delete buyer.getAccount();
                        delete buyer.getSeller();
                        
                        allBuyers.erase(it);
                        
                        cout << "Account deleted successfully.\n";
                    } else {
                        cout << "Error: Buyer not found in system.\n";
                    }
                    return;
                } else {
                    cout << "Account deletion cancelled.\n";
                }
                break;

            default:
                cout << "Invalid option. Please try again.\n";
        }
    }
}

void menuSeller(Seller &seller, const vector<Buyer*>& allBuyers) {
    int choice = 0;
    while (choice != 9) {
        cout << "\nSeller Menu:\n";
        cout << "1. Check Inventory\n";
        cout << "2. Add Item to Inventory\n";
        cout << "3. Remove Item from Inventory\n";
        cout << "4. Update Item in Inventory\n";
        cout << "5. View Orders\n";
        cout << "6. Complete an Order\n";
        cout << "7. Discover Top K Most Popular Items (This Month)\n"; // TAMBAHAN
        cout << "8. Discover Loyal Customer (This Month)\n";
        cout << "9. Exit to Main Menu\n";
        cout << "Select an option: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                cout << "\nCheck Inventory selected.\n";
                if (seller.getItems().empty()) {
                    cout << "Inventory is empty.\n";
                } else {
                    cout << "Current Inventory:\n";
                    for (const auto& item : seller.getItems()) {
                        cout << "Item ID: " << item.getId()
                            << ", Name: " << item.getName()
                            << ", Quantity: " << item.getQuantity()
                            << ", Price: Rp " << fixed << setprecision(0)
                            << item.getPrice() << endl;
                    }
                }
                break;

            case 2: {
                cout << "Add Item to Inventory selected.\n";
                int id, qty;
                double price;
                string name;
                cout << "Enter Item ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter Item Name: ";
                getline(cin, name);
                cout << "Enter Quantity: ";
                cin >> qty;
                cout << "Enter Price: ";
                cin >> price;
                seller.addNewItem(id, name, qty, price);
                cout << "Item added successfully.\n";
                break;
            }

            case 3: {
                cout << "Remove Item from Inventory selected.\n";

                vector<Item>& items = seller.getItems();
                if (items.empty()) {
                    cout << "Inventory is empty. Nothing to remove.\n";
                    break;
                }

                cout << "Current Inventory:\n";
                for (const auto& item : items) {
                    cout << "Item ID: " << item.getId()
                        << ", Name: " << item.getName()
                        << ", Quantity: " << item.getQuantity()
                        << ", Price: Rp " << fixed << setprecision(0)
                        << item.getPrice() << endl;
                }

                int removeId;
                cout << "Enter the Item ID to remove: ";
                cin >> removeId;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                bool found = false;
                for (auto it = items.begin(); it != items.end(); ++it) {
                    if (it->getId() == removeId) {
                        items.erase(it);
                        found = true;
                        cout << "Item removed successfully.\n";
                        break;
                    }
                }

                if (!found) {
                    cout << "Item with ID " << removeId << " not found.\n";
                }

                break;
            }

            case 4: { 
                cout << "Update Item in Inventory selected.\n";
                if (seller.getItems().empty()) {
                    cout << "Inventory is empty. Nothing to update.\n";
                    break;
                }

                cout << "Current Inventory:\n";
                cout << "ID\tName\t\tQuantity\tPrice\n";
                for (const auto &item : seller.getItems()) {
                    cout << item.getId() << "\t" 
                        << item.getName() << "\t\t"
                        << item.getQuantity() << "\t\tRp " 
                        << item.getPrice() << "\n";
                }
                int id;
                cout << "Enter Item ID to update: ";
                cin >> id;
                int updateChoice;
                cout << "1. Replenish Quantity\n2. Change Price\nSelect: ";
                cin >> updateChoice;
                if (updateChoice == 1) {
                    int qty;
                    cout << "Enter new quantity: ";
                    cin >> qty;
                    seller.updateItemQuantity(id, qty);
                } else if (updateChoice == 2) {
                    double price;
                    cout << "Enter new price: ";
                    cin >> price;
                    seller.updateItemPrice(id, price);
                } else {
                    cout << "Invalid option.\n";
                }
                break;
            }

            case 5: {
                cout << "\nAll Orders for " << seller.getSellerName() << "\n";
                const auto& transactions = seller.getTransactions();
                if (transactions.empty()) {
                    cout << "No orders yet.\n";
                } else {
                    for (const auto* t : transactions) {
                        cout << "ID: " << t->getTransactionId() << ", Buyer ID: " << t->getBuyerId()
                            << ", Amount: Rp " << t->getAmount() << ", Status: " << t->getStatus()
                            << ", Date: " << t->getDate() << endl;
                    }
                }
                break;
            }

            case 6: {
                cout << "\nComplete an Order\n";
                vector<Transaction*> paid_transactions;
                for (auto* t : seller.getTransactions()) {
                    if (t->getStatus() == "Paid") {
                        paid_transactions.push_back(t);
                    }
                }

                if (paid_transactions.empty()) {
                    cout << "No paid orders to complete.\n";
                    break;
                }

                cout << "Paid Orders:\n";
                for (const auto* t : paid_transactions) {
                    cout << "ID: " << t->getTransactionId() << ", Buyer ID: " << t->getBuyerId()
                        << ", Amount: Rp " << t->getAmount() << endl;
                }

                cout << "\nEnter Transaction ID to mark as 'Completed' (0 to cancel): ";
                int transId;
                cin >> transId;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (transId == 0) break;

                bool found = false;
                for (auto* t : paid_transactions) {
                    if (t->getTransactionId() == transId) {
                        t->setStatus("Completed");
                        found = true;
                        cout << "Transaction " << transId << " has been marked as completed.\n";
                        break;
                    }
                }

                if (!found) {
                    cout << "Paid transaction with that ID not found.\n";
                }
                break;
            }

            case 7: {
                cout << "\nTop K Most Popular Items (This Month)\n";
                int k;
                cout << "Enter number of top items to show (k): ";
                cin >> k;

                string currentMonth = getTodayDate().substr(0, 7);
                map<int, int> itemSales; 

                for (const auto* t : seller.getTransactions()) {
                    if ((t->getStatus() == "Paid" || t->getStatus() == "Completed") && t->getDate().substr(0, 7) == currentMonth) {
                        for (const auto& item : t->getItems()) {
                            itemSales[item.itemId] += item.quantity;
                        }
                    }
                }

                if (itemSales.empty()) {
                    cout << "No items sold this month.\n";
                    break;
                }

                vector<pair<int, int>> sortedItems(itemSales.begin(), itemSales.end());
                sort(sortedItems.begin(), sortedItems.end(), [](const auto& a, const auto& b){
                    return a.second > b.second;
                });

                cout << "Top " << k << " items sold in " << currentMonth << ":\n";
                int rank = 1;
                for (const auto& p : sortedItems) {
                    if (rank > k) break;
                    Item* itemInfo = seller.getItemById(p.first);
                    string itemName = itemInfo ? itemInfo->getName() : "Unknown Item";
                    cout << rank++ << ". " << itemName << " (ID: " << p.first << ") - Sold: " << p.second << " units.\n";
                }
                break;
            }

            case 8: {
                cout << "\nLoyal Customer (This Month)\n";
                
                string currentMonth = getTodayDate().substr(0, 7);
                map<int, double> customerSpending;

                for (const auto* t : seller.getTransactions()) {
                    if ((t->getStatus() == "Paid" || t->getStatus() == "Completed") && t->getDate().substr(0, 7) == currentMonth) {
                        customerSpending[t->getBuyerId()] += t->getAmount();
                    }
                }

                if (customerSpending.empty()) {
                    cout << "No customers made a purchase this month.\n";
                    break;
                }

                pair<int, double> topCustomer = {0, 0.0};
                for(const auto& pair : customerSpending) {
                    if (pair.second > topCustomer.second) {
                        topCustomer = pair;
                    }
                }

                string customerName = "Unknown (ID: " + to_string(topCustomer.first) + ")";
                for(const auto* b : allBuyers) {
                    if(b->getId() == topCustomer.first) {
                        customerName = b->getName();
                        break;
                    }
                }

                cout << "The most loyal customer in " << currentMonth << " is:\n";
                cout << "Name: " << customerName << "\n";
                cout << "Total Spending: Rp " << fixed << setprecision(0) << topCustomer.second << endl;
                break;
            }

            case 9:
                cout << "Exiting to Main Menu...\n";
                return;

            default:
                cout << "Invalid option. Please try again.\n";
        }
    }
}

void loadAllData(vector<Buyer*>& buyers, vector<Transaction*>& transactions) {
    ifstream file("database.json");
    if (!file.is_open()) {
        cout << "No existing database found. Starting fresh.\n";
        return;
    }

    json j;
    file >> j;
    file.close();

    map<int, Transaction*> transactionMap;

    if (j.contains("transactions")) {
        for (const auto& t_json : j["transactions"]) {
            Transaction* t = new Transaction();
            t->fromJson(t_json);
            transactions.push_back(t);
            transactionMap[t->getId()] = t;
        }
    }
    
    if (j.contains("buyers")) {
        for (const auto& b_json : j["buyers"]) {
            Buyer* buyer = new Buyer();
            buyer->fromJson(b_json);
            buyers.push_back(buyer);
        }
    }
}

int main() {
    PrimaryPrompt prompt = LOGIN;
    int nextBuyerId = 1;

    loadApplicationData();

    if (!allBuyers.empty()) {
        nextBuyerId = allBuyers.back()->getId() + 1;
    } else {
    }

    while (prompt != EXIT) {
        cout << "\nSelect an option:\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Store Admin Menu\n";
        cout << "4. Bank Admin Menu\n";
        cout << "5. Exit\n";

        cout << "Select an option: ";
        int choice;
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice < 1 || choice > 5) {
            cout << "Invalid option.\n";
            continue;
        }
        prompt = static_cast<PrimaryPrompt>(choice - 1);

        switch (prompt) {
            case LOGIN: {
                int id;
                cout << "Login selected.\n";
                cout << "Enter Buyer ID: ";
                cin >> id;
                cin.ignore();

                Buyer* found = findBuyerById(id);
                if (!found) {
                    cout << "Buyer not found.\n";
                } else {
                    cout << "Login successful as " << found->getName() << endl;
                    if (found->isSeller()) {
                        cout << "This account has both Buyer and Seller roles.\n";
                        cout << "1. Login as Buyer\n2. Login as Seller\nSelect: ";
                        int roleChoice;
                        cin >> roleChoice;
                        cin.ignore();
                        if (roleChoice == 2) {
                            menuSeller(*found->getSeller(), allBuyers);
                        } else {
                            menuBuyer(*found);
                        }
                    } else {
                        menuBuyer(*found);
                    }
                }
                break;
            }

            case REGISTER: {
                cout << "Register selected.\n";
                string name, address, phone, email;
                cout << "Register as Buyer\n";
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Home Address: ";
                getline(cin, address);
                cout << "Enter Phone Number: ";
                getline(cin, phone);
                cout << "Enter Email: ";
                getline(cin, email);

                Buyer* newBuyer = new Buyer(nextBuyerId++, name);
                allBuyers.push_back(newBuyer);
                currentBuyer = newBuyer;
                cout << "Registration successful! Your Buyer ID is " << newBuyer->getId() << endl;

                cout << "Do you want to create a Banking Account? (y/n): ";
                char bankChoice;
                cin >> bankChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                bool hasBank = false;
                if (bankChoice == 'y' || bankChoice == 'Y') {
                    double initialDeposit;
                    cout << "Enter Initial Deposit Amount: ";
                    cin >> initialDeposit;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    BankCustomer* bankAccount = new BankCustomer(newBuyer->getId(), name, initialDeposit);

                    bankAccount->setAddress(address);
                    bankAccount->setPhone(phone);
                    bankAccount->setEmail(email);

                    currentBuyer->setAccount(bankAccount);
                    hasBank = true;
                    cout << "Banking Account created successfully with initial deposit of "
                        << fixed << setprecision(0) << initialDeposit << endl;
                } else {
                    cout << "Banking Account creation skipped.\n";
                    cout << "You cannot upgrade to Seller without a Banking Account.\n";
                }

                if (hasBank) {
                    cout << "Do you want to create a Seller Account? (y/n): ";
                    char sellerChoice;
                    cin >> sellerChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (sellerChoice == 'y' || sellerChoice == 'Y') {
                        string storeName, storeAddress, storePhone, storeEmail;
                        cout << "Enter Store Name: ";
                        getline(cin, storeName);
                        cout << "Enter Store Address: ";
                        getline(cin, storeAddress);
                        cout << "Enter Store Phone Number: ";
                        getline(cin, storePhone);
                        cout << "Enter Store Email: ";
                        getline(cin, storeEmail);

                        Seller* seller = new Seller(*currentBuyer, currentBuyer->getId(), storeName);
                        currentBuyer->setSeller(seller);
                        cout << "Seller Account created successfully.\n";
                    } else {
                        cout << "Seller Account creation skipped.\n";
                    }
                } else {
                    cout << "Cannot create Seller Account without a Banking Account.\n";
                }
                break;
            }

            case STORE_ADMIN: {
                StoreManager::showStoreAdminMenu(allBuyers, allTransactions);
                break;
            }

            case BANK_ADMIN: { 
                BankManager::showBankAdminMenu(allBuyers, allTransactions);
                break;
            }

            case EXIT:
                cout << "Exiting.\n";
                break;

            default:
                cout << "Invalid option.\n";
                break;
        }
    }

    DataManager::saveAllData(allBuyers);

    json j_out;
    for (auto* b : allBuyers) {
        j_out["buyers"].push_back(b->toJson());
    }
    for (auto* t : allTransactions) {
        j_out["transactions"].push_back(t->toJson());
    }
    ofstream file("database.json");
    file << setw(4) << j_out;
    file.close();

    for (auto b : allBuyers) {
        delete b; 
    }
    for (auto t : allTransactions) {
        delete t;
    }
    allBuyers.clear();
    allTransactions.clear();

    return 0;
}