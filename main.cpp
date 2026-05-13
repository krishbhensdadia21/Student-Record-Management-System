// main.cpp - Main menu
#include "student.h"
#include <cstdlib>
using namespace std;

int main() {
    loadFromFile();
    int choice;

    while (true) {
        system("cls");
        cout << "\n========================================\n";
        cout << "STUDENT RECORD MANAGEMENT SYSTEM\n";
        cout << "========================================\n";
        cout << "1. Console Mode (Text-based Menu)\n";
        cout << "2. Web Server Mode (Browser Interface)\n";
        cout << "3. Exit\n";
        cout << "========================================\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                runConsoleMode();
                break;
            case 2:
                runWebServerMode();
                break;
            case 3:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice!\n";
                system("pause");
        }
    }

    return 0;
}