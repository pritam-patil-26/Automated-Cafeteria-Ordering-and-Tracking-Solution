// Cafe Ordering and Tracking Solution

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include <limits>

using namespace std;

// Forward declarations
class MenuItem;
class Order;
class Table;
class Cafe;
class User;

// MenuItem class to store menu item details
class MenuItem {
private:
    int id;
    string name;
    double price;
    string category;
    bool available;

public:
    MenuItem(int id, string name, double price, string category, bool available = true)
        : id(id), name(name), price(price), category(category), available(available) {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    string getCategory() const { return category; }
    bool isAvailable() const { return available; }

    void setPrice(double newPrice) { price = newPrice; }
    void setAvailable(bool status) { available = status; }

    void display() const {
        cout << left << setw(5) << id << setw(25) << name 
             << setw(10) << fixed << setprecision(2) << price 
             << setw(15) << category 
             << (available ? "Available" : "Not Available") << endl;
    }
};

// Order class to store order details
class Order {
private:
    int orderId;
    int tableId;
    vector<pair<MenuItem, int>> items; // MenuItem and quantity
    time_t orderTime;
    bool isPaid;
    double totalAmount;

public:
    Order(int orderId, int tableId) 
        : orderId(orderId), tableId(tableId), orderTime(time(0)), isPaid(false), totalAmount(0.0) {}

    void addItem(const MenuItem& item, int quantity) {
        items.push_back(make_pair(item, quantity));
        totalAmount += item.getPrice() * quantity;
    }

    double getTotalAmount() const { return totalAmount; }
    int getOrderId() const { return orderId; }
    int getTableId() const { return tableId; }
    bool getIsPaid() const { return isPaid; }
    time_t getOrderTime() const { return orderTime; }

    void setPaid(bool status) { isPaid = status; }

    void displayOrderItems() const {
        cout << "\n--- Order Items ---\n";
        cout << left << setw(25) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << setw(10) << "Total" << endl;
        cout << string(55, '-') << endl;
        
        for (const auto& item : items) {
            cout << left << setw(25) << item.first.getName() 
                 << setw(10) << fixed << setprecision(2) << item.first.getPrice()
                 << setw(10) << item.second
                 << setw(10) << fixed << setprecision(2) << (item.first.getPrice() * item.second) << endl;
        }
        cout << string(55, '-') << endl;
        cout << "Total Amount: Rs" << fixed << setprecision(2) << totalAmount << endl;
    }

    // For logging orders to file
    string getOrderDetailsForLog() const {
        string details = "Order #" + to_string(orderId) + ", Table #" + to_string(tableId) + ", Items: ";
        for (const auto& item : items) {
            details += item.first.getName() + " x" + to_string(item.second) + ", ";
        }
        details += "Total: RS" + to_string(totalAmount);
        return details;
    }
};

// Table class to manage cafe tables
class Table {
private:
    int tableId;
    int capacity;
    bool occupied;
    int currentOrderId;

public:
    Table(int id, int cap) : tableId(id), capacity(cap), occupied(false), currentOrderId(-1) {}

    int getId() const { return tableId; }
    int getCapacity() const { return capacity; }
    bool isOccupied() const { return occupied; }
    int getCurrentOrderId() const { return currentOrderId; }

    void occupy(int orderId) {
        occupied = true;
        currentOrderId = orderId;
    }

    void vacate() {
        occupied = false;
        currentOrderId = -1;
    }

    void display() const {
        cout << "Table #" << tableId 
             << " (Capacity: " << capacity << ") - " 
             << (occupied ? "Occupied" : "Available") << endl;
    }
};

// Cafe class to manage menu, tables, and orders
class Cafe {
private:
    string name;
    vector<MenuItem> menu;
    vector<Table> tables;
    vector<Order> orders;
    vector<Order> completedOrders;
    int nextOrderId;

public:
    Cafe(string name) : name(name), nextOrderId(1) {
        // Initialize with some sample tables
        for (int i = 1; i <= 10; i++) {
            tables.push_back(Table(i, 4)); // Default 4-seat tables
        }
    }

