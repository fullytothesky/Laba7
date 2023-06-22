
#include "students.h"

void Students::loadFromFile() {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        // File doesn't exist or unable to open, do nothing
        return;
    }

    while (studentCount < MAX_STUDENTS && file.read(reinterpret_cast<char*>(&students[studentCount]), sizeof(Student))) {
        studentCount++;
    }

    file.close();
}

void Students::saveToFile() {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing!");
    }

    file.write(reinterpret_cast<const char*>(&students[0]), sizeof(Student) * studentCount);

    file.close();
}

void Students::addStudent(const Student& student) {
    if (studentCount < MAX_STUDENTS) {
        std::tm tmDOB = {};
        std::istringstream ss(student.dateOfBirth);
        std::string token;

        // Parse day
        if (std::getline(ss, token, '-')) {
            int day = std::stoi(token);
            if (day < 1 || day > 31) {
                throw std::invalid_argument("Invalid day. Please enter a valid day between 1 and 31.");
            }
            tmDOB.tm_mday = day;
        }

        // Parse month
        if (std::getline(ss, token, '-')) {
            int month = std::stoi(token);
            if (month < 1 || month > 12) {
                throw std::invalid_argument("Invalid month. Please enter a valid month between 1 and 12.");
            }
            tmDOB.tm_mon = month - 1;  // Month is zero-based in std::tm
        }

        // Parse year
        if (std::getline(ss, token, '-'))
            tmDOB.tm_year = std::stoi(token) - 1900;  // Year is relative to 1900 in std::tm

        std::time_t timeDOB = std::mktime(&tmDOB);

        std::time_t currentTime = std::time(nullptr);
        std::tm* tmCurrent = std::localtime(&currentTime);

        // Compare dates
        if ((timeDOB > currentTime || tmDOB.tm_year > tmCurrent->tm_year) ||
            (tmDOB.tm_year == tmCurrent->tm_year && tmDOB.tm_mon > tmCurrent->tm_mon)
            || (tmDOB.tm_year == tmCurrent->tm_year && tmDOB.tm_mon == tmCurrent->tm_mon && tmDOB.tm_mday > tmCurrent->tm_mday)) {
            throw std::invalid_argument("Invalid date of birth. The date cannot be greater than the current date.");
        }

        students[studentCount++] = student;
        saveToFile();
    }
    else {
        throw std::runtime_error("Maximum number of students reached!");
    }
}

void Students::searchStudentsByMonth(const std::string& month) {
    int searchMonth = std::stoi(month);
    if (searchMonth < 1 || searchMonth > 12) {
        throw std::invalid_argument("Invalid month. Please enter a valid month between 1 and 12.");
    }

    bool found = false;

    for (int i = 0; i < studentCount; i++) {
        int studentMonth = std::stoi(students[i].dateOfBirth.substr(3, 2));

        if (studentMonth == searchMonth) {
            QMessageBox::information(nullptr, "Search Result", "Name: " + QString::fromStdString(students[i].name) +
                "\nClass Number: " + QString::number(students[i].classNumber));
            found = true;
        }
    }

    if (!found) {
        QMessageBox::information(nullptr, "Search Result", "No students found for the given month.");
    }
}

std::string Students::getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    auto nowTime = std::chrono::system_clock::to_time_t(now);
    std::string currentDate = std::ctime(&nowTime);

    // Remove newline character from the end of the string
    currentDate.erase(currentDate.find_last_not_of("\n") + 1);

    return currentDate;
}

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Student Search");

    
        QLabel* nameLabel = new QLabel("Name:");
    nameLineEdit = new QLineEdit;
    QLabel* classNumberLabel = new QLabel("Class Number:");
    classNumberLineEdit = new QLineEdit;
    QLabel* dobLabel = new QLabel("Date of Birth (DD-MM-YYYY):");
    dobLineEdit = new QLineEdit;
    QPushButton* addButton = new QPushButton("Add Student");
    QLabel* monthLabel = new QLabel("Enter the month (MM) to search for students:");
    monthLineEdit = new QLineEdit;
    QPushButton* searchButton = new QPushButton("Search");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(nameLabel);
    layout->addWidget(nameLineEdit);
    layout->addWidget(classNumberLabel);
    layout->addWidget(classNumberLineEdit);
    layout->addWidget(dobLabel);
    layout->addWidget(dobLineEdit);
    layout->addWidget(addButton);
    layout->addWidget(monthLabel);
    layout->addWidget(monthLineEdit);
    layout->addWidget(searchButton);

    setLayout(layout);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::handleAddStudent);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::handleSearchStudents);
}

void MainWindow::handleAddStudent() {
    std::string name = nameLineEdit->text().toStdString();
    int classNumber = classNumberLineEdit->text().toInt();
    std::string dob = dobLineEdit->text().toStdString();

    Student student(name, classNumber, dob);

    try {
        studentData.addStudent(student);
        QMessageBox::information(this, "Success", "Student added successfully!");
    }
    catch (const std::exception& ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
}

void MainWindow::handleSearchStudents() {
    std::string month = monthLineEdit->text().toStdString();

    try {
        studentData.searchStudentsByMonth(month);
    }
    catch (const std::exception& ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
}


/*void Students::addStudent(const Student& student) {
    if (studentCount < MAX_STUDENTS) {
        std::tm tmDOB = {};
        std::istringstream ss(student.dateOfBirth);
        std::string token;

        // Parse day
        if (std::getline(ss, token, '-')) {
            int day = std::stoi(token);
            if (day < 1 || day > 31) {
                throw std::invalid_argument("Invalid day. Please enter a valid day between 1 and 31.");
            }
            tmDOB.tm_mday = day;
        }

        // Parse month
        if (std::getline(ss, token, '-')) {
            int month = std::stoi(token);
            if (month < 1 || month > 12) {
                throw std::invalid_argument("Invalid month. Please enter a valid month between 1 and 12.");
            }
            tmDOB.tm_mon = month - 1;  // Month is zero-based in std::tm
        }

        // Parse year
        if (std::getline(ss, token, '-'))
            tmDOB.tm_year = std::stoi(token) - 1900;  // Year is relative to 1900 in std::tm

        std::time_t timeDOB = std::mktime(&tmDOB);

        std::time_t currentTime = std::time(nullptr);
        std::tm* tmCurrent = std::localtime(&currentTime);

        // Compare dates
        if ((timeDOB > currentTime || tmDOB.tm_year > tmCurrent->tm_year) ||
            (tmDOB.tm_year == tmCurrent->tm_year && tmDOB.tm_mon > tmCurrent->tm_mon)
            || (tmDOB.tm_year == tmCurrent->tm_year && tmDOB.tm_mon == tmCurrent->tm_mon && tmDOB.tm_mday > tmCurrent->tm_mday)) {
            throw std::invalid_argument("Invalid date of birth. The date cannot be greater than the current date.");
        }

        students[studentCount++] = student;
        saveToFile();
    }
    else {
        throw std::runtime_error("Maximum number of students reached!");
    }
}*/