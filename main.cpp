#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <iomanip>
using namespace std;



class RentalException {
private:
    string message;
public:
    RentalException(string msg) : message(msg) {}
    string getMessage() const { return message; }
};

class VehicleNotFoundException : public RentalException {
public:
    VehicleNotFoundException() : RentalException("No vehicle exists with that ID.") {}
};

class VehicleUnavailableException : public RentalException {
private:
    int vehicleId;
public:
    VehicleUnavailableException(int id)
        : RentalException("this vehicle is currently rented out."), vehicleId(id) {}
    int getVehicleId() const { return vehicleId; }
};




template <class T>
class Range {
private:
    T low;
    T high;
public:
    Range(T l, T h) : low(l), high(h) {}
    bool contains(T value) const { return value >= low && value <= high; }
};

template <class T>
T readNumber(string prompt, T low, T high) {
    Range<T> range(low, high);          
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(1000, '\n');
            if (range.contains(value)) return value;
        } else {
            if (cin.eof()) { cout << "\nInput closed. Exiting.\n"; exit(0); }
            cin.clear();                
            cin.ignore(1000, '\n');     
        }
        cout << "  Invalid input. Enter a value between " << low << " and " << high << ".\n";
    }
}

inline string readWord(string prompt) {
    string s;
    while (true) {
        cout << prompt;
        if (cin >> s) { cin.ignore(1000, '\n'); return s; }
        if (cin.eof()) { cout << "\nInput closed. Exiting.\n"; exit(0); }
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

inline bool readYesNo(string prompt) {
    while (true) {
        string s = readWord(prompt + " (y/n): ");
        if (s == "y" || s == "Y") return true;
        if (s == "n" || s == "N") return false;
        cout << "  Please type y or n.\n";
    }
}




class Date {
private:                                   
    int day, month, year;
    static bool isLeap(int y);             
    static int daysInMonth(int m, int y);
    int toDayNumber() const;               
public:
    Date();                                
    Date(int d, int m, int y);             

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }

    bool isValid() const;

    int operator-(const Date& other) const;

    friend ostream& operator<<(ostream& out, const Date& d);

    static Date input(string prompt);
};




class Vehicle {
private:                                  
    int id;
    string brand;
    string model;
    double baseRate;                      
    bool available;
    static int totalVehicles;             

public:
    Vehicle();                                                      
    Vehicle(int i, string b, string m, double rate);                
    Vehicle(int i, string b, string m, double rate, bool avail);    
    Vehicle(const Vehicle& v);                                      
    virtual ~Vehicle();                   

    int getId() const { return id; }
    string getBrand() const { return brand; }
    string getModel() const { return model; }
    double getBaseRate() const { return baseRate; }
    bool isAvailable() const { return available; }
    void setAvailable(bool a) { available = a; }
    void setBaseRate(double r) { if (r > 0) baseRate = r; }

    static int getTotalVehicles();

    virtual double calculateRent(int days) const = 0;
    virtual void displayDetails() const = 0;
    virtual string getType() const = 0;
    virtual void saveToFile(ofstream& out) const = 0;

    double getBill(int days) const;                     
    double getBill(int plannedDays, int actualDays) const;  

    bool operator==(const Vehicle& other) const { return id == other.id; }

protected:
    void displayCommon() const;           
    void saveCommon(ofstream& out) const;
};

class Car : public Vehicle {
private:
    int seats;
    bool hasAC;
public:
    Car();
    Car(int i, string b, string m, double rate, int s, bool ac, bool avail);
    int getSeats() const { return seats; }
    bool getHasAC() const { return hasAC; }
    double calculateRent(int days) const;
    void displayDetails() const;
    string getType() const { return "Car"; }
    void saveToFile(ofstream& out) const;
};

class Bike : public Vehicle {
private:
    int engineCC;
public:
    Bike();
    Bike(int i, string b, string m, double rate, int cc, bool avail);
    int getEngineCC() const { return engineCC; }
    double calculateRent(int days) const;
    void displayDetails() const;
    string getType() const { return "Bike"; }
    void saveToFile(ofstream& out) const;
};

class Truck : public Vehicle {
private:
    double loadTons;
public:
    Truck();
    Truck(int i, string b, string m, double rate, double tons, bool avail);
    double getLoadTons() const { return loadTons; }
    double calculateRent(int days) const;
    void displayDetails() const;
    string getType() const { return "Truck"; }
    void saveToFile(ofstream& out) const;
};




class Rental {
private:
    int id;
    int vehicleId;
    string customer;
    int plannedDays;
    int actualDays;
    Date rentDate;                
    Date returnDate;
    double total;
    bool active;
    static int nextId;            

public:
    Rental();
    Rental(int rid, int vid, string cust, int days, Date rd);
    Rental(const Rental& r);      

    int getId() const { return id; }
    int getVehicleId() const { return vehicleId; }
    string getCustomer() const { return customer; }
    int getPlannedDays() const { return plannedDays; }
    Date getRentDate() const { return rentDate; }
    bool isActive() const { return active; }

    void setTotal(double t) { total = t; }
    void closeRental(int actual, Date ret, double bill);

    void display() const;
    void saveToFile(ofstream& out) const;
    bool loadFromFile(ifstream& in);

    static int generateId();
    static void setNextId(int n);

    friend void printReceipt(Rental r);
};




class RentalSystem {
private:
    vector<Vehicle*> vehicles;
    vector<Rental> rentals;                   
    map<int, queue<string> > waitingList;     

    RentalSystem(const RentalSystem&);
    RentalSystem& operator=(const RentalSystem&);

    int findIndex(int id) const;
    Vehicle* findVehicle(int id) const;       
    int nextVehicleId() const;
    bool isVehicleRented(int id) const;
    void loadVehicles();
    void loadRentals();
    void saveAll() const;
    void seedVehicles();

public:
    RentalSystem();
    ~RentalSystem();                          

    void addVehicle();
    void removeVehicle();
    void viewAll() const;
    void viewAvailable() const;
    void checkAvailability() const;
    void rentVehicle();
    void returnVehicle();
    void viewRentals() const;
};




Date::Date() : day(1), month(1), year(2025) {}
Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

bool Date::isLeap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Date::daysInMonth(int m, int y) {
    int dm[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && isLeap(y)) return 29;
    return dm[m - 1];
}

bool Date::isValid() const {
    if (year < 2000 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    return
