=

#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <iomanip>
using namespace std;



// Base user-defined exception class
class RentalException {
private:
    string message;
public:
    RentalException(string msg) : message(msg) {}
    string getMessage() const { return message; }
};

// [Syllabus: Unit-2 Inheritance] exception hierarchy: derived exceptions
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

// ======================================================================
// PART: InputHelper.h
// ======================================================================
// InputHelper.h
// [Syllabus: Unit-3 Templates -> class template and function template]

// CLASS TEMPLATE: a range that works for int, double, etc.
template <class T>
class Range {
private:
    T low;
    T high;
public:
    Range(T l, T h) : low(l), high(h) {}
    bool contains(T value) const { return value >= low && value <= high; }
};

// FUNCTION TEMPLATE: validated numeric input for any numeric type
template <class T>
T readNumber(string prompt, T low, T high) {
    Range<T> range(low, high);          // object of the class template
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(1000, '\n');
            if (range.contains(value)) return value;
        } else {
            if (cin.eof()) { cout << "\nInput closed. Exiting.\n"; exit(0); }
            cin.clear();                // clear error flags
            cin.ignore(1000, '\n');     // discard bad input
        }
        cout << "  Invalid input. Enter a value between " << low << " and " << high << ".\n";
    }
}

// [Syllabus: Unit-1 String Handling] read one word (no spaces)
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

// ======================================================================
// PART: Date.h
// ======================================================================
// Date.h
// [Syllabus: Unit-1 Classes/Objects; Unit-2 Operator Overloading, member classes]

class Date {
private:                                   // access specifier: private
    int day, month, year;
    static bool isLeap(int y);             // static member functions
    static int daysInMonth(int m, int y);
    int toDayNumber() const;               // days counted from year 1
public:
    Date();                                // default constructor
    Date(int d, int m, int y);             // parameterized constructor

    // inline functions (defined inside the class)
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }

    bool isValid() const;

    // OPERATOR OVERLOADING: date1 - date2 gives the number of days between them
    int operator-(const Date& other) const;

    // FRIEND FUNCTION + operator overloading: cout << date
    friend ostream& operator<<(ostream& out, const Date& d);

    // static member function: reads a valid date from the keyboard
    static Date input(string prompt);
};

// ======================================================================
// PART: Vehicle.h
// ======================================================================
// Vehicle.h
// [Syllabus: Unit-1 Classes, constructors, destructor, static members, function overloading]
// [Syllabus: Unit-2 Inheritance, virtual functions, pure virtual functions, dynamic binding]

// ABSTRACT BASE CLASS - contains pure virtual functions, so no object of Vehicle can be made
class Vehicle {
private:                                  // data hiding (encapsulation)
    int id;
    string brand;
    string model;
    double baseRate;                      // rent per day
    bool available;
    static int totalVehicles;             // static data member: shared by all objects

public:
    // Constructor overloading
    Vehicle();                                                      // default
    Vehicle(int i, string b, string m, double rate);                // parameterized
    Vehicle(int i, string b, string m, double rate, bool avail);    // parameterized (overloaded)
    Vehicle(const Vehicle& v);                                      // copy constructor
    virtual ~Vehicle();                   // virtual destructor -> correct clean-up via base pointer

    // inline member functions (defined inside the class)
    int getId() const { return id; }
    string getBrand() const { return brand; }
    string getModel() const { return model; }
    double getBaseRate() const { return baseRate; }
    bool isAvailable() const { return available; }
    void setAvailable(bool a) { available = a; }
    void setBaseRate(double r) { if (r > 0) baseRate = r; }

    // static member function
    static int getTotalVehicles();

    // PURE VIRTUAL FUNCTIONS: every derived class must override them (runtime polymorphism)
    virtual double calculateRent(int days) const = 0;
    virtual void displayDetails() const = 0;
    virtual string getType() const = 0;
    virtual void saveToFile(ofstream& out) const = 0;

    // FUNCTION OVERLOADING (compile-time polymorphism): same name, different parameters
    double getBill(int days) const;                     // normal bill
    double getBill(int plannedDays, int actualDays) const;  // bill with late fee

