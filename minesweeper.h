#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QPushButton"
#include <iostream>
#include <QString>
#include <vector>
#include <QTimer>
#include <QLabel>
#include <QLCDNumber>

static const int EXIT_CODE_REBOOT = -123456789;

static int bx=10,by=10,bb=10; //Board X-axis,Y-axis lenght, Board bombs number
static QPushButton *board[10][10]; //Button board
static char backBoard[10][10]; //BackButton values board
static QTimer *timer;

//CHANGE IMAGE FOLDER PATH BEFORE RUN!
static QString path="C:\\Users\\Public\\Documents\\Qt\\Minesweeper\\images\\";

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void setTimer();
    void slotReboot(); //restart app

private:
    Ui::MainWindow *ui;
    QLCDNumber *timeDisplay=new QLCDNumber();
    int cells=(bx*by)*bb; //Num of cells player have to open in order to win.
    int min=0,sec=0; // timer valuess.
    int flagValue=0;

private slots:
   void handleButton(); //Action listener for the buttons.
   void setFlag(); //Next cell going to be flaged
   void quitApp(); //close app
   void removeFlag(); // next cell going to unflaged
   void open_cell(int x,int y); //Reveals what the cell contains (bomb or number).
};


class generate_Bombs{
private:
    int x,y,cnt;
public:
    generate_Bombs(); //Randomly places the bombs on the backboard.

    void update_Board(int x, int y); //Counts how many bombs are around a cell.
};


#endif // MAINWINDOW_H
