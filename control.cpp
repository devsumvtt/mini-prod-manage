#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;


typedef struct 
{
    string ID;
    string name;
    string category;
    float price;
    int amount;
    string expire_date;

} Product;


int init(vector<Product> &objects, vector<Product> &foods, vector<Product> &drinks);

int main() {
    vector<Product> objects, foods, drinks;

    if (init(objects, foods, drinks) != 0) {
        cout << "ERROR INITIALIZING PRODUCT DATA" << endl;
        return 1;
    }


    
    return 0;
}

int init(vector<Product> &objects, vector<Product> &foods, vector<Product> &drinks)
{
    ifstream InObjectFile, InFoodFile, InDrinkFile;

    string line;

    InObjectFile.open("data\\objects.dat");

    if (InObjectFile.is_open()) {

        while (getline(InObjectFile, line)) {

            stringstream ss(line);
            string subdata;
            vector<string> data;

            while (getline(ss, subdata, ',')) {
                data.push_back(subdata);
            }

            Product p;
            p.ID = data[0];
            p.name = data[1];
            p.category = "Object";
            p.price = stof(data[2]);
            p.amount = stoi(data[3]);
            p.expire_date = data[4];

            objects.push_back(p);
        }
    }
    else {
        cout << "ERROR OPENING OBJECTS FILE" << endl;
        return 1;
    }

    InObjectFile.close();

    InFoodFile.open("data\\foods.dat");

    if (InFoodFile.is_open()) {

        while (getline(InFoodFile, line)) {

            stringstream ss(line);
            string subdata;
            vector<string> data;

            while (getline(ss, subdata, ',')) {
                data.push_back(subdata);
            }

            Product p;
            p.ID = data[0];
            p.name = data[1];
            p.category = "Food";
            p.price = stof(data[2]);
            p.amount = stoi(data[3]);
            p.expire_date = data[4];

            objects.push_back(p);
        }
    }
    else {
        cout << "ERROR OPENING FOODS FILE" << endl;
        return 1;
    }

    InFoodFile.close();

    InDrinkFile.open("data\\drinks.dat");

    if (InDrinkFile.is_open()) {

        while (getline(InDrinkFile, line)) {

            stringstream ss(line);
            string subdata;
            vector<string> data;

            while (getline(ss, subdata, ',')) {
                data.push_back(subdata);
            }

            Product p;
            p.ID = data[0];
            p.name = data[1];
            p.category = "Drink";
            p.price = stof(data[2]);
            p.amount = stoi(data[3]);
            p.expire_date = data[4];

            objects.push_back(p);
        }
    }
    else {
        cout << "ERROR OPENING DRINKS FILE" << endl;
        return 1;
    }

    InDrinkFile.close();

    return 0;
}