    string getName() const { return name; }

    void addMenuItem(const MenuItem& item) {
        menu.push_back(item);
    }

    void addTable(const Table& table) {
        tables.push_back(table);
    }

    void displayMenu() const {
        cout << "\n--- " << name << " Menu ---\n";
        cout << left << setw(5) << "ID" << setw(25) << "Name" << setw(10) << "Price" 
             << setw(15) << "Category" << "Status" << endl;
        cout << string(70, '-') << endl;
        
        for (const auto& item : menu) {
            if (item.isAvailable()) {
                item.display();
            }
        }
    }

    void displayAllMenuItems() const {
        cout << "\n--- Complete Menu (Admin View) ---\n";
        cout << left << setw(5) << "ID" << setw(25) << "Name" << setw(10) << "Price" 
             << setw(15) << "Category" << "Status" << endl;
        cout << string(70, '-') << endl;
        
        for (const auto& item : menu) {
            item.display();
        }
    }

    void displayTables() const {
        cout << "\n--- Tables ---\n";
        for (const auto& table : tables) {
            table.display();
        }
    }

    vector<Table>& getTables() {
        return tables;
    }

    Table* getTable(int tableId) {
        for (auto& table : tables) {
            if (table.getId() == tableId) {
                return &table;
            }
        }
        return nullptr;
    }

    MenuItem* getMenuItem(int id) {
        for (auto& item : menu) {
            if (item.getId() == id) {
                return &item;
            }
        }
        return nullptr;
    }

    Order* createOrder(int tableId) {
        Table* table = getTable(tableId);
        if (!table) {
            cout << "Invalid table ID!" << endl;
            return nullptr;
        }
        
        if (table->isOccupied()) {
            cout << "Table is already occupied!" << endl;
            return nullptr;
        }
        
        Order newOrder(nextOrderId++, tableId);
        orders.push_back(newOrder);
        table->occupy(newOrder.getOrderId());
        
        return &orders.back();
    }

    Order* getCurrentOrderForTable(int tableId) {
        Table* table = getTable(tableId);
        if (!table || !table->isOccupied()) {
            return nullptr;
        }
        
        int orderId = table->getCurrentOrderId();
        for (auto& order : orders) {
            if (order.getOrderId() == orderId) {
                return &order;
            }
        }
        return nullptr;
    }

    void generateBill(int orderId) {
        for (auto& order : orders) {
            if (order.getOrderId() == orderId) {
                cout << "\n\n--- BILL ---\n";
                cout << "Cafe: " << name << endl;
                cout << "Order #: " << order.getOrderId() << endl;
                cout << "Table #: " << order.getTableId() << endl;
                
                time_t orderTime = order.getOrderTime();
                cout << "Date & Time: " << ctime(&orderTime);
                
                order.displayOrderItems();
                cout << "\nThank you for visiting!\n";
                return;
            }
        }
        cout << "Order not found!" << endl;
    }

    void payBill(int tableId) {
        Order* order = getCurrentOrderForTable(tableId);
        if (!order) {
            cout << "No active order for this table!" << endl;
            return;
        }
        
        generateBill(order->getOrderId());
        cout << "\nPayment processed successfully!" << endl;
        
        order->setPaid(true);
        
        // Log the completed order
        logOrderToFile(*order);
        
        // Move to completed orders
        completedOrders.push_back(*order);
        
        // Remove from active orders
        for (auto it = orders.begin(); it != orders.end(); ++it) {
            if (it->getOrderId() == order->getOrderId()) {
                orders.erase(it);
                break;
            }
        }
        
        // Free up the table
        Table* table = getTable(tableId);
        if (table) {
            table->vacate();
        }
    }

