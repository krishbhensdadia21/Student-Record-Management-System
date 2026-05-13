// Miniproject.cpp - Implementation of all functions
#include "student.h"
#include <cstdlib>
using namespace std;

// Global variables
vector<Student> students;
const string DATA_FILE = "student.txt";

// Student Constructor
Student::Student() {
    ps = daa = dbms = oops = total = percentage = 0;
    grade = "";
    roll = 0;
    name = "";
}

void Student::input() {
    try {
        cout << "\nEnter Roll: ";
        cin >> roll;
        if (roll <= 0) throw roll;

        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Enter PS: ";
        cin >> ps;
        if (ps < 0 || ps > 100) throw ps;

        cout << "Enter DAA: ";
        cin >> daa;
        if (daa < 0 || daa > 100) throw daa;

        cout << "Enter DBMS: ";
        cin >> dbms;
        if (dbms < 0 || dbms > 100) throw dbms;

        cout << "Enter OOPs: ";
        cin >> oops;
        if (oops < 0 || oops > 100) throw oops;
    }
    catch (...) {
        cout << "Invalid Input! Try Again\n";
        input();
    }
}

void Student::calculate() {
    total = ps + daa + dbms + oops;
    percentage = total / 4;

    if (percentage >= 90) grade = "AA";
    else if (percentage >= 80) grade = "AB";
    else if (percentage >= 70) grade = "BB";
    else if (percentage >= 60) grade = "CC";
    else if (percentage >= 50) grade = "DD";
    else grade = "FF";
}

void Student::display() {
    cout << "| " << left << setw(6) << roll
         << "| " << setw(20) << name
         << "| " << setw(5) << ps
         << "| " << setw(5) << daa
         << "| " << setw(6) << dbms
         << "| " << setw(6) << oops
         << "| " << setw(7) << total
         << "| " << setw(6) << percentage
         << "| " << setw(6) << grade << "|\n";
}

void Student::save(ofstream &fout) {
    fout << roll << "|" << name << "|"
         << ps << "|" << daa << "|"
         << dbms << "|" << oops << endl;
}

bool Student::load(ifstream &fin) {
    string line;
    if (getline(fin, line)) {
        int pos;
        pos = line.find('|');
        roll = atoi(line.substr(0, pos).c_str());
        line.erase(0, pos + 1);

        pos = line.find('|');
        name = line.substr(0, pos);
        line.erase(0, pos + 1);

        pos = line.find('|');
        ps = atof(line.substr(0, pos).c_str());
        line.erase(0, pos + 1);

        pos = line.find('|');
        daa = atof(line.substr(0, pos).c_str());
        line.erase(0, pos + 1);

        pos = line.find('|');
        dbms = atof(line.substr(0, pos).c_str());
        line.erase(0, pos + 1);

        oops = atof(line.c_str());
        calculate();
        return true;
    }
    return false;
}

void Student::setName(string n) { name = n; calculate(); }
void Student::setPS(float x) { if (x >= 0 && x <= 100) { ps = x; calculate(); } }
void Student::setDAA(float x) { if (x >= 0 && x <= 100) { daa = x; calculate(); } }
void Student::setDBMS(float x) { if (x >= 0 && x <= 100) { dbms = x; calculate(); } }
void Student::setOOPs(float x) { if (x >= 0 && x <= 100) { oops = x; calculate(); } }
void Student::setRoll(int r) { 
    roll = r; 
}
int Student::getRoll() { return roll; }
float Student::getPercentage() { return percentage; }
string Student::getGrade() { return grade; }

string Student::toJSON() {
    stringstream ss;
    ss << "{"
       << "\"roll\":" << roll << ","
       << "\"name\":\"" << name << "\","
       << "\"ps\":" << ps << ","
       << "\"daa\":" << daa << ","
       << "\"dbms\":" << dbms << ","
       << "\"oops\":" << oops << ","
       << "\"total\":" << total << ","
       << "\"percentage\":" << percentage << ","
       << "\"grade\":\"" << grade << "\""
       << "}";
    return ss.str();
}

// ==================== CONSOLE FUNCTIONS ====================
void line() {
    cout << "----------------------------------------------------------------------------------\n";
}

void header() {
    line();
    cout << "| " << left << setw(6) << "Roll"
         << "| " << setw(20) << "Name"
         << "| " << setw(5) << "PS"
         << "| " << setw(5) << "DAA"
         << "| " << setw(6) << "DBMS"
         << "| " << setw(6) << "OOPs"
         << "| " << setw(7) << "Total"
         << "| " << setw(6) << "%"
         << "| " << setw(6) << "Grade" << "|\n";
    line();
}

void saveToFile() {
    ofstream fout(DATA_FILE);
    for (auto &student : students) {
        student.save(fout);
    }
    fout.close();
    cout << "Data saved to " << DATA_FILE << endl;
}