    // OPERATOR OVERLOADING: two vehicles are equal if their IDs match
    bool operator==(const Vehicle& other) const { return id == other.id; }

protected:
    void displayCommon() const;           // accessible to derived classes only
    void saveCommon(ofstream& out) const;
};

// Types of inheritance: HIERARCHICAL (one base, many derived classes), public mode
class Car : public Vehicle {
private:
    int seats;
    bool hasAC;
public:
    Car();
    Car(int i, string b, string m, double rate, int s, bool ac, bool avail);
    int getSeats() const { return seats; }
    bool getHasAC() const { return hasAC; }
    // FUNCTION OVERRIDING
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

// ======================================================================
// PART: Rental.h
// ======================================================================
// Rental.h
// [Syllabus: Unit-1 friend function, objects as function arguments, copy constructor, static members]
// [Syllabus: Unit-2 member classes (a Rental HAS-A Date), file streams in Unit-3]

class Rental {
private:
    int id;
    int vehicleId;
    string customer;
    int plannedDays;
    int actualDays;
    Date rentDate;                // member object (class inside a class - composition)
    Date returnDate;
    double total;
    bool active;
    static int nextId;            // static data member: auto-increment rental ID

public:
    Rental();
    Rental(int rid, int vid, string cust, int days, Date rd);
    Rental(const Rental& r);      // copy constructor

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

    // FRIEND FUNCTION: not a member, but may read private data.
    // The Rental object is passed BY VALUE (copy constructor is called).
    friend void printReceipt(Rental r);
};

// ======================================================================
// PART: RentalSystem.h
// ======================================================================
// RentalSystem.h
// [Syllabus: Unit-1 pointers with objects; Unit-3 dynamic memory, file streams, STL vector/map/queue]

class RentalSystem {
private:
    // STL vector of BASE-CLASS POINTERS: each element points to a Car, Bike or Truck
    // created with 'new' (dynamic memory allocation)
    vector<Vehicle*> vehicles;
    vector<Rental> rentals;                   // STL vector of objects
    map<int, queue<string> > waitingList;     // STL map + queue: vehicleId -> waiting customers

    // Copying is disabled (the class owns raw pointers) - declared private, never defined
    RentalSystem(const RentalSystem&);
    RentalSystem& operator=(const RentalSystem&);

    int findIndex(int id) const;
    Vehicle* findVehicle(int id) const;       // throws VehicleNotFoundException
    int nextVehicleId() const;
    bool isVehicleRented(int id) const;
    void loadVehicles();
    void loadRentals();
    void saveAll() const;
    void seedVehicles();

public:
    RentalSystem();
    ~RentalSystem();                          // frees all dynamically allocated vehicles

    void addVehicle();
    void removeVehicle();
    void viewAll() const;
    void viewAvailable() const;
    void checkAvailability() const;
    void rentVehicle();
    void returnVehicle();
    void viewRentals() const;
};

// ======================================================================
// PART: Date.cpp
// ======================================================================
// Date.cpp - member functions defined OUTSIDE the class using the scope resolution operator

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
    return day >= 1 && day <= daysInMonth(month, year);
}