    void logOrderToFile(const Order& order) {
        time_t now = time(0);
        struct tm* ltm = localtime(&now);
        
        string filename = "sales_" + 
                          to_string(1900 + ltm->tm_year) + "_" +
                          to_string(1 + ltm->tm_mon) + "_" +
                          to_string(ltm->tm_mday) + ".log";
        
        ofstream logFile(filename, ios::app);
        if (logFile.is_open()) {
            logFile << "[" << ctime(&now) << "] " << order.getOrderDetailsForLog() << endl;
            logFile.close();
        } else {
            cerr << "Unable to open log file!" << endl;
        }
    }

    void modifyMenuItem(int id) {
        MenuItem* item = getMenuItem(id);
        if (!item) {
            cout << "Menu item not found!" << endl;
            return;
        }
        
        int choice;
        cout << "\nCurrent details:" << endl;
        item->display();
        
        cout << "\n1. Change price\n2. Change availability\n3. Back\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1: {
                double newPrice;
                cout << "Enter new price: Rs";
                cin >> newPrice;
                item->setPrice(newPrice);
                cout << "Price updated successfully!" << endl;
                break;
            }
            case 2: {
                char status;
                cout << "Set as available? (Y/N): ";
                cin >> status;
                item->setAvailable(toupper(status) == 'Y');
                cout << "Availability updated successfully!" << endl;
                break;
            }
            case 3:
                return;
            default:
                cout << "Invalid choice!" << endl;
        }
    }

    void viewDailySales() {
        time_t now = time(0);
        struct tm* ltm = localtime(&now);
        
        string filename = "sales_" + 
                          to_string(1900 + ltm->tm_year) + "_" +
                          to_string(1 + ltm->tm_mon) + "_" +
                          to_string(ltm->tm_mday) + ".log";
        
        ifstream logFile(filename);
        if (logFile.is_open()) {
            cout << "\n--- Daily Sales Log ---\n";
            string line;
            while (getline(logFile, line)) {
                cout << line << endl;
            }
            logFile.close();
        } else {
            cout << "No sales records for today yet." << endl;
        }
    }
};

// User class for login management - Now with polymorphic methods
class User {
protected:
    string username;
    string password;
    string role;

public:
    User(string uname, string pass, string role)
        : username(uname), password(pass), role(role) {}
    
    virtual ~User() {} // Virtual destructor for proper cleanup of derived objects

    string getUsername() const { return username; }
    string getRole() const { return role; }
    
    bool authenticate(const string& pass) const {
        return password == pass;
    }

    // Polymorphic method to be overridden in derived classes
    virtual void showMenu(vector<Cafe>& cafes) = 0; // Pure virtual function
};

// Admin class derived from User
class Admin : public User {
public:
    Admin(string uname, string pass) : User(uname, pass, "admin") {}
    
    // Override showMenu with Admin implementation
    virtual void showMenu(vector<Cafe>& cafes) override {
        if (cafes.empty()) {
            cout << "No cafes available to manage!" << endl;
            return;
        }
        
        // Choose a cafe to manage
        Cafe& cafe = cafes[0]; // For simplicity, admin manages the first cafe
        
        int choice;
        do {
            cout << "\n--- Admin Menu: " << cafe.getName() << " ---\n";
            cout << "1. View All Menu Items\n";
            cout << "2. Add New Menu Item\n";
            cout << "3. Modify Menu Item\n";
            cout << "4. View Tables\n";
            cout << "5. View Daily Sales\n";
            cout << "6. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1:
                    cafe.displayAllMenuItems();
                    break;
                case 2:
                    addMenuItem(cafe);
                    break;
                case 3:
                    modifyMenuItem(cafe);
                    break;
                case 4:
                    cafe.displayTables();
                    break;
                case 5:
                    cafe.viewDailySales();
                    break;
                case 6:
                    cout << "Logging out..." << endl;
                    break;
                default:
                    cout << "Invalid choice!" << endl;
            }
        } while (choice != 6);
    }
    
