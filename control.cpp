#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
using namespace std;

/*--------------------------------------------------------------------------------------------*/
typedef struct {
    string id;
    string name;
    double price;
    int quantity;
    string expiryDate; // Format: YYYY-MM-DD
} Product;

typedef struct {
    string username;
    string password;
    string name;
    string role;
} User;

typedef enum {
    OBJECT = 0,
    FOOD = 1,
    DRINK = 2
} Category;

/*--------------------------------------------------------------------------------------------*/

void saveSaleRecord(const vector<pair<int, Product>> &basket);
void updateStock(const vector<pair<int, Product>> &basket, vector<Product>* products[], int numCategories);
void checkout(vector<pair<int, Product>> &basket, vector<Product>* products[], int numCategories);
void calculate(const vector<pair<int, Product>> &basket);
void removeFromBasket(vector<pair<int, Product>> &basket);
void showBatch(const vector<pair<int, Product>>& basket);
void addToBasket(vector<pair<int, Product>> &basket, vector<Product>* products[], int numCategories);
void initialize();
void pause();
void log(string user, string action, string details, string timestamp);
bool login(User &currentUser);
void logout(User &currentUser);
void staffPrograms(User &currentUser);
void adminPrograms(User &currentUser);
bool checkFiles(string fileName);
void createUserFile();
void createProductsFile();
void loadProducts(vector<Product>* products[], int numCategories);
void viewProducts(vector<Product>* products[], int numCategories);
void sellProducts(vector<Product>* products[], int numCategories);
void showExpiryProducts(vector<Product>* products[], int numCategories);
void showLowStockProducts(vector<Product>* products[], int numCategories);
void showAllProducts(vector<Product>* products[], int numCategories);
void showProduct(vector<Product>* products, string category);
void searchByID(vector<Product>* products[], int numCategories);
void searchByName(vector<Product>* products[], int numCategories);
void manageProducts(vector<Product>* products[], int numCategories);
void manageUsers();
void viewReports(vector<Product>* products[], int numCategories);
void checkLog();
void addProduct(vector<Product>* products[], int numCategories);
void updateProduct(vector<Product>* products[], int numCategories);
void deleteProduct(vector<Product>* products[], int numCategories);
void loadUsers(vector<User> &users);
void viewUsers(const vector<User> &users);
void addUser(vector<User> &users);
void updateUser(vector<User> &users);
void deleteUser(vector<User> &users);
string getCurrentTime();


/*--------------------------------------------------------------------------------------------*/

const string PRODUCT_FILE = "products.dat";
const string LOG_FILE = "activity.log";
const string SALES_FILE = "sales.dat";
const string USER_FILE = "user.dat";
const int ID_WIDTH = 10;
const int NAME_WIDTH = 20;
const int PRICE_WIDTH = 10;
const int QUANTITY_WIDTH = 10;
const int EXPIRY_WIDTH = 15;
const int TOTAL_WIDTH = ID_WIDTH + NAME_WIDTH + PRICE_WIDTH + QUANTITY_WIDTH + EXPIRY_WIDTH;
const string LINE(TOTAL_WIDTH, '-');


/*--------------------------------------------------------------------------------------------*/

int main() {
    User currentUser;
    char choice;

    // Initialize program.
    initialize();

    do {
        cout << "PRODUCT MANAGING PROGRAMS" << endl;
        cout << LINE << endl;
        cout << "Please login to continue." << endl;

        while (login(currentUser) == false) {
            cout << "\nLogin failed. Invalid username or password." << endl;
            cout << "Enter anykey to try again or 'q' to quit.\n";
            cin.get(choice);
            if (choice == 'q' || choice == 'Q') break;
            system("cls");
        }

        if (currentUser.role == "staff" || currentUser.role == "admin") {
            log(currentUser.username, "Logged in", "Successfully Login", getCurrentTime());
            cout << "\nLogin successful. Welcome, " << currentUser.name << "!" << endl;
            pause();
        }
        else break;


        if (currentUser.role == "staff") {
            staffPrograms(currentUser);
        }
        else if (currentUser.role == "admin") {
            adminPrograms(currentUser);
        } 
        else {
            cout << "ERROR!, Unauthorized role." << endl;
            break;
        }

        // After logout, ask if want to login again or exit
        cout << "Do you want to login again? (y/n): ";
        cin.get(choice);
        if (choice == 'y' || choice == 'Y') {
            system("cls");
        } else {
            break;
        }
    } while (true);

    cout << "Exiting program. Goodbye!" << endl;
    return 0;
}


