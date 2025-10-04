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
            log(currentUser.username, "Logged in", "Successfully Login", to_string(time(0)));
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
                log(currentUser.username, "Logged out", "User logged out", to_string(time(0)));
                break;
            default:
                cout << "ERROR" << endl;
        }

        pause();
    } while (choice != 5);
}

/*--------------------------------------------------------------------------------------------*/

void sellProducts(vector<Product>* products[], int numCategories) {
    // To be implemented
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

    // Add product to the appropriate category
    products[category]->push_back({id, name, price, quantity, expiryDate});
    cout << "Product added successfully." << endl;

    // Log the addition
    log("admin", "Add Product", "Added product ID: " + id, to_string(time(0)));

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
                if (!input.empty()) prod.name = input;

                cout << "New Price: ";
                getline(cin, input);
                if (!input.empty()) prod.price = stod(input);

                cout << "New Quantity: ";
                getline(cin, input);
                if (!input.empty()) prod.quantity = stoi(input);

                if (prod.id[0] == 'F' || prod.id[0] == 'D') {
                    cout << "New Expiry Date (YYYY-MM-DD): ";
                    getline(cin, input);
                    if (!input.empty()) prod.expiryDate = input;
                }

                cout << "Product updated successfully." << endl;

                // Log the update
                log("admin", "Update Product", "Updated product ID: " + searchID, to_string(time(0)));

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
                products[i]->erase(it);
                cout << "Product deleted successfully." << endl;

                // Log the deletion
                log("admin", "Delete Product", "Deleted product ID: " + searchID, to_string(time(0)));

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

    users.push_back({username, password, name, role});
    cout << "User added successfully." << endl;

    // Log the addition
    log("admin", "Add User", "Added user: " + username, to_string(time(0)));

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
            if (!input.empty()) user.password = input;

            cout << "New Name: ";
            getline(cin, input);
            if (!input.empty()) user.name = input;

            cout << "New Role (admin/staff): ";
            getline(cin, input);
            if (!input.empty()) {
                if (input == "admin" || input == "staff") {
                    user.role = input;
                } else {
                    cout << "Invalid role. Must be 'admin' or 'staff'." << endl;
                    return;
                }
            }

            cout << "User updated successfully." << endl;

            // Log the update
            log("admin", "Update User", "Updated user: " + searchUsername, to_string(time(0)));

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
            users.erase(it);
            cout << "User deleted successfully." << endl;

            // Log the deletion
            log("admin", "Delete User", "Deleted user: " + searchUsername, to_string(time(0)));

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
    const int TIMESTAMP_WIDTH = 20;
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