void loadFromFile() {
    ifstream fin(DATA_FILE);
    if (!fin.is_open()) {
        cout << "No existing data file. Starting fresh.\n";
        return;
    }
    
    students.clear();
    Student temp;
    while (temp.load(fin)) {
        students.push_back(temp);
    }
    fin.close();
    cout << "Loaded " << students.size() << " records from " << DATA_FILE << endl;
}

void addStudent() {
    system("cls");
    Student temp;
    temp.input();
    temp.calculate();
    students.push_back(temp);
    saveToFile();
    cout << "Added Successfully!\n";
    system("pause");
}

void displayAll() {
    system("cls");
    header();
    for (int i = 0; i < students.size(); i++) {
        students[i].display();
    }
    line();
    system("pause");
}

void searchStudent() {
    system("cls");
    int r;
    cout << "Enter Roll: ";
    cin >> r;

    bool found = false;
    header();

    for (int i = 0; i < students.size(); i++) {
        if (students[i].getRoll() == r) {
            students[i].display();
            found = true;
        }
    }

    if (!found) {
        cout << "| No Record Found!                                                           |\n";
    }

    line();
    system("pause");
}

void updateStudent() {
    system("cls");
    int r;
    cout << "Enter Roll to Update: ";
    cin >> r;

    for (int i = 0; i < students.size(); i++) {
        if (students[i].getRoll() == r) {
            int choice;
            do {
                system("cls");
                cout << "\n--- Update Menu ---";
                cout << "\n1. Update Name";
                cout << "\n2. Update PS";
                cout << "\n3. Update DAA";
                cout << "\n4. Update DBMS";
                cout << "\n5. Update OOPs";
                cout << "\n6. Update All";
                cout << "\n7. Exit";

                cout << "\nEnter choice: ";
                cin >> choice;

                switch (choice) {
                    case 1: {
                        string name;
                        cout << "Enter New Name: ";
                        cin.ignore();
                        getline(cin, name);
                        students[i].setName(name);
                        break;
                    }
                    case 2: {
                        float ps;
                        cout << "Enter New PS: ";
                        cin >> ps;
                        students[i].setPS(ps);
                        break;
                    }
                    case 3: {
                        float daa;
                        cout << "Enter New DAA: ";
                        cin >> daa;
                        students[i].setDAA(daa);
                        break;
                    }
                    case 4: {
                        float dbms;
                        cout << "Enter New DBMS: ";
                        cin >> dbms;
                        students[i].setDBMS(dbms);
                        break;
                    }
                    case 5: {
                        float oops;
                        cout << "Enter New OOPs: ";
                        cin >> oops;
                        students[i].setOOPs(oops);
                        break;
                    }
                    case 6: {
                        cout << "Enter Full Data:\n";
                        students[i].input();
                        break;
                    }
                    case 7:
                        break;
                    default:
                        cout << "Invalid!\n";
                        system("pause");
                }
                students[i].calculate();
            } while (choice != 7);

            saveToFile();
            cout << "Updated Successfully!\n";
            system("pause");
            return;
        }
    }
    cout << "Not Found!\n";
    system("pause");
}

void deleteStudent() {
    system("cls");
    int r;
    cout << "Enter Roll to Delete: ";
    cin >> r;

    for (int i = 0; i < students.size(); i++) {
        if (students[i].getRoll() == r) {
            students.erase(students.begin() + i);
            saveToFile();
            cout << "Deleted!\n";
            system("pause");
            return;
        }
    }
    cout << "Not Found!\n";
    system("pause");
}

void sortStudents() {
    system("cls");
    for (int i = 0; i < students.size() - 1; i++) {
        for (int j = i + 1; j < students.size(); j++) {
            if (students[i].getPercentage() < students[j].getPercentage()) {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
    saveToFile();
    cout << "Sorted by Percentage!\n";
    system("pause");
}

void filterGrade() {
    system("cls");
    string g;
    cout << "Enter Grade: ";
    cin >> g;

    header();
    for (int i = 0; i < students.size(); i++) {
        if (students[i].getGrade() == g) {
            students[i].display();
        }
    }
    line();
    system("pause");
}

void runConsoleMode() {
    int ch;
    while (true) {
        system("cls");
        cout << "\n===== Student Record Management System (CONSOLE MODE) =====";
        cout << "\n1. Add Student Record";
        cout << "\n2. Display All Student Records";
        cout << "\n3. Search Student Record";
        cout << "\n4. Update Student Record";
        cout << "\n5. Delete Student Record";
        cout << "\n6. Sort Students Record";
        cout << "\n7. Filter Students Record";
        cout << "\n8. Save Records to File";
        cout << "\n9. Load Records from File";
        cout << "\n10. Exit to Main Menu";

        cout << "\nEnter choice: ";
        cin >> ch;

        switch (ch) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: sortStudents(); break;
            case 7: filterGrade(); break;
            case 8: saveToFile(); break;
            case 9: loadFromFile(); break;
            case 10: return;
            default: cout << "Invalid!"; system("pause");
        }
    }
}