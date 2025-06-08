#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <ctime>
#include <sstream>

using namespace std;

class Product {
public:
    int id;
    string name;
    float price;
    int quantity;

    Product(int i, string n, float p, int q) : id(i), name(n), price(p), quantity(q) {}

    void display() const {
        cout << setw(5) << id << setw(25) << name
             << setw(10) << fixed << setprecision(2) << price
             << setw(10) << quantity << endl;
    }
};

vector<Product> products = {
    {1, "Smartphone", 499.99f, 10},
    {2, "Laptop", 899.99f, 5},
    {3, "Headphones", 149.99f, 20},
    {4, "Smartwatch", 199.99f, 15}
};

class CartItem {
public:
    int id;
    string name;
    float price;
    int quantity;

    CartItem(int i, string n, float p, int q) : id(i), name(n), price(p), quantity(q) {}

    void display() const {
        cout << setw(5) << id << setw(25) << name
             << setw(10) << fixed << setprecision(2) << price
             << setw(10) << quantity
             << setw(12) << price * quantity << endl;
    }
};

vector<CartItem> cart;

void listProducts() {
    cout << "\nAvailable Products:\n";
    cout << setw(5) << "ID" << setw(25) << "Name"
         << setw(10) << "Price" << setw(10) << "Qty\n";
    for (const auto& p : products) {
        p.display();
    }
}

void resetProductIDs() {
    int id = 1;
    for (auto& p : products) {
        p.id = id++;
    }
}