private:
    void addMenuItem(Cafe& cafe) {
        int id;
        string name, category;
        double price;
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter menu item ID: ";
        cin >> id;
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter item name: ";
        getline(cin, name);
        
        cout << "Enter price: Rs";
        cin >> price;
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter category: ";
        getline(cin, category);
        
        MenuItem newItem(id, name, price, category);
        cafe.addMenuItem(newItem);
        
        cout << "Menu item added successfully!" << endl;
    }
    
    void modifyMenuItem(Cafe& cafe) {
        int id;
        cout << "Enter menu item ID to modify: ";
        cin >> id;
        
        cafe.modifyMenuItem(id);
    }
};

// Customer class derived from User
class Customer : public User {
public:
    Customer(string uname, string pass) : User(uname, pass, "customer") {}
    
    // Override showMenu with Customer implementation
    virtual void showMenu(vector<Cafe>& cafes) override {
        if (cafes.empty()) {
            cout << "No cafes available!" << endl;
            return;
        }
        
        // Choose a cafe
        cout << "\n--- Available Cafes ---\n";
        for (size_t i = 0; i < cafes.size(); i++) {
            cout << i+1 << ". " << cafes[i].getName() << endl;
        }
        
        int cafeChoice;
        cout << "Select a cafe (1-" << cafes.size() << "): ";
        cin >> cafeChoice;
        
        if (cafeChoice < 1 || cafeChoice > static_cast<int>(cafes.size())) {
            cout << "Invalid choice!" << endl;
            return;
        }
        
        Cafe& selectedCafe = cafes[cafeChoice-1];
        customerCafeMenu(selectedCafe);
    }
    
private:
    void customerCafeMenu(Cafe& cafe) {
        int tableId = -1;
        Order* currentOrder = nullptr;
        
        int choice;
        do {
            cout << "\n--- Customer Menu: " << cafe.getName() << " ---\n";
            if (tableId == -1) {
                cout << "1. Reserve a Table\n";
            } else {
                cout << "1. View Menu\n";
                cout << "2. Order Food\n";
                cout << "3. View Current Order\n";
                cout << "4. Request Bill and Pay\n";
            }
            cout << "5. Leave Cafe (Exit)\n";
            cout << "Enter your choice: ";
            cin >> choice;
            
            if (tableId == -1) {
                if (choice == 1) {
                    tableId = reserveTable(cafe);
                    if (tableId != -1) {
                        currentOrder = cafe.createOrder(tableId);
                    }
                } else if (choice == 5) {
                    cout << "Thank you for visiting!" << endl;
                } else {
                    cout << "Please reserve a table first!" << endl;
                }
            } else {
                switch (choice) {
                    case 1:
                        cafe.displayMenu();
                        break;
                    case 2:
                        orderFood(cafe, currentOrder);
                        break;
                    case 3:
                        if (currentOrder) {
                            currentOrder->displayOrderItems();
                        }
                        break;
                    case 4:
                        if (currentOrder) {
                            cafe.payBill(tableId);
                            tableId = -1;
                            currentOrder = nullptr;
                        }
                        break;
                    case 5:
                        if (currentOrder) {
                            cout << "Please pay your bill before leaving!" << endl;
                        } else {
                            cout << "Thank you for visiting!" << endl;
                        }
                        break;
                    default:
                        cout << "Invalid choice!" << endl;
                }
            }
        } while (choice != 5 || (tableId != -1 && currentOrder != nullptr));
    }
    
    int reserveTable(Cafe& cafe) {
        cout << "\n--- Available Tables ---\n";
        bool tablesAvailable = false;
        
        for (const auto& table : cafe.getTables()) {
            if (!table.isOccupied()) {
                table.display();
                tablesAvailable = true;
            }
        }
        
        if (!tablesAvailable) {
            cout << "Sorry, no tables are available at the moment." << endl;
            return -1;
        }
        
        int tableChoice;
        cout << "Select a table (Enter table number): ";
        cin >> tableChoice;
        
        Table* selectedTable = cafe.getTable(tableChoice);
        if (!selectedTable) {
            cout << "Invalid table number!" << endl;
            return -1;
        }
        
        if (selectedTable->isOccupied()) {
            cout << "This table is already occupied. Please select another table." << endl;
            return -1;
        }
        
        cout << "Table #" << tableChoice << " reserved successfully!" << endl;
        return tableChoice;
    }
    
