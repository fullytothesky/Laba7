#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include "students.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    Students studentData;
    try {
        studentData.loadFromFile();

        MainWindow mainWindow;
        mainWindow.show();

        return app.exec();
    }
    catch (const std::exception& ex) {
        QMessageBox::critical(nullptr, "Error", ex.what());
        return 1;
    }
}