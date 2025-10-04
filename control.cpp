#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

/*--------------------------------------------------------------------------------------------*/

typedef struct {
    string username;
    string password;
    string name;
    string role;
} User;

/*--------------------------------------------------------------------------------------------*/

void initialize();
void pause();
int login(User &currentUser);
void staffPrograms(User &currentUser);
void adminPrograms(User &currentUser);
bool checkFiles(string fileName);
void createUserFile();
void createObjectsFile();
void createFoodsFile();
void createDrinksFile();

/*--------------------------------------------------------------------------------------------*/

const string OBJECT_FILE = "object.dat";
const string FOOD_FILE = "foods.dat";
const string DRINK_FILE = "drinks.dat";
const string USER_FILE = "user.dat";

/*--------------------------------------------------------------------------------------------*/

int main() {
    User currentUser;

    // Initialize program.
    initialize();

    cout << "PRODUCT MANAGING PROGRAMS" << endl;
    cout << "\nPLEASE LOGIN TO CONTINUE." << endl;
    pause();

    login(currentUser);

    if (currentUser.role == "staff") {
       // staffPrograms(currentUser);
    }
    else if (currentUser.role == "admin") {
        //adminPrograms(currentUser);
    } 


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
    if (! checkFiles(OBJECT_FILE)) createObjectsFile();
    if (! checkFiles(FOOD_FILE)) createFoodsFile();
    if (! checkFiles(DRINK_FILE)) createDrinksFile();


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
    outputFile << "admin1,1234,Administrator,admin" << endl;
    outputFile.close();
    return;
}

void createObjectsFile() {
    ofstream o(OBJECT_FILE);
    if (o.is_open()) {
        o << "O1,Notebook,35.50,20,2026-05-10";
        o << "O2,Pencil,5.00,100,2028-12-31";
        o << "O3,Pen,12.75,50,2027-09-15";
        o << "O4,Eraser,8.25,40,2029-01-01";
        o << "O5,Ruler,15.00,30,2028-07-22";
        o.close();
    }
}

void createFoodsFile() {
    ofstream f(FOOD_FILE);
    if (f.is_open()) {
        f << "F1,Burger,120.00,30,2025-12-31\n";
        f << "F2,Pizza,200.00,20,2025-12-31\n";
        f << "F3,Fried Rice,80.00,50,2025-12-31\n";
        f << "F4,Salad,70.00,40,2025-12-31\n";
        f << "F5,Sandwich,90.00,25,2025-12-31\n";
        f << "F6,Spaghetti,110.00,30,2025-12-31\n";
        f.close();
    }
}

void createDrinksFile() {
    ofstream d(DRINK_FILE);
    if (d.is_open()) {
        d << "D1,Water,10.00,200,2025-12-31\n";
        d << "D2,Juice,25.00,120,2025-12-31\n";
        d << "D3,Milk,20.00,100,2025-12-31\n";
        d << "D4,Soda,22.00,150,2025-12-31\n";
        d << "D5,Tea,15.00,180,2025-12-31\n";
        d << "D6,Coffee,30.00,80,2025-12-31\n";
        d.close();
    }
}

/*--------------------------------------------------------------------------------------------*/

void pause() {
    char wait;
    cin.get(wait);
    cin.get(wait);
}

/*--------------------------------------------------------------------------------------------*/

int login(User &currentUser) {


}