    void orderFood(Cafe& cafe, Order* order) {
        if (!order) {
            cout << "No active order!" << endl;
            return;
        }
        
        cafe.displayMenu();
        
        char continueOrdering;
        do {
            int itemId, quantity;
            
            cout << "Enter item ID to order: ";
            cin >> itemId;
            
            MenuItem* item = cafe.getMenuItem(itemId);
            if (!item) {
                cout << "Invalid item ID!" << endl;
                continue;
            }
            
            if (!item->isAvailable()) {
                cout << "This item is currently not available." << endl;
                continue;
            }
            
            cout << "Enter quantity: ";
            cin >> quantity;
            
            if (quantity <= 0) {
                cout << "Quantity must be positive!" << endl;
                continue;
            }
            
            order->addItem(*item, quantity);
            cout << quantity << "x " << item->getName() << " added to your order." << endl;
            
            cout << "Add more items? (Y/N): ";
            cin >> continueOrdering;
        } while (toupper(continueOrdering) == 'Y');
    }
};

// User management class
class UserManager {
private:
    vector<User*> users;
    
public:
    UserManager() {
        // Add default admin
        users.push_back(new Admin("admin", "admin123"));
        
        // Add default customer
        users.push_back(new Customer("customer", "customer123"));
    }
    
    ~UserManager() {
        for (auto user : users) {
            delete user;
        }
    }
    
    User* authenticate(const string& username, const string& password) {
        for (auto user : users) {
            if (user->getUsername() == username && user->authenticate(password)) {
                return user;
            }
        }
        return nullptr;
    }
};

// Main function
int main() {
    // Initialize cafes
    vector<Cafe> cafes;
    
    Cafe cafe1("Coffee Heaven");
    // Add sample menu items
    cafe1.addMenuItem(MenuItem(1, "Espresso", 250, "Coffee"));
    cafe1.addMenuItem(MenuItem(2, "Cappuccino", 150, "Coffee"));
    cafe1.addMenuItem(MenuItem(3, "Latte", 200, "Coffee"));
    cafe1.addMenuItem(MenuItem(4, "Croissant", 100, "Pastry"));
    cafe1.addMenuItem(MenuItem(5, "Chocolate Cake", 300, "Dessert"));
    
    cafes.push_back(cafe1);
    
    Cafe cafe2("Tea Time");
    cafe2.addMenuItem(MenuItem(1, "Green Tea", 20, "Tea"));
    cafe2.addMenuItem(MenuItem(2, "Black Tea", 15, "Tea"));
    cafe2.addMenuItem(MenuItem(3, "Lemon Tea", 10, "Tea"));
    
    cafes.push_back(cafe2);
    
    // Initialize user manager
    UserManager userManager;
    
    string username, password;
    cout << "=== Cafe Ordering and Tracking System ===\n";
    
    while (true) {
        cout << "\n--- Login ---\n";
        cout << "Username (or 'exit' to quit): ";
        cin >> username;
        
        if (username == "exit") {
            break;
        }
        
        cout << "Password: ";
        cin >> password;
        
        User* user = userManager.authenticate(username, password);
        
        if (!user) {
            cout << "Invalid username or password. Please try again." << endl;
            continue;
        }
        
        cout << "Login successful! Welcome, " << user->getUsername() << " (" << user->getRole() << ")" << endl;
        
        // Using polymorphism to show the appropriate menu
        user->showMenu(cafes);
    }
    
    cout << "Thank you for using the Cafe Management System. Goodbye!" << endl;
    return 0;
}