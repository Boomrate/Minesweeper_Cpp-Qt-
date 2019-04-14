#include "mainwindow.h"
#include <QApplication>
#include "QGridLayout"
#include "QPushButton"
#include <QWidget>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    int currentExitCode = 0;

    do {
     QApplication a(argc, argv);
     MainWindow w;
     w.show();
     currentExitCode = a.exec();
    } while( currentExitCode == EXIT_CODE_REBOOT );

    return currentExitCode;
}

