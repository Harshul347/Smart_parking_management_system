
#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

const int TOTAL_SLOTS = 10;

class Vehicle {
public:
    string number;
    string owner;
    string type;
    int slot;
    int entryTime;
    Vehicle* next;

    Vehicle(string num, string name, string t,
            int s, int time) {
        number = num;
        owner = name;
        type = t;
        slot = s;
        entryTime = time;
        next = nullptr;
    }
};

class SmartParking {
private:
    Vehicle* head;
    bool occupied[TOTAL_SLOTS + 1];
    double totalRevenue;

    Vehicle* findVehicle(string number) {
        Vehicle* temp = head;

        while (temp != nullptr) {
            if (temp->number == number)
                return temp;

            temp = temp->next;
        }

        return nullptr;
    }

    int findFreeSlot() {
        for (int i = 1; i <= TOTAL_SLOTS; i++) {
            if (!occupied[i])
                return i;
        }

        return -1;
    }

    double getRate(string type) {
        if (type == "Bike")
            return 10.0;
        if (type == "Car")
            return 20.0;
        return 30.0; // Bus
    }

public:
    SmartParking() {
        head = nullptr;
        totalRevenue = 0;

        for (int i = 0; i <= TOTAL_SLOTS; i++)
            occupied[i] = false;
    }

    // 1. Park a vehicle
    void parkVehicle() {
        string number, owner, type;
        int entryTime, slot;

        cout << "\nEnter Vehicle Number: ";
        getline(cin >> ws, number);

        if (findVehicle(number) != nullptr) {
            cout << "This vehicle is already parked!\n";
            return;
        }

        cout << "Enter Owner Name: ";
        getline(cin >> ws, owner);

        cout << "\nVehicle Type:\n";
        cout << "1. Bike\n2. Car\n3. Bus\n";
        cout << "Enter choice: ";

        int choice;
        if (!(cin >> choice) || choice < 1 || choice > 3) {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );
            cout << "Invalid vehicle type!\n";
            return;
        }

        if (choice == 1)
            type = "Bike";
        else if (choice == 2)
            type = "Car";
        else
            type = "Bus";

        slot = findFreeSlot();

        if (slot == -1) {
            cout << "Parking is full!\n";
            return;
        }

