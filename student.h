// student.h - Header file with class definition
#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

// BASE CLASS
class Person {
protected:
    int roll;
    string name;

public:
    Person() { roll = 0; name = ""; }
    virtual void input() = 0;
    virtual void display() = 0;
    virtual ~Person() {}
};

// DERIVED CLASS
class Student : public Person {
private:
    float ps, daa, dbms, oops;
    float total, percentage;
    string grade;

public:
    Student();
    void input();
    void calculate();
    void display();
    void save(ofstream &fout);
    bool load(ifstream &fin);
    void setName(string n);
    void setPS(float x);
    void setDAA(float x);
    void setDBMS(float x);
    void setOOPs(float x);
    void setRoll(int r);
    int getRoll();
    float getPercentage();
    string getGrade();
    string toJSON();
};

// Global variables
extern vector<Student> students;
extern const string DATA_FILE;

// Function declarations
void line();
void header();
void saveToFile();
void loadFromFile();
void addStudent();
void displayAll();
void searchStudent();
void updateStudent();
void deleteStudent();
void sortStudents();
void filterGrade();
void runConsoleMode();
void runWebServerMode();

#endif