#pragma once
#ifndef STUDENTS_H
#define STUDENTS_H

#include <string>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <sstream>
#include<fstream>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QVBoxLayout>

class Student {
public:
    std::string name;
    int classNumber;
    std::string dateOfBirth;

    Student() {}

    Student(const std::string& name, int classNumber, const std::string& dateOfBirth)
        : name(name), classNumber(classNumber), dateOfBirth(dateOfBirth) {}
};

class Students {
private:
    const std::string filename = "student_data.bin";
    const int MAX_STUDENTS = 5;
    Student students[5];
    int studentCount;

public:
    Students() : studentCount(0) {}

    void loadFromFile();
    void saveToFile();
    void addStudent(const Student& student);
    void searchStudentsByMonth(const std::string& month);

private:
    std::string getCurrentDate();
};

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    Students studentData;
    QLineEdit* nameLineEdit;
    QLineEdit* classNumberLineEdit;
    QLineEdit* dobLineEdit;
    QLineEdit* monthLineEdit;

    void handleAddStudent();
    void handleSearchStudents();
};

#endif