/*--------------------------------------------------------------------------------------------*/
/*-------------------------------------DEFINE FUNCTION----------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

void initialize() {
    // Initialize program setting.
    // Check the files.
    // Create default data if not exist.

    if (! checkFiles(USER_FILE)) createUserFile();
    if (! checkFiles(PRODUCT_FILE)) createProductsFile();

    return;
}

bool checkFiles(string fileName) {
    // Check the existance of files
    // Return true if exist

    bool flag;
    ifstream inputFile;

    inputFile.open(fileName);
    if (inputFile.is_open()) flag = true;
    else flag = false;
    inputFile.close();

    return flag;
}

void createUserFile() {
    // Create a starting user data.
    ofstream outputFile;

    outputFile.open(USER_FILE.c_str());
    outputFile << "admin1,1234,Administrator,admin\n"
               << "staff1,1234,Staff,staff\n";
    outputFile.close();
    return;
}

void createProductsFile() {
    ofstream o(PRODUCT_FILE);
    if (o.is_open()) {
        o << "O1,Notebook,35.50,20,-\n";
        o << "O2,Pencil,5.00,100,-\n";
        o << "O3,Pen,12.75,50,-\n";
        o << "O4,Eraser,8.25,40,-\n";
        o << "O5,Ruler,15.00,30,-\n";
        o << "F1,Burger,120.00,30,2025-10-25\n";
        o << "F2,Pizza,200.00,20,2025-10-31\n";
        o << "F3,Fried Rice,80.00,50,2025-10-15\n";
        o << "F4,Salad,70.00,40,2025-10-24\n";
        o << "F5,Sandwich,90.00,25,2025-12-31\n";
        o << "F6,Spaghetti,110.00,30,2025-12-31\n";
        o << "D1,Water,10.00,200,2025-10-11\n";
        o << "D2,Juice,25.00,120,2025-12-31\n";
        o << "D3,Milk,20.00,100,2025-10-21\n";
        o << "D4,Soda,22.00,150,2025-12-31\n";
        o << "D5,Tea,15.00,180,2025-10-22\n";
        o << "D6,Coffee,30.00,80,2025-12-31\n";
        o.close();
    }
}

/*--------------------------------------------------------------------------------------------*/
string getCurrentTime() {

    time_t now = std::time(nullptr);

    tm *ltm = std::localtime(&now);

    ostringstream oss;
    oss << std::put_time(ltm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void pause() {
    cin.clear();
    cin.ignore(INT32_MAX, '\n');
    cin.get();
}

/*--------------------------------------------------------------------------------------------*/

bool login(User &currentUser) {

    string inputUsername, inputPassword;
    bool authenticated = false;
    vector<User> users;
    ifstream userFile(USER_FILE);
    string line;
    // Read all users from file
    while (getline(userFile, line)) {
        stringstream ss(line);
        string username, password, name, role;
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, name, ',');
        getline(ss, role, ',');
        users.push_back({username, password, name, role});
    }
    userFile.close();

    // Prompt for login
    cout << "\n--- LOGIN ---" << endl;
    cout << "\nUsername: ";
    getline(cin, inputUsername);
    cout << "Password: ";
    getline(cin, inputPassword);

    // Check credentials
    for (const auto& user : users) {
        if (user.username == inputUsername && user.password == inputPassword) {
            currentUser = user;
            authenticated = true;
            break;
        }
    }

    return authenticated;
}

/*--------------------------------------------------------------------------------------------*/

void log(string user, string action, string details, string timestamp) {
    ofstream logFile(LOG_FILE, ios::app);
    if (logFile.is_open()) {
        logFile << timestamp << "," << user << "," << action << "," << details << endl;
        logFile.close();
    }
}

void logout(User &currentUser) {
    currentUser = {"", "", "", ""};
}

void loadProducts(vector<Product>* products[], int numCategories) {
    ifstream inputFile(PRODUCT_FILE);
    string line;
    Category object = OBJECT;
    Category food = FOOD;
    Category drink = DRINK;


    while (getline(inputFile, line)) {
        stringstream ss(line);
        Product prod;
        getline(ss, prod.id, ',');
        getline(ss, prod.name, ',');
        ss >> prod.price;
        ss.ignore();
        ss >> prod.quantity;
        ss.ignore();
        getline(ss, prod.expiryDate, ',');
        switch (prod.id[0]) {
            case 'O':
                products[OBJECT] -> push_back(prod);
                break;
            case 'F':
                products[FOOD] -> push_back(prod);
                break;
            case 'D':
                products[DRINK] -> push_back(prod);
                break;
        }

    }
    inputFile.close();
}


/*--------------------------------------------------------------------------------------------*/

void staffPrograms(User &currentUser) {
    string line;
    int choice;
    vector<Product> objects, foods, drinks;
    vector<Product> *products[3] = {&objects, &foods, &drinks};
    const int NUM_CATEGORIES = 3;

    // First load existing products from files
    loadProducts(products, NUM_CATEGORIES);


    // Display menu until logout
    do {
        system("cls");
        do {
            // Display menu
            cout << "CURRENT USER: " << currentUser.name << " (" << currentUser.role << ")" << endl;
            cout << LINE << endl;
            cout << "STAFF PROGRAMS" << endl;
            cout << LINE << endl;
            cout << "1. Sell Product." << endl;
            cout << "2. View Product." << endl;
            cout << "3. Products Near Expiry" << endl;
            cout << "4. Products Low in stocks" << endl;
            cout << "5. Logout" << endl;
            cout << "\nEnter your choice: ";
            cin >> choice;
            cin.ignore();
            if (choice < 1 || choice > 5) {
                cout << "Invalid choice. Please try again." << endl;
                pause();
                system("cls");
            }
        } while (choice < 1 || choice > 5);
    
        switch (choice) {
            case 1:
                // Sell Product
                sellProducts(products, NUM_CATEGORIES);
                break;
            case 2:
                // View Product
                viewProducts(products, NUM_CATEGORIES);
                break;
            case 3:
                // Products Near Expiry
                showExpiryProducts(products, NUM_CATEGORIES);
                break;
            case 4:
                // Products Low in stocks
                showLowStockProducts(products, NUM_CATEGORIES);
                break;
            case 5:
                cout << "Logging out..." << endl;
                logout(currentUser);
                log(currentUser.username, "Logged out", "User logged out", getCurrentTime());
                break;
            default:
                cout << "ERROR" << endl;
        }

        pause();
    } while (choice != 5);
}

/*--------------------------------------------------------------------------------------------*/

void sellProducts(vector<Product>* products[], int numCategories) {
    // Current batch sell product
    Product testProduct = {"C2", "MD", 44.50, 20, "-"};
    vector<pair<int, Product>> basket;
    int choice;

    do {
        system("cls");
        cout << "SELL PRODUCT" << endl;
        cout << LINE << endl;
        cout << "**THE RECORD WILL NOT BE SAVED UNTIL CHECKOUT**" << endl;
        showBatch(basket);

        cout << endl << LINE << endl;
        cout << "1. Add product to basket.\n"
             << "2. Remove product from basket.\n"
             << "3. Checkout.\n"
             << "4. Cancel and return to previous menu.\n";
            
        cout << "Enter choice: ";
        cin >> choice;

        if (choice < 1 || choice > 4) {
            cout << "Invalid choice." << endl;
            continue;
        }
        

        switch (choice) {
            case 1:
                addToBasket(basket, products, numCategories);
                pause();
                break;
            case 2:
                removeFromBasket(basket);
                pause();
                break;
            case 3:
                checkout(basket, products, numCategories);
                pause();
                break;
            case 4:
                basket.clear();
                cout << "Return to previous menu..." << endl;

        }

    } while (choice != 4);

}

void showBatch(const vector<pair<int, Product>>& basket) {
    cout << "\nCurrent Basket:\n";
    if (basket.empty()) {
        cout << "(empty)\n";
        return;
    }
    cout << left << setw(20) << "Product"
         << setw(10) << "Price"
         << setw(10) << "Qty"
         << setw(10) << "Total" << endl;
    for (auto &item : basket) {
        const Product &p = item.second;
        int qty = item.first;
        cout << left << setw(20) << p.name
             << setw(10) << p.price
             << setw(10) << qty
             << setw(10) << p.price * qty << endl;
    }
}

void addToBasket(vector<pair<int, Product>> &basket,
                 vector<Product>* products[], int numCategories) {
    string productName;
    cout << "Enter product name: ";
    cin.ignore(); // flush newline
    getline(cin, productName);

    // find product in all categories
    Product* found = nullptr;
    for (int i = 0; i < numCategories && !found; i++) {
        for (auto &p : *products[i]) { // dereference the vector pointer
            if (p.name == productName) {
                found = &p;
                break;
            }
        }
    }

    if (!found) {
        cout << "Product not found.\n";
        return;
    }

    cout << "Enter quantity to add: ";
    int qty;
    cin >> qty;

    if (qty <= 0) {
        cout << "Invalid quantity.\n";
        return;
    }

    if (qty > found->quantity) {
        cout << "Not enough stock. Available: " << found->quantity << "\n";
        return;
    }

    // check if already in basket
    bool inBasket = false;
    for (auto &item : basket) {
        if (item.second.id == found->id) { // match by id
            // update quantity
            item.first += qty;
            inBasket = true;
            break;
        }
    }

    if (!inBasket) {
        basket.push_back({qty, *found});
    }

    cout << "Added " << qty << " of " << found->name << " to basket.\n";
}

void removeFromBasket(vector<pair<int, Product>> &basket) {
    if (basket.empty()) {
        cout << "Basket is empty.\n";
        return;
    }

    // show current basket first
    showBatch(basket);

    string productToDelete;
    cout << "Enter product name to remove from basket: ";
    cin.ignore();  // flush newline left by previous cin
    getline(cin, productToDelete);

    // find and erase
    bool found = false;
    for (auto it = basket.begin(); it != basket.end(); ++it) {
        if (it->second.name == productToDelete) {
            basket.erase(it);
            cout << "Removed " << productToDelete << " from basket.\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Product \"" << productToDelete << "\" not found in basket.\n";
    }
}

void checkout(vector<pair<int, Product>> &basket, vector<Product>* products[], int numCategories) {

    calculate(basket);
    updateStock(basket, products, numCategories);
    saveSaleRecord(basket);
    basket.clear();
}

void calculate(const vector<pair<int, Product>> &basket) {
    if (basket.empty()) {
        cout << "Basket is empty.\n";
        return;
    }

    double total = 0;
    cout << "\n---- RECEIPT ----\n";
    cout << left << setw(20) << "Product"
         << setw(10) << "Qty"
         << setw(10) << "Price"
         << setw(10) << "Subtotal" << endl;

    for (auto &item : basket) {
        int qty = item.first;
        const Product &p = item.second;
        double sub = qty * p.price;
        total += sub;
        cout << left << setw(20) << p.name
             << setw(10) << qty
             << setw(10) << p.price
             << setw(10) << sub << endl;
    }
    cout << "-------------------------\n";
    cout << "Total: " << total << "\n";
}

void updateStock(const vector<pair<int, Product>> &basket, vector<Product>* products[], int numCategories) {

    int numItem = basket.size();
    for (auto &item : basket) {
        int soldQty = item.first;
        const Product &soldProd = item.second;

        // find matching product in inventory
        for (int i = 0; i < numCategories; i++) {
            for (auto &p : *products[i]) {
                if (numItem <= 0) break;
                if (p.id == soldProd.id) { // or compare by name
                    if (p.quantity >= soldQty) {
                        p.quantity -= soldQty;
                    } else {
                        // If basket qty > stock, zero it out
                        p.quantity = 0;
                    }

                    numItem--;
                }
                
            }
        }
    }

    ofstream prodFile(PRODUCT_FILE);
    for (int i = 0; i < numCategories; i++) {
        for (auto &p : *products[i]) {
            prodFile << p.id << "," << p.name << "," << p.price << "," << p.quantity << "," << p.expiryDate << endl;
        }
    }
    prodFile.close();
    cout << "Stock updated.\n";

}

void saveSaleRecord(const std::vector<std::pair<int, Product>> &basket) {
    // define the struct
    struct SaleRecord {
        std::string saleID;
        std::string timestamp;
        std::string totalSales;
    };

    // generate timestamp
    std::string timestr = getCurrentTime(); // "YYYY-MM-DD HH:MM:SS"

    // extract parts by substrings:
    std::string y  = timestr.substr(0,4);
    std::string mo = timestr.substr(5,2);
    std::string d  = timestr.substr(8,2);
    std::string h  = timestr.substr(11,2);
    std::string mn = timestr.substr(14,2);
    std::string s  = timestr.substr(17,2);

    // build saleID = y+mo+d+h+mn+s
    std::string saleID = y + mo + d + h + mn + s;

    // compute total sales amount
    double total = 0;
    for (auto &item : basket) {
        total += item.first * item.second.price;
    }

    SaleRecord record{saleID, timestr, std::to_string(total)};

    // append to file
    ofstream out(SALES_FILE, ios_base::app);
    if (!out) {
        cerr << "Error opening sales files for writing.\n";
        return;
    }

    // write header of this sale
    out << record.saleID 
        << "," << record.timestamp 
        << "," << record.totalSales << "\n";

    // write each product line
    for (auto &item : basket) {
        out << item.second.id << "," 
            << item.second.name << "," 
            << item.first << "," 
            << item.second.price << "," 
            << item.first * item.second.price << "\n";
    }

    out << "----\n";
    out.close();
    cout << "Sale recorded to sales.txt (Sale ID " << record.saleID << ")\n";
}
/*--------------------------------------------------------------------------------------------*/

void viewProducts(vector<Product>* products[], int numCategories) {
    int choice;

    do {
        system("cls");

        do {
            // Menu for viewing products
            cout << "VIEW PRODUCTS" << endl;
            cout << LINE << endl;
            cout << "1. All products" << endl;
            cout << "2. Objects" << endl;
            cout << "3. Foods" << endl;
            cout << "4. Drinks" << endl;
            cout << "5. Search by ID" << endl;
            cout << "6. Search by Name" << endl;
            cout << "7. Return to previous menu" << endl;
            cout << "\nEnter your choice: ";
            cin >> choice;
        
            if (choice < 1 || choice > 7) {
                cout << "Invalid choice. Please try again." << endl;
                pause();
                system("cls");
            }
        } while (choice < 1 || choice > 7);

        switch (choice) {
            case 1:
                // View all products
                showAllProducts(products, numCategories);
                pause();
                break;
            case 2:
                // View objects products
                showProduct(products[OBJECT], "Objects");
                pause();
                break;
            case 3:
                // View foods products
                showProduct(products[FOOD], "Foods");
                pause();
                break;
            case 4:
                // View drinks products
                showProduct(products[DRINK], "Drinks");
                pause();
                break;
            case 5:
                // Search by ID
                searchByID(products, numCategories);
                pause();
                break;
            case 6:
                // Search by Name
                searchByName(products, numCategories);
                pause();
                break;
            case 7:
                cout << "Returning to previous menu..." << endl;

                break;
            default:
                cout << "ERROR" << endl;
        }

    } while (choice != 7);
    
}

void showAllProducts(vector<Product>* products[], int numCategories) {
    cout << "\nALL PRODUCTS" << endl;
    cout << LINE << endl;
    cout << left << setw(ID_WIDTH) << "ID"
         << setw(NAME_WIDTH) << "Name"
         << setw(PRICE_WIDTH) << "Price"
         << setw(QUANTITY_WIDTH) << "Quantity"
         << setw(EXPIRY_WIDTH) << "Expiry Date" << endl;
    cout << LINE << endl;

    for (int i = 0; i < numCategories; i++) {
        for (const auto& prod : *products[i]) {
            cout << left << setw(ID_WIDTH) << prod.id
                 << setw(NAME_WIDTH) << prod.name
                 << setw(PRICE_WIDTH) << fixed << setprecision(2) << prod.price
                 << setw(QUANTITY_WIDTH) << prod.quantity
                 << setw(EXPIRY_WIDTH) << prod.expiryDate << endl;
        }
    }

}

void showProduct(vector<Product>* products, string category) {

    // Header
    cout << LINE << endl;
    cout << setw(LINE.size()/2 + category.size()) << category << endl;
    cout << LINE << endl;
    cout << left << setw(ID_WIDTH) << "ID"
         << setw(NAME_WIDTH) << "Name"
         << setw(PRICE_WIDTH) << "Price"
         << setw(QUANTITY_WIDTH) << "Quantity"
         << setw(EXPIRY_WIDTH) << "Expiry Date" << endl;
    cout << LINE << endl;

    for (const auto& prod : *products) {
        cout << left << setw(ID_WIDTH) << prod.id
             << setw(NAME_WIDTH) << prod.name
             << setw(PRICE_WIDTH) << fixed << setprecision(2) << prod.price
             << setw(QUANTITY_WIDTH) << prod.quantity
             << setw(EXPIRY_WIDTH) << prod.expiryDate << endl;
    }
}

/*--------------------------------------------------------------------------------------------*/

void showExpiryProducts(vector<Product>* products[], int numCategories) {
    cout << "\nPRODUCTS NEAR EXPIRY (within 30 days)" << endl;
    cout << LINE << endl;
    cout << left << setw(ID_WIDTH) << "ID"
         << setw(NAME_WIDTH) << "Name"
         << setw(PRICE_WIDTH) << "Price"
         << setw(QUANTITY_WIDTH) << "Quantity"
         << setw(EXPIRY_WIDTH) << "Expiry Date" << endl;
    cout << LINE << endl;

    // Get current date
    time_t t = time(0);
    tm* now = localtime(&t);
    int currentYear = now->tm_year + 1900;
    int currentMonth = now->tm_mon + 1;
    int currentDay = now->tm_mday;

    for (int i = 0; i < numCategories; i++) {
        for (const auto& prod : *products[i]) {
            if (prod.expiryDate != "-") {
                int year, month, day;
                char dash1, dash2;
                stringstream ss(prod.expiryDate);
                ss >> year >> dash1 >> month >> dash2 >> day;

                // Calculate difference in days
                tm expiryDate = {};
                expiryDate.tm_year = year - 1900;
                expiryDate.tm_mon = month - 1;
                expiryDate.tm_mday = day;
                time_t expiryTime = mktime(&expiryDate);
                double secondsDiff = difftime(expiryTime, t);
                int daysDiff = secondsDiff / (60 * 60 * 24);

                if (daysDiff >= 0 && daysDiff <= 30) {
                    cout << left << setw(ID_WIDTH) << prod.id
                         << setw(NAME_WIDTH) << prod.name
                         << setw(PRICE_WIDTH) << fixed << setprecision(2) << prod.price
                         << setw(QUANTITY_WIDTH) << prod.quantity
                         << setw(EXPIRY_WIDTH) << prod.expiryDate << endl;
                }
            }
        }
    }
}

/*--------------------------------------------------------------------------------------------*/

void showLowStockProducts(vector<Product>* products[], int numCategories) {
    const int LOW_STOCK_THRESHOLD = 30;
    cout << "\nPRODUCTS LOW IN STOCK (<= " << LOW_STOCK_THRESHOLD << ")" << endl;
    cout << LINE << endl;
    cout << left << setw(ID_WIDTH) << "ID"
         << setw(NAME_WIDTH) << "Name"
         << setw(PRICE_WIDTH) << "Price"
         << setw(QUANTITY_WIDTH) << "Quantity"
         << setw(EXPIRY_WIDTH) << "Expiry Date" << endl;
    cout << LINE << endl;

    for (int i = 0; i < numCategories; i++) {
        for (const auto& prod : *products[i]) {
            if (prod.quantity <= LOW_STOCK_THRESHOLD) {
                cout << left << setw(ID_WIDTH) << prod.id
                     << setw(NAME_WIDTH) << prod.name
                     << setw(PRICE_WIDTH) << fixed << setprecision(2) << prod.price
                     << setw(QUANTITY_WIDTH) << prod.quantity
                     << setw(EXPIRY_WIDTH) << prod.expiryDate << endl;
            }
        }
    }
}

/*--------------------------------------------------------------------------------------------*/

void searchByID(vector<Product>* products[], int numCategories) {
    string searchID;
    bool found = false;

    cout << "\nSEARCH PRODUCT BY ID" << endl;
    cout << "Enter Product ID to search: ";
    cin >> searchID;

    cout << LINE << endl;
    cout << left << setw(ID_WIDTH) << "ID"
         << setw(NAME_WIDTH) << "Name"
         << setw(PRICE_WIDTH) << "Price"
         << setw(QUANTITY_WIDTH) << "Quantity"
         << setw(EXPIRY_WIDTH) << "Expiry Date" << endl;
    cout << LINE << endl;

    for (int i = 0; i < numCategories; i++) {
        for (const auto& prod : *products[i]) {
            if (prod.id == searchID) {
                cout << left << setw(ID_WIDTH) << prod.id
                     << setw(NAME_WIDTH) << prod.name
                     << setw(PRICE_WIDTH) << fixed << setprecision(2) << prod.price
                     << setw(QUANTITY_WIDTH) << prod.quantity
                     << setw(EXPIRY_WIDTH) << prod.expiryDate << endl;
                found = true;
                break;
            }
        }
        if (found) break;
    }

    if (!found) {
        cout << "Product with ID " << searchID << " not found." << endl;
    }
}

/*--------------------------------------------------------------------------------------------*/

void searchByName(vector<Product>* products[], int numCategories) {
    string searchName;
    bool found = false;

    cout << "\nSEARCH PRODUCT BY NAME" << endl;
    cout << "Enter Product Name to search: ";
    cin.ignore();
    getline(cin, searchName);

    cout << LINE << endl;
    cout << left << setw(ID_WIDTH) << "ID"
         << setw(NAME_WIDTH) << "Name"
         << setw(PRICE_WIDTH) << "Price"
         << setw(QUANTITY_WIDTH) << "Quantity"
         << setw(EXPIRY_WIDTH) << "Expiry Date" << endl;
    cout << LINE << endl;

    for (int i = 0; i < numCategories; i++) {
        for (const auto& prod : *products[i]) {
            if (prod.name.find(searchName) != string::npos) {
                cout << left << setw(ID_WIDTH) << prod.id
                     << setw(NAME_WIDTH) << prod.name
                     << setw(PRICE_WIDTH) << fixed << setprecision(2) << prod.price
                     << setw(QUANTITY_WIDTH) << prod.quantity
                     << setw(EXPIRY_WIDTH) << prod.expiryDate << endl;
                found = true;
            }
        }
    }

    if (!found) {
        cout << "Product with name containing \"" << searchName << "\" not found." << endl;
    }
}

/*--------------------------------------------------------------------------------------------*/

void adminPrograms(User &currentUser) {
    string line;
    int choice;
    vector<Product> objects, foods, drinks;
    vector<Product> *products[3] = {&objects, &foods, &drinks};
    const int NUM_CATEGORIES = 3;

    // First load existing products from files
    loadProducts(products, NUM_CATEGORIES);


    // Display menu until logout
    do {
        system("cls");
        do {
            // Display menu
            cout << "CURRENT USER: " << currentUser.name << " (" << currentUser.role << ")" << endl;
            cout << LINE << endl;
            cout << "ADMIN PROGRAMS" << endl;
            cout << LINE << endl;
            cout << "1. Manage Product." << endl;
            cout << "2. Manage User." << endl;
            cout << "3. View Report." << endl;
            cout << "4. Check Log" << endl;
            cout << "5. Logout" << endl;
            cout << "\nEnter your choice: ";
            cin >> choice;
            cin.ignore();
            if (choice < 1 || choice > 5) {
                cout << "Invalid choice. Please try again." << endl;
                pause();
                system("cls");
            }
        } while (choice < 1 || choice > 5);
    
        switch (choice) {
            case 1:
                // Manage Product
                manageProducts(products, NUM_CATEGORIES);
                pause();
                break;
            case 2:
                // Manage User
                manageUsers();
                pause();
                break;
            case 3:
                // View Report
                viewReports(products, NUM_CATEGORIES);
                pause();
                break;
            case 4:
                // Check activity Log
                checkLog();
                pause();
                 break;
            case 5:
                cout << "Logging out..." << endl;
                logout(currentUser);
                log(currentUser.username, "Logged out", "User logged out", to_string(time(0)));
                break;
            default:
                cout << "ERROR" << endl;
        }
        
    }
    while (choice != 5);
}

/*--------------------------------------------------------------------------------------------*/

void manageProducts(vector<Product>* products[], int numCategories) {
    int choice;


    do {
        system("cls");

        cout << "MANAGE PRODUCTS" << endl;
        cout << LINE << endl;
        cout << "1. View Product" << endl;
        cout << "2. Add Product" << endl;
        cout << "3. Update Product" << endl;
        cout << "4. Delete Product" << endl;
        cout << "5. Return to previous menu" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                // View Product
                viewProducts(products, numCategories);
                break;
            case 2:
                // Add Product
                addProduct(products, numCategories);
                break;
            case 3:
                // Update Product
                updateProduct(products, numCategories);
                break;
            case 4:
                // Delete Product
                deleteProduct(products, numCategories);
                break;
            case 5:
                cout << "Returning to previous menu..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        pause();

    } while (choice != 5);

}

void addProduct(vector<Product>* products[], int numCategories) {
    string id, name, expiryDate;
    double price;
    int quantity;
    char categoryChoice;
    Category category;

    cout << "\nADD NEW PRODUCT" << endl;
    cout << "Enter Product ID (start with O for Object, F for Food, D for Drink): ";
    getline(cin, id);

    // Check if ID already exists
    for (int i = 0; i < numCategories; i++) {
        for (const auto& prod : *products[i]) {
            if (prod.id == id) {
                cout << "Product ID already exists. Cannot add product." << endl;
                return;
            }
        }
    }

    cout << "Enter Product Name: ";
    getline(cin, name);
    cout << "Enter Product Price: ";
    cin >> price;
    cout << "Enter Product Quantity: ";
    cin >> quantity;
    cin.ignore();
    if (id[0] == 'F' || id[0] == 'D') {
        cout << "Enter Expiry Date (YYYY-MM-DD): ";
        getline(cin, expiryDate);
    } else {
        expiryDate = "-";
    }

    // Determine category based on ID prefix
    switch (id[0]) {
        case 'O':
            category = OBJECT;
            break;
        case 'F':
            category = FOOD;
            break;
        case 'D':
            category = DRINK;
            break;
        default:
            cout << "Invalid Product ID prefix. Must start with O, F, or D." << endl;
            return;
    }

    cout << LINE << endl;
    cout << "STATUS" << endl;
    cout << LINE << endl;
            
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Price: " << price << endl;
    cout << "Stocks: " << quantity << endl;
    cout << "Expiry Date: " << expiryDate << endl;

    char confirmation;
    cout << "Are you sure you want to add this product? (Y/N): \n";
    cin >> confirmation;

    if (confirmation != 'Y' && confirmation != 'y') {
        cout << "add product cancelled." << endl;
        return;
    }

    // Add product to the appropriate category
    products[category]->push_back({id, name, price, quantity, expiryDate});
    cout << "Product added successfully." << endl;

    // Log the addition
    log("admin", "Add Product", "Added product ID: " + id, getCurrentTime());

    // Save to file
    ofstream outputFile(PRODUCT_FILE);
    if (outputFile.is_open()) {
        for (int i = 0; i < numCategories; i++) {
            for (const auto& prod : *products[i]) {
                outputFile << prod.id << "," << prod.name << "," << prod.price << ","
                           << prod.quantity << "," << prod.expiryDate << endl;
            }
        }   
        outputFile.close();
    }
}

void updateProduct(vector<Product>* products[], int numCategories) {
    string searchID;
    bool found = false;
    string newName, newExpiry;
    double newPrice;
    int newQty;
    cout << "\nUPDATE PRODUCT" << endl;
    cout << "Enter Product ID to update: ";
    getline(cin, searchID);

    for (int i = 0; i < numCategories; i++) {
        for (auto& prod : *products[i]) {
            if (prod.id == searchID) {
                found = true;
                cout << "Current details:" << endl;
                cout << "Name: " << prod.name << endl;
                cout << "Price: " << prod.price << endl;
                cout << "Quantity: " << prod.quantity << endl;
                cout << "Expiry Date: " << prod.expiryDate << endl;

                cout << "\nEnter new details (leave blank to keep current value):" << endl;
                string input;

                cout << "New Name: ";
                getline(cin, input);
                if (!input.empty()) newName = input;

                cout << "New Price: ";
                getline(cin, input);
                if (!input.empty()) newPrice = stod(input);

                cout << "New Quantity: ";
                getline(cin, input);
                if (!input.empty()) newQty = stoi(input);

                if (prod.id[0] == 'F' || prod.id[0] == 'D') {
                    cout << "New Expiry Date (YYYY-MM-DD): ";
                    getline(cin, input);
                    if (!input.empty()) newExpiry = input;
                }

                cout << LINE << endl;
                cout << "STATUS" << endl;
                cout << LINE << endl;
            
                cout << "ID: " << prod.id << endl;
                cout << "Name: " << prod.name; 
                if (!newName.empty()) cout << " -> " << newName; 
                cout << endl;
                cout << "Price: " << prod.price; 
                if (prod.price != newPrice) cout << " -> " << newPrice; 
                cout << endl;
                cout << "Quantity: " << prod.quantity;
                if (prod.quantity != newQty) cout << " -> " << newQty; 
                cout << endl;
                cout << "Expiry Date: " << prod.expiryDate;
                if (!newExpiry.empty()) cout << " -> " << newExpiry;
                cout << endl;

                char confirmation;
                cout << "Are you sure you want to update this product? (Y/N): \n";
                cin >> confirmation;

                if (confirmation != 'Y' && confirmation != 'y') {
                    cout << "Update product cancelled." << endl;
                    return;
                }

                prod.name = newName.empty() ? prod.name : newName;
                prod.price = (prod.price == newPrice) ? prod.price : newPrice;
                prod.quantity = (prod.quantity == newQty) ? prod.quantity : newQty;
                prod.expiryDate = newExpiry.empty() ? prod.expiryDate : newExpiry;

                cout << "Product updated successfully." << endl;

                // Log the update
                log("admin", "Update Product", "Updated product ID: " + searchID, getCurrentTime());

                // Save to file
                ofstream outputFile(PRODUCT_FILE);
                if (outputFile.is_open()) {
                    for (int j = 0; j < numCategories; j++) {
                        for (const auto& p : *products[j]) {
                            outputFile << p.id << "," << p.name << "," << p.price << ","
                                       << p.quantity << "," << p.expiryDate << endl;
                        }
                    }   
                    outputFile.close();
                }

                break;
            }
        }
        if (found) break;
    }

    if (!found) {
        cout << "Product with ID " << searchID << " not found." << endl;
    }
}

void deleteProduct(vector<Product>* products[], int numCategories) {
    string searchID;
    bool found = false;

    cout << "\nDELETE PRODUCT" << endl;
    cout << "Enter Product ID to delete: ";
    getline(cin, searchID);

    for (int i = 0; i < numCategories; i++) {
        for (auto it = products[i]->begin(); it != products[i]->end(); ++it) {
            if (it->id == searchID) {
                found = true;

                cout << "Current details:" << endl;
                cout << "Name: " << it->name << endl;
                cout << "Price: " << it->price << endl;
                cout << "Quantity: " << it->quantity << endl;
                cout << "Expiry Date: " << it->expiryDate << endl;
                char confirmation;
                cout << "Are you sure you want to delete this product? (Y/N): \n";
                cin >> confirmation;

                products[i]->erase(it);

                if (confirmation != 'Y' && confirmation != 'y') {
                    cout << "Deletion cancelled." << endl;
                    return;
                }

                products[i]->erase(it);

                cout << "Product deleted successfully." << endl;

                // Log the deletion
                log("admin", "Delete Product", "Deleted product ID: " + searchID, getCurrentTime());

                // Save to file
                ofstream outputFile(PRODUCT_FILE);
                if (outputFile.is_open()) {
                    for (int j = 0; j < numCategories; j++) {
                        for (const auto& p : *products[j]) {
                            outputFile << p.id << "," << p.name << "," << p.price << ","
                                       << p.quantity << "," << p.expiryDate << endl;
                        }
                    }   
                    outputFile.close();
                }

                break;
            }
        }
        if (found) break;
    }

    if (!found) {
        cout << "Product with ID " << searchID << " not found." << endl;
    }
}

/*--------------------------------------------------------------------------------------------*/

void manageUsers() {
    int choice;
    vector<User> users;
    system("cls");

    loadUsers(users);

    do {
        cout << "MANAGE USERS" << endl;
        cout << LINE << endl;
        cout << "1. View Users" << endl;
        cout << "2. Add User" << endl;
        cout << "3. Update User" << endl;
        cout << "4. Delete User" << endl;
        cout << "5. Return to previous menu" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                // View Users
                viewUsers(users);
                break;
            case 2:
                // Add User
                addUser(users);
                break;
            case 3:
                // Update User
                updateUser(users);
                break;
            case 4:
                // Delete User
                deleteUser(users);
                break;
            case 5:
                cout << "Returning to previous menu..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

        pause();
        system("cls");

    } while (choice != 5);
}

void loadUsers(vector<User> &users) {
    ifstream inputFile(USER_FILE);
    string line;
    users.clear();

    while (getline(inputFile, line)) {
        stringstream ss(line);
        User user;
        getline(ss, user.username, ',');
        getline(ss, user.password, ',');
        getline(ss, user.name, ',');
        getline(ss, user.role, ',');
        users.push_back(user);
    }
    inputFile.close();
}

void viewUsers(const vector<User> &users) {
    const int USERNAME_WIDTH = 15;
    const int NAME_WIDTH = 20;
    const int ROLE_WIDTH = 10;
    cout << "\nALL USERS" << endl;
    cout << LINE << endl;
    cout << left << setw(USERNAME_WIDTH) << "Username"
         << setw(NAME_WIDTH) << "Name"
         << setw(ROLE_WIDTH) << "Role" << endl;
    cout << LINE << endl;

    for (const auto& user : users) {
        cout << left << setw(USERNAME_WIDTH) << user.username
             << setw(NAME_WIDTH) << user.name
             << setw(ROLE_WIDTH) << user.role << endl;
    }
}

void addUser(vector<User> &users) {
    string username, password, name, role;

    cout << "\nADD NEW USER" << endl;
    cout << "Enter Username: ";
    getline(cin, username);

    // Check if username already exists
    for (const auto& user : users) {
        if (user.username == username) {
            cout << "Username already exists. Cannot add user." << endl;
            return;
        }
    }

    cout << "Enter Password: ";
    getline(cin, password);
    cout << "Enter Name: ";
    getline(cin, name);
    cout << "Enter Role (admin/staff): ";
    getline(cin, role);

    if (role != "admin" && role != "staff") {
        cout << "Invalid role. Must be 'admin' or 'staff'." << endl;
        return;
    }
    
    // Show demo 
    cout << LINE << endl;
    cout << "STATUS" << endl;
    cout << LINE << endl;
            
    cout << "Username: " << username << endl;
    cout << "Password: " << password << endl;
    cout << "Name: " << name << endl;
    cout << "Role: " << role << endl;

    char confirmation;
    cout << "Are you sure you want to add this user? (Y/N): \n";
    cin >> confirmation;

    if (confirmation != 'Y' && confirmation != 'y') {
        cout << "add user cancelled." << endl;
        return;
    }

    users.push_back({username, password, name, role});
    cout << "User added successfully." << endl;

    // Log the addition
    log("admin", "Add User", "Added user: " + username, getCurrentTime());

    // Save to file
    ofstream outputFile(USER_FILE);
    if (outputFile.is_open()) {
        for (const auto& user : users) {
            outputFile << user.username << "," << user.password << ","
                       << user.name << "," << user.role << endl;
        }
        outputFile.close();
    }
}

void updateUser(vector<User> &users) {
    string searchUsername;
    string newName, newPassword, newRole;
    bool found = false;

    cout << "\nUPDATE USER" << endl;
    cout << "Enter Username to update: ";
    getline(cin, searchUsername);

    for (auto& user : users) {
        if (user.username == searchUsername) {
            found = true;
            cout << "Current details:" << endl;
            cout << "Name: " << user.name << endl;
            cout << "Role: " << user.role << endl;

            cout << "\nEnter new details (leave blank to keep current value):" << endl;
            string input;

            cout << "New Password: ";
            getline(cin, input);
            if (!input.empty()) newPassword = input;
            input.clear();

            cout << "New Name: ";
            getline(cin, input);
            if (!input.empty()) newName = input;
            input.clear();

            cout << "New Role (admin/staff): ";
            getline(cin, input);
            if (!input.empty()) {
                if (input == "admin" || input == "staff") {
                    newRole = input;
                } else {
                    cout << "Invalid role. Must be 'admin' or 'staff'." << endl;
                    return;
                }
            }

            cout << LINE << endl;
            cout << "STATUS" << endl;
            cout << LINE << endl;
            
            cout << "Username: " << user.username << endl;
            cout << "Password: " << user.password; 
            if (!newPassword.empty()) cout << " -> " << newPassword; 
            cout << endl;
            cout << "Name: " << user.name; 
            if (!newName.empty()) cout << " -> " << newName; 
            cout << endl;
            cout << "Role: " << user.role;
            if (user.role != newRole) cout << " -> " << newRole; 
            cout << endl;

            char confirmation;
            cout << "Are you sure you want to update this user? (Y/N): \n";
            cin >> confirmation;

            if (confirmation != 'Y' && confirmation != 'y') {
                cout << "Update user cancelled." << endl;
                return;
            }

            user.name = newName.empty() ?  user.name : newName;
            user.password = newPassword.empty() ? user.password : newPassword;
            user.role = newRole.empty() ? user.role : newRole;

            cout << "User updated successfully." << endl;

            // Log the update
            log("admin", "Update User", "Updated user: " + searchUsername, getCurrentTime());

            // Save to file
            ofstream outputFile(USER_FILE);
            if (outputFile.is_open()) {
                for (const auto& u : users) {
                    outputFile << u.username << "," << u.password << ","
                               << u.name << "," << u.role << endl;
                }
                outputFile.close();
            }

            break;
        }
    }
    if (!found) {
        cout << "User with username " << searchUsername << " not found." << endl;
    }
}

void deleteUser(vector<User> &users) {
    string searchUsername;
    bool found = false;

    cout << "\nDELETE USER" << endl;
    cout << "Enter Username to delete: ";
    getline(cin, searchUsername);

    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->username == searchUsername) {
            found = true;

            // Show detail of delete user
            cout << LINE << endl;
            cout << "DETAIL" << endl;
            cout << LINE << endl;
            cout << "Username: " << it->username << endl;
            cout << "Name: " << it->name << endl;
            cout << "Role: " << it->role << endl;

            char confirmation;
            cout << "Are you sure to delete this user? (y/n).\n";
            cin >> confirmation;
            
            if (confirmation != 'Y' && confirmation != 'y') {
                cout << "Delete user cancelled." << endl;
                return;
            }

            users.erase(it);
            cout << "User deleted successfully." << endl;

            // Log the deletion
            log("admin", "Delete User", "Deleted user: " + searchUsername, getCurrentTime());

            // Save to file
            ofstream outputFile(USER_FILE);
            if (outputFile.is_open()) {
                for (const auto& u : users) {
                    outputFile << u.username << "," << u.password << ","
                               << u.name << "," << u.role << endl;
                }
                outputFile.close();
            }

            break;
        }
    }

    if (!found) {
        cout << "User with username " << searchUsername << " not found." << endl;
    }
}

/*--------------------------------------------------------------------------------------------*/

void viewReports(vector<Product>* products[], int numCategories) {
    cout << "\nVIEW REPORTS - Feature to be implemented." << endl;
}

/*--------------------------------------------------------------------------------------------*/

void checkLog() {
    const int TIMESTAMP_WIDTH = 25;
    const int USER_WIDTH = 15;
    const int ACTION_WIDTH = 20;
    const int DETAILS_WIDTH = 30;
    
    ifstream logFile(LOG_FILE);
    string line;

    cout << "\nACTIVITY LOG" << endl;
    cout << LINE << endl;
    cout << left << setw(TIMESTAMP_WIDTH) << "Timestamp"
         << setw(USER_WIDTH) << "User"
         << setw(ACTION_WIDTH) << "Action"
         << "Details" << endl;
    cout << LINE << endl;

    while (getline(logFile, line)) {
        stringstream ss(line);
        string timestamp, user, action, details;
        getline(ss, timestamp, ',');
        getline(ss, user, ',');
        getline(ss, action, ',');
        getline(ss, details, ',');

        cout << left << setw(TIMESTAMP_WIDTH) << timestamp
             << setw(USER_WIDTH) << user
             << setw(ACTION_WIDTH) << action
             << details << endl;
    }

    logFile.close();
}