// Converts the date into a running day number so two dates can be subtracted
int Date::toDayNumber() const {
    int y = year - 1;
    int total = y * 365 + y / 4 - y / 100 + y / 400;     // days in complete years
    int cumulative[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    total += cumulative[month - 1];
    if (month > 2 && isLeap(year)) total++;
    return total + day;
}

int Date::operator-(const Date& other) const {
    return toDayNumber() - other.toDayNumber();
}

ostream& operator<<(ostream& out, const Date& d) {
    out << right << setfill('0') << setw(2) << d.day << '/' << setw(2) << d.month << '/' << d.year
        << setfill(' ');
    return out;
}

Date Date::input(string prompt) {
    while (true) {
        cout << prompt << " (enter day, month, year)\n";
        int d = readNumber<int>("  Day   : ", 1, 31);
        int m = readNumber<int>("  Month : ", 1, 12);
        int y = readNumber<int>("  Year  : ", 2000, 2100);
        Date dt(d, m, y);
        if (dt.isValid()) return dt;
        cout << "  That date does not exist. Please try again.\n";
    }
}

// ======================================================================
// PART: Vehicle.cpp
// ======================================================================
// Vehicle.cpp - member functions defined outside the class

// Definition and initialisation of the static data member
int Vehicle::totalVehicles = 0;

// ---------- Vehicle (base) ----------
Vehicle::Vehicle() : id(0), brand("Unknown"), model("Unknown"), baseRate(0), available(true) {
    totalVehicles++;                      // object lifecycle: count increases on creation
}

Vehicle::Vehicle(int i, string b, string m, double rate)
    : id(i), brand(b), model(m), baseRate(rate), available(true) {
    totalVehicles++;
}

Vehicle::Vehicle(int i, string b, string m, double rate, bool avail)
    : id(i), brand(b), model(m), baseRate(rate), available(avail) {
    totalVehicles++;
}

Vehicle::Vehicle(const Vehicle& v)        // copy constructor
    : id(v.id), brand(v.brand), model(v.model), baseRate(v.baseRate), available(v.available) {
    totalVehicles++;
}

Vehicle::~Vehicle() {
    totalVehicles--;                      // object lifecycle: count decreases on destruction
}

int Vehicle::getTotalVehicles() { return totalVehicles; }

// Overloaded bill functions - both use the virtual calculateRent() (dynamic binding)
double Vehicle::getBill(int days) const {
    return calculateRent(days);
}

double Vehicle::getBill(int plannedDays, int actualDays) const {
    double bill = calculateRent(actualDays);
    if (actualDays > plannedDays)         // late fee: 50% of daily rate per extra day
        bill += 0.5 * baseRate * (actualDays - plannedDays);
    return bill;
}

// [Syllabus: Unit-1 Manipulators] setw, left, right, fixed, setprecision
void Vehicle::displayCommon() const {
    cout << "[ID " << right << setw(2) << id << "] "
         << left << setw(6) << getType()
         << setw(12) << brand << setw(14) << model
         << "Rs. " << fixed << setprecision(2) << baseRate << "/day  "
         << (available ? "Available" : "Rented") << endl;
}

void Vehicle::saveCommon(ofstream& out) const {
    out << getType() << ' ' << id << ' ' << brand << ' ' << model << ' '
        << fixed << setprecision(2) << baseRate << ' ' << (available ? 1 : 0) << ' ';
}

// ---------- Car ----------
Car::Car() : Vehicle(), seats(4), hasAC(false) {}

Car::Car(int i, string b, string m, double rate, int s, bool ac, bool avail)
    : Vehicle(i, b, m, rate, avail), seats(s), hasAC(ac) {}

// Car pricing: +10% if AC, 10% discount for 7+ days
double Car::calculateRent(int days) const {
    double amount = getBaseRate() * days;
    if (hasAC) amount = amount * 1.10;
    if (days >= 7) amount = amount * 0.90;
    return amount;
}

void Car::displayDetails() const {
    displayCommon();
    cout << "        Seats: " << seats << " | AC: " << (hasAC ? "Yes" : "No") << endl;
}

void Car::saveToFile(ofstream& out) const {
    saveCommon(out);
    out << seats << ' ' << (hasAC ? 1 : 0) << '\n';
}

// ---------- Bike ----------
Bike::Bike() : Vehicle(), engineCC(100) {}

Bike::Bike(int i, string b, string m, double rate, int cc, bool avail)
    : Vehicle(i, b, m, rate, avail), engineCC(cc) {}

// Bike pricing: 15% discount for 7+ days, plus flat Rs.50 helmet fee
double Bike::calculateRent(int days) const {
    double amount = getBaseRate() * days;
    if (days >= 7) amount = amount * 0.85;
    return amount + 50.0;
}

void Bike::displayDetails() const {
    displayCommon();
    cout << "        Engine: " << engineCC << " cc (Rs. 50 helmet fee included in bill)" << endl;
}

void Bike::saveToFile(ofstream& out) const {
    saveCommon(out);
    out << engineCC << '\n';
}

// ---------- Truck ----------
Truck::Truck() : Vehicle(), loadTons(1.0) {}

Truck::Truck(int i, string b, string m, double rate, double tons, bool avail)
    : Vehicle(i, b, m, rate, avail), loadTons(tons) {}

// Truck pricing: (base + Rs.100 per ton) per day, plus flat Rs.500 insurance
double Truck::calculateRent(int days) const {
    return (getBaseRate() + 100.0 * loadTons) * days + 500.0;
}

void Truck::displayDetails() const {
    displayCommon();
    cout << "        Load capacity: " << fixed << setprecision(1) << loadTons
         << " tons (Rs. 500 insurance included in bill)" << endl;
}

void Truck::saveToFile(ofstream& out) const {
    saveCommon(out);
    out << loadTons << '\n';
}

// ======================================================================
// PART: Rental.cpp
// ======================================================================
// Rental.cpp

int Rental::nextId = 1;

Rental::Rental()
    : id(0), vehicleId(0), customer(""), plannedDays(0), actualDays(0),
      total(0), active(false) {}

Rental::Rental(int rid, int vid, string cust, int days, Date rd)
    : id(rid), vehicleId(vid), customer(cust), plannedDays(days), actualDays(0),
      rentDate(rd), returnDate(rd), total(0), active(true) {}

Rental::Rental(const Rental& r)
    : id(r.id), vehicleId(r.vehicleId), customer(r.customer), plannedDays(r.plannedDays),
      actualDays(r.actualDays), rentDate(r.rentDate), returnDate(r.returnDate),
      total(r.total), active(r.active) {}

int Rental::generateId() { return nextId++; }
void Rental::setNextId(int n) { nextId = n; }

void Rental::closeRental(int actual, Date ret, double bill) {
    actualDays = actual;
    returnDate = ret;
    total = bill;
    active = false;
}

void Rental::display() const {
    cout << "Rental #" << id << " | Vehicle " << vehicleId << " | " << customer
         << " | " << rentDate << " | planned " << plannedDays << "d | "
         << (active ? "ACTIVE" : "RETURNED") << " | Rs. "
         << fixed << setprecision(2) << total << endl;
}

// [Syllabus: Unit-3 File I/O] write one record as a line of text
void Rental::saveToFile(ofstream& out) const {
    out << id << ' ' << vehicleId << ' ' << customer << ' ' << plannedDays << ' '
        << actualDays << ' '
        << rentDate.getDay() << ' ' << rentDate.getMonth() << ' ' << rentDate.getYear() << ' '
        << returnDate.getDay() << ' ' << returnDate.getMonth() << ' ' << returnDate.getYear() << ' '
        << fixed << setprecision(2) << total << ' ' << (active ? 1 : 0) << '\n';
}

// read one record; returns false at end of file
bool Rental::loadFromFile(ifstream& in) {
    int d1, m1, y1, d2, m2, y2, act;
    in >> id >> vehicleId >> customer >> plannedDays >> actualDays
       >> d1 >> m1 >> y1 >> d2 >> m2 >> y2 >> total >> act;
    if (!in) return false;
    rentDate = Date(d1, m1, y1);
    returnDate = Date(d2, m2, y2);
    active = (act == 1);
    return true;
}

void printReceipt(Rental r) {
    cout << "\n------------- RETURN RECEIPT -------------\n"
         << "Rental ID    : " << r.id << "\n"
         << "Customer     : " << r.customer << "\n"
         << "Vehicle ID   : " << r.vehicleId << "\n"
         << "Rented on    : " << r.rentDate << "\n"
         << "Returned on  : " << r.returnDate << "\n"
         << "Planned days : " << r.plannedDays << "\n"
         << "Actual days  : " << r.actualDays << "\n"
         << "TOTAL BILL   : Rs. " << fixed << setprecision(2) << r.total << "\n"
         << "------------------------------------------\n";
}

// ======================================================================
// PART: RentalSystem.cpp
// ======================================================================
// RentalSystem.cpp

// ---------- Constructor / Destructor ----------
RentalSystem::RentalSystem() {
    loadVehicles();
    loadRentals();
    if (vehicles.empty()) seedVehicles();
}

RentalSystem::~RentalSystem() {
    saveAll();
    // [Syllabus: Unit-3 dynamic memory deallocation] every 'new' needs a matching 'delete'
    for (size_t i = 0; i < vehicles.size(); i++)
        delete vehicles[i];       // virtual destructor makes this safe for Car/Bike/Truck
    vehicles.clear();
}

// ---------- File handling ----------
void RentalSystem::loadVehicles() {
    ifstream in("vehicles.txt");              // [Syllabus: Unit-3 file streams]
    if (!in) return;                          // first run: file does not exist yet
    string type;
    while (in >> type) {
        int id, avail;
        string brand, model;
        double rate;
        in >> id >> brand >> model >> rate >> avail;
        if (type == "Car") {
            int seats, ac;
            in >> seats >> ac;
            vehicles.push_back(new Car(id, brand, model, rate, seats, ac == 1, avail == 1));
        } else if (type == "Bike") {
            int cc;
            in >> cc;
            vehicles.push_back(new Bike(id, brand, model, rate, cc, avail == 1));
        } else if (type == "Truck") {
            double tons;
            in >> tons;
            vehicles.push_back(new Truck(id, brand, model, rate, tons, avail == 1));
        }
    }
    in.close();
}

void RentalSystem::loadRentals() {
    ifstream in("rentals.txt");
    if (!in) return;
    Rental r;
    int maxId = 0;
    while (r.loadFromFile(in)) {
        rentals.push_back(r);
        if (r.getId() > maxId) maxId = r.getId();
    }
    Rental::setNextId(maxId + 1);
    in.close();
}

void RentalSystem::saveAll() const {
    ofstream vout("vehicles.txt");
    for (size_t i = 0; i < vehicles.size(); i++)
        vehicles[i]->saveToFile(vout);        // dynamic binding: correct derived version runs
    vout.close();

    ofstream rout("rentals.txt");
    for (size_t i = 0; i < rentals.size(); i++)
        rentals[i].saveToFile(rout);
    rout.close();
}

void RentalSystem::seedVehicles() {
    vehicles.push_back(new Car(1, "Toyota", "Innova", 3000, 7, true, true));
    vehicles.push_back(new Car(2, "Maruti", "Swift", 1800, 5, false, true));
    vehicles.push_back(new Bike(3, "Royal", "Enfield350", 700, 350, true));
    vehicles.push_back(new Bike(4, "Honda", "Activa", 400, 110, true));
    vehicles.push_back(new Truck(5, "Tata", "Ace", 2500, 1.5, true));
    saveAll();
}

// ---------- Helpers ----------
int RentalSystem::findIndex(int id) const {
    for (size_t i = 0; i < vehicles.size(); i++)
        if (vehicles[i]->getId() == id) return (int)i;
    return -1;
}

Vehicle* RentalSystem::findVehicle(int id) const {
    int idx = findIndex(id);
    if (idx < 0) throw VehicleNotFoundException();   // [Unit-2] throwing an exception
    return vehicles[idx];
}

int RentalSystem::nextVehicleId() const {
    int maxId = 0;
    for (size_t i = 0; i < vehicles.size(); i++)
        if (vehicles[i]->getId() > maxId) maxId = vehicles[i]->getId();
    return maxId + 1;
}

bool RentalSystem::isVehicleRented(int id) const {
    for (size_t i = 0; i < rentals.size(); i++)
        if (rentals[i].isActive() && rentals[i].getVehicleId() == id) return true;
    return false;
}

// ---------- Inventory management ----------
void RentalSystem::addVehicle() {
    cout << "\nVehicle type: 1. Car   2. Bike   3. Truck\n";
    int type = readNumber<int>("Type: ", 1, 3);
    string brand = readWord("Brand (one word): ");
    string model = readWord("Model (one word): ");
    double rate = readNumber<double>("Base rate per day (Rs.): ", 1.0, 1000000.0);
    int id = nextVehicleId();

    Vehicle* v = NULL;                        // base-class pointer
    if (type == 1) {
        int seats = readNumber<int>("Seats (2-12): ", 2, 12);
        bool ac = readYesNo("Air conditioned?");
        v = new Car(id, brand, model, rate, seats, ac, true);     // dynamic allocation
    } else if (type == 2) {
        int cc = readNumber<int>("Engine capacity in cc (50-2000): ", 50, 2000);
        v = new Bike(id, brand, model, rate, cc, true);
    } else {
        double tons = readNumber<double>("Load capacity in tons (0.5-40): ", 0.5, 40.0);
        v = new Truck(id, brand, model, rate, tons, true);
    }
    vehicles.push_back(v);                    // base pointer holds derived object (upcasting)
    saveAll();
    cout << "Vehicle added with ID " << id << ".\n";
}

void RentalSystem::removeVehicle() {
    int id = readNumber<int>("Vehicle ID to remove: ", 1, 100000);
    int idx = findIndex(id);
    if (idx < 0) throw VehicleNotFoundException();
    if (isVehicleRented(id)) throw RentalException("A rented vehicle cannot be removed.");
    if (!readYesNo("Are you sure?")) return;
    delete vehicles[idx];                                   // free the object
    vehicles.erase(vehicles.begin() + idx);                 // remove the pointer from the vector
    saveAll();
    cout << "Vehicle removed.\n";
}

void RentalSystem::viewAll() const {
    cout << "\n=========== FLEET INVENTORY ===========\n";
    if (vehicles.empty()) cout << "No vehicles in inventory.\n";
    for (size_t i = 0; i < vehicles.size(); i++)
        vehicles[i]->displayDetails();                      // runtime polymorphism
    cout << "Total vehicles in memory: " << Vehicle::getTotalVehicles() << endl;
}

void RentalSystem::viewAvailable() const {
    cout << "\n=========== AVAILABLE VEHICLES ===========\n";
    bool any = false;
    for (size_t i = 0; i < vehicles.size(); i++) {
        if (vehicles[i]->isAvailable()) {
            vehicles[i]->displayDetails();
            any = true;
        }
    }
    if (!any) cout << "No vehicles are available right now.\n";
}

void RentalSystem::checkAvailability() const {
    int id = readNumber<int>("Vehicle ID: ", 1, 100000);
    Vehicle* v = findVehicle(id);                           // pointer to object
    v->displayDetails();
    cout << (v->isAvailable() ? ">> This vehicle is AVAILABLE for rent.\n"
                              : ">> This vehicle is currently NOT available.\n");
}

// ---------- Rental workflow ----------
void RentalSystem::rentVehicle() {
    viewAll();
    int id = readNumber<int>("Enter Vehicle ID to rent (0 to cancel): ", 0, 100000);
    if (id == 0) return;
    Vehicle* v = findVehicle(id);
    string customer = readWord("Customer name (one word): ");

    // [Unit-2 Exception handling] try / throw / catch with a user-defined exception
    try {
        if (!v->isAvailable()) throw VehicleUnavailableException(id);
    } catch (VehicleUnavailableException& e) {
        cout << "Sorry, " << e.getMessage() << endl;
        if (readYesNo("Join the waiting list for this vehicle?")) {
            waitingList[id].push(customer);                 // STL map of queues
            cout << "Added. Customers waiting for this vehicle: "
                 << waitingList[id].size() << endl;
        }
        return;
    }

    int days = readNumber<int>("Number of rental days (1-90): ", 1, 90);
    Date today = Date::input("Rental start date");
    cout << "Estimated bill: Rs. " << fixed << setprecision(2) << v->getBill(days) << endl;
    if (!readYesNo("Confirm rental?")) { cout << "Cancelled.\n"; return; }

    Rental r(Rental::generateId(), id, customer, days, today);
    r.setTotal(v->getBill(days));
    rentals.push_back(r);
    v->setAvailable(false);
    saveAll();
    cout << "\nRental confirmed! Your Rental ID is " << r.getId() << ".\n";
}

void RentalSystem::returnVehicle() {
    bool anyActive = false;
    for (size_t i = 0; i < rentals.size(); i++)
        if (rentals[i].isActive()) { rentals[i].display(); anyActive = true; }
    if (!anyActive) { cout << "There are no active rentals.\n"; return; }

    int rid = readNumber<int>("Rental ID to return (0 to cancel): ", 0, 100000);
    if (rid == 0) return;

    int idx = -1;
    for (size_t i = 0; i < rentals.size(); i++)
        if (rentals[i].getId() == rid && rentals[i].isActive()) idx = (int)i;
    if (idx < 0) throw RentalException("No active rental found with that ID.");

    Date returnDate = Date::input("Return date");
    int actual = returnDate - rentals[idx].getRentDate();   // overloaded operator-
    if (actual < 0) throw RentalException("Return date cannot be before the rental date.");
    if (actual < 1) actual = 1;                             // minimum one day charged

    Vehicle* v = findVehicle(rentals[idx].getVehicleId());
    double bill = v->getBill(rentals[idx].getPlannedDays(), actual);   // overloaded getBill
    rentals[idx].closeRental(actual, returnDate, bill);
    v->setAvailable(true);
    saveAll();

    cout << "Vehicle: " << v->getBrand() << " " << v->getModel() << " (" << v->getType() << ")\n";
    printReceipt(rentals[idx]);                             // friend function

    // STL map lookup + queue front/pop: notify the next waiting customer
    map<int, queue<string> >::iterator it = waitingList.find(v->getId());
    if (it != waitingList.end() && !it->second.empty()) {
        cout << "Notify next customer in waiting list: " << it->second.front() << endl;
        it->second.pop();
    }
}

void RentalSystem::viewRentals() const {
    cout << "\n=========== RENTAL HISTORY ===========\n";
    if (rentals.empty()) cout << "No rentals recorded.\n";
    for (size_t i = 0; i < rentals.size(); i++)
        rentals[i].display();
}

// ======================================================================
// PART: main.cpp
// ======================================================================
// main.cpp - Entry point
// [Syllabus: Unit-1 Structure of a C++ program, control statements (do-while, switch)]

int main() {
    RentalSystem rentalSystem;           // object creation (constructor loads files)
    int choice;

    cout << "=====================================\n"
         << "     VEHICLE RENTAL SYSTEM (C++ OOP)\n"
         << "  By Tlhoriso Molise (UID 25bca20025)\n"
         << "=====================================\n";

    do {
        cout << "\n----------- MAIN MENU -----------\n"
             << "1. Add vehicle\n"
             << "2. View all vehicles\n"
             << "3. View available vehicles\n"
             << "4. Check availability of a vehicle\n"
             << "5. Remove vehicle\n"
             << "6. Rent a vehicle\n"
             << "7. Return a vehicle\n"
             << "8. Rental history\n"
             << "9. Exit\n";
        choice = readNumber<int>("Enter choice: ", 1, 9);

        try {
            switch (choice) {
                case 1: rentalSystem.addVehicle(); break;
                case 2: rentalSystem.viewAll(); break;
                case 3: rentalSystem.viewAvailable(); break;
                case 4: rentalSystem.checkAvailability(); break;
                case 5: rentalSystem.removeVehicle(); break;
                case 6: rentalSystem.rentVehicle(); break;
                case 7: rentalSystem.returnVehicle(); break;
                case 8: rentalSystem.viewRentals(); break;
                case 9: cout << "Data saved. Goodbye!\n"; break;
            }
        } catch (RentalException& e) {   // also catches the derived exception classes
            cout << "Error: " << e.getMessage() << endl;
        }
    } while (choice != 9);

    return 0;
}