void addMultipleProducts() {
    int count;
    cout << "How many products do you want to add? ";
    while (!(cin >> count) || count <= 0) {
        cout << "Invalid input. Enter positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();

    for (int i = 0; i < count; i++) {
        string name;
        float price;
        int quantity;

        cout << "Enter name for product #" << (i+1) << ": ";
        getline(cin, name);
        while (name.empty()) {
            cout << "Name cannot be empty. Enter name: ";
            getline(cin, name);
        }

        cout << "Enter price for product #" << (i+1) << ": ";
        while (!(cin >> price) || price < 0) {
            cout << "Invalid input. Enter positive price: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter quantity for product #" << (i+1) << ": ";
        while (!(cin >> quantity) || quantity < 0) {
            cout << "Invalid input. Enter quantity >= 0: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore();

        int newID = products.empty() ? 1 : products.back().id + 1;
        products.push_back(Product(newID, name, price, quantity));
        cout << "Product added successfully.\n";
    }
}

void deleteMultipleProducts() {
    if (products.empty()) {
        cout << "No products to delete.\n";
        return;
    }

    cout << "Enter product IDs to delete (space-separated): ";
    string line;
    getline(cin, line);
    if (line.empty()) {
        cout << "No IDs entered.\n";
        return;
    }

    istringstream iss(line);
    vector<int> idsToDelete;
    int id;

    while (iss >> id) {
        idsToDelete.push_back(id);
    }

    if (idsToDelete.empty()) {
        cout << "No valid IDs entered.\n";
        return;
    }

    // Remove duplicates from idsToDelete
    sort(idsToDelete.begin(), idsToDelete.end());
    idsToDelete.erase(unique(idsToDelete.begin(), idsToDelete.end()), idsToDelete.end());

    // Delete products with matching IDs
    int deletedCount = 0;
    for (int delID : idsToDelete) {
        auto it = find_if(products.begin(), products.end(),
                          [delID](const Product& p) { return p.id == delID; });
        if (it != products.end()) {
            products.erase(it);
            deletedCount++;
        } else {
            cout << "Product ID " << delID << " not found.\n";
        }
    }

    if (deletedCount > 0) {
        resetProductIDs();
        cout << deletedCount << " product(s) deleted and IDs reset.\n";
    } else {
        cout << "No products were deleted.\n";
    }
}

void editProduct() {
    int id;
    cout << "Enter product ID to edit: ";
    while (!(cin >> id)) {
        cout << "Invalid input. Enter a number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();

    auto it = find_if(products.begin(), products.end(),
                      [id](const Product& p) { return p.id == id; });

    if (it == products.end()) {
        cout << "Product not found.\n";
        return;
    }

    string name;
    float price;
    int quantity;

    cout << "Editing product '" << it->name << "'. Press enter to keep current values.\n";

    cout << "Enter new name (or press Enter to keep: " << it->name << "): ";
    getline(cin, name);
    if (!name.empty())
        it->name = name;

    cout << "Enter new price (or -1 to keep: " << it->price << "): ";
    while (!(cin >> price)) {
        cout << "Invalid input. Enter price or -1 to keep: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (price >= 0)
        it->price = price;

    cout << "Enter new quantity (or -1 to keep: " << it->quantity << "): ";
    while (!(cin >> quantity)) {
        cout << "Invalid input. Enter quantity or -1 to keep: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (quantity >= 0)
        it->quantity = quantity;

    cin.ignore();
    cout << "Product updated.\n";
}

bool updateStock(int id, int qty) {
    for (auto& p : products) {
        if (p.id == id) {
            if (p.quantity >= qty) {
                p.quantity -= qty;
                return true;
            } else {
                cout << "Not enough stock for " << p.name << ". Available: " << p.quantity << "\n";
                return false;
            }
        }
    }
    cout << "Product not found.\n";
    return false;
}

void addMultipleToCart() {
    if (products.empty()) {
        cout << "No products available to buy.\n";
        return;
    }

    while (true) {
        int id, qty;
        cout << "Enter product ID to buy (or 0 to stop): ";
        while (!(cin >> id)) {
            cout << "Invalid input. Enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (id == 0) {
            cin.ignore();
            break;  // Stop adding products
        }

        cout << "Enter quantity to buy: ";
        while (!(cin >> qty) || qty <= 0) {
            cout << "Invalid input. Enter positive quantity: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore();

        auto it = find_if(products.begin(), products.end(),
                          [id](const Product& p) { return p.id == id; });

        if (it == products.end()) {
            cout << "Product not found.\n";
            continue;
        }

        if (updateStock(id, qty)) {
            // Check if already in cart, update qty
            auto cit = find_if(cart.begin(), cart.end(),
                               [id](const CartItem& c) { return c.id == id; });

            if (cit != cart.end()) {
                cit->quantity += qty;
            } else {
                cart.push_back(CartItem(id, it->name, it->price, qty));
            }
            cout << "Added to cart.\n";
        }
    }
}

void viewCart() {
    if (cart.empty()) {
        cout << "Cart is empty.\n";
        return;
    }

    cout << "\nYour Cart:\n";
    cout << setw(5) << "ID" << setw(25) << "Name"
         << setw(10) << "Price" << setw(10) << "Qty" << setw(12) << "Total\n";
    float grandTotal = 0;
    for (const auto& item : cart) {
        item.display();
        grandTotal += item.price * item.quantity;
    }
    cout << "Grand Total: $" << fixed << setprecision(2) << grandTotal << "\n";
}

void printReceipt() {
    if (cart.empty()) {
        cout << "Cart is empty. Nothing to print.\n";
        return;
    }

    time_t now = time(0);
    tm* local = localtime(&now);

    cout << "\n--- Receipt ---\n";
    cout << "Date: " << 1900 + local->tm_year << "-"
         << 1 + local->tm_mon << "-"
         << local->tm_mday << "\n";

    cout << setw(5) << "ID" << setw(25) << "Name"
         << setw(10) << "Price" << setw(10) << "Qty" << setw(12) << "Total\n";

    float total = 0;
    for (const auto& item : cart) {
        item.display();
        total += item.price * item.quantity;
    }

    cout << "Grand Total: $" << fixed << setprecision(2) << total << "\n";
    cout << "Thank you for your purchase!\n\n";

    cart.clear();
}

bool adminLogin() {
    string username, password;
    cout << "Enter Admin Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    return username == "admin" && password == "admin123";
}

void adminMenu() {
    int choice;
    do {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. List Products\n";
        cout << "2. Add Products\n";
        cout << "3. Edit Product\n";
        cout << "4. Delete Products\n";
        cout << "5. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: listProducts(); break;
            case 2: addMultipleProducts(); break;
            case 3: editProduct(); break;
            case 4: deleteMultipleProducts(); break;
            case 5: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

void userMenu() {
    int choice;
    do {
        cout << "\n--- User Menu ---\n";
        cout << "1. List Products\n";
        cout << "2. Buy Products\n";
        cout << "3. View Cart\n";
        cout << "4. Print Receipt & Checkout\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: listProducts(); break;
            case 2: addMultipleToCart(); break;
            case 3: viewCart(); break;
            case 4: printReceipt(); break;
            case 5: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

int main() {
    cout << "Welcome to the Online Shopping Cart System!\n";

    int choice;
    do {
        cout << "\n--- Main Menu ---\n";
        cout << "1. Admin Login\n";
        cout << "2. User Access\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                if (adminLogin())
                    adminMenu();
                else
                    cout << "Invalid username or password.\n";
                break;
            case 2:
                userMenu();
                break;
            case 3:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 3);

    return 0;
}