        cout << "Enter Entry Time (hour, 0-23): ";
        if (!(cin >> entryTime) ||
            entryTime < 0 || entryTime > 23) {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );
            cout << "Invalid entry time!\n";
            return;
        }

        Vehicle* newVehicle = new Vehicle(
            number, owner, type, slot, entryTime
        );

        newVehicle->next = head;
        head = newVehicle;

        occupied[slot] = true;

        cout << "\nVehicle parked successfully!\n";
        cout << "Vehicle Number: " << number << '\n';
        cout << "Parking Slot  : " << slot << '\n';
        cout << "Vehicle Type  : " << type << '\n';
    }

    // 2. Display parking slots
    void displaySlots() {
        cout << "\n========== PARKING SLOT STATUS ==========\n";

        for (int i = 1; i <= TOTAL_SLOTS; i++) {
            cout << "Slot " << setw(2) << i << " : ";

            if (occupied[i]) {
                Vehicle* temp = head;

                while (temp != nullptr) {
                    if (temp->slot == i) {
                        cout << "Occupied by "
                             << temp->number;
                        break;
                    }
                    temp = temp->next;
                }
            } else {
                cout << "Available";
            }

            cout << '\n';
        }

        int freeSlots = 0;

        for (int i = 1; i <= TOTAL_SLOTS; i++) {
            if (!occupied[i])
                freeSlots++;
        }

        cout << "\nTotal Slots     : " << TOTAL_SLOTS;
        cout << "\nAvailable Slots : " << freeSlots;
        cout << "\nOccupied Slots  : "
             << TOTAL_SLOTS - freeSlots << '\n';
    }

    // 3. Search vehicle
    void searchVehicle() {
        string number;

        cout << "\nEnter Vehicle Number: ";
        getline(cin >> ws, number);

        Vehicle* v = findVehicle(number);

        if (v == nullptr) {
            cout << "Vehicle not found in parking!\n";
            return;
        }

        cout << "\nVehicle Found!\n";
        cout << "Vehicle Number : " << v->number << '\n';
        cout << "Owner          : " << v->owner << '\n';
        cout << "Type           : " << v->type << '\n';
        cout << "Parking Slot   : " << v->slot << '\n';
        cout << "Entry Time     : " << v->entryTime << ":00\n";
    }

    // 4. Vehicle exit and fee calculation
    void vehicleExit() {
        string number;
        int exitTime;

        cout << "\nEnter Vehicle Number: ";
        getline(cin >> ws, number);

        Vehicle* temp = head;
        Vehicle* prev = nullptr;

        while (temp != nullptr && temp->number != number) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) {
            cout << "Vehicle is not parked!\n";
            return;
        }

        cout << "Enter Exit Time (hour, 0-23): ";

        if (!(cin >> exitTime) ||
            exitTime < 0 || exitTime > 23) {
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );
            cout << "Invalid exit time!\n";
            return;
        }

        int hours = (exitTime - temp->entryTime + 24) % 24;

        // Minimum one hour is charged
        if (hours == 0)
            hours = 24;

        double fee = hours * getRate(temp->type);

        cout << "\n========== PARKING BILL ==========\n";
        cout << "Vehicle Number : " << temp->number << '\n';
        cout << "Vehicle Type   : " << temp->type << '\n';
        cout << "Parking Slot   : " << temp->slot << '\n';
        cout << "Hours Charged  : " << hours << '\n';
        cout << "Rate Per Hour  : Rs. "
             << getRate(temp->type) << '\n';
        cout << "Total Fee      : Rs. "
             << fixed << setprecision(2) << fee << '\n';

        totalRevenue += fee;
        occupied[temp->slot] = false;

        if (prev == nullptr)
            head = temp->next;
        else
            prev->next = temp->next;

        delete temp;

        cout << "Vehicle exited successfully!\n";
    }

    // 5. Display parked vehicles
    void displayVehicles() {
        if (head == nullptr) {
            cout << "\nNo vehicles currently parked.\n";
            return;
        }

        Vehicle* temp = head;

        cout << "\n========== PARKED VEHICLES ==========\n";

        while (temp != nullptr) {
            cout << "\nVehicle : " << temp->number;
            cout << "\nOwner   : " << temp->owner;
            cout << "\nType    : " << temp->type;
            cout << "\nSlot    : " << temp->slot;
            cout << "\nEntry   : " << temp->entryTime << ":00";
            cout << "\n-------------------------------------\n";

            temp = temp->next;
        }
    }

    // 6. Revenue report
    void revenueReport() {
        cout << "\n========== REVENUE REPORT ==========\n";
        cout << "Total Parking Revenue: Rs. "
             << fixed << setprecision(2)
             << totalRevenue << '\n';
    }

    // 7. Sort parked vehicles by slot
    void sortVehiclesBySlot() {
        if (head == nullptr || head->next == nullptr) {
            cout << "Not enough vehicles to sort.\n";
            return;
        }

        bool swapped;

        do {
            swapped = false;
            Vehicle* temp = head;

            while (temp != nullptr && temp->next != nullptr) {
                Vehicle* nextVehicle = temp->next;

                if (temp->slot > nextVehicle->slot) {
                    swap(temp->number, nextVehicle->number);
                    swap(temp->owner, nextVehicle->owner);
                    swap(temp->type, nextVehicle->type);
                    swap(temp->slot, nextVehicle->slot);
                    swap(temp->entryTime, nextVehicle->entryTime);

                    swapped = true;
                }

                temp = temp->next;
            }
        } while (swapped);

        cout << "Vehicles sorted by parking slot!\n";
    }

    // Free memory
    ~SmartParking() {
        while (head != nullptr) {
            Vehicle* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    SmartParking parking;
    int choice;

    do {
        cout << "\n\n====================================";
        cout << "\n    SMART PARKING MANAGEMENT SYSTEM";
        cout << "\n====================================";
        cout << "\n1. Park Vehicle";
        cout << "\n2. Display Parking Slots";
        cout << "\n3. Search Vehicle";
        cout << "\n4. Vehicle Exit and Generate Bill";
        cout << "\n5. Display Parked Vehicles";
        cout << "\n6. Revenue Report";
        cout << "\n7. Sort Vehicles by Slot";
        cout << "\n0. Exit";
        cout << "\nEnter Your Choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Enter a number.\n";
            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );
            continue;
        }

        switch (choice) {
            case 1:
                parking.parkVehicle();
                break;
            case 2:
                parking.displaySlots();
                break;
            case 3:
                parking.searchVehicle();
                break;
            case 4:
                parking.vehicleExit();
                break;
            case 5:
                parking.displayVehicles();
                break;
            case 6:
                parking.revenueReport();
                break;
            case 7:
                parking.sortVehiclesBySlot();
                break;
            case 0:
                cout << "Thank you for using Smart Parking!\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
