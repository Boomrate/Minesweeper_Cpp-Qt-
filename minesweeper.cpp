#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QGridLayout"
#include "QPushButton"
#include <QTimer>
#include <QMainWindow>
#include <QString>
#include <QLCDNumber>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    //Hello Message
    QMessageBox *dialog=new QMessageBox();
    dialog->information(this,tr("GameManager"),tr(">>Hello there! Welcome to Minesweeper!! \n-You got a 10x10 board and %1 bombs to find!! \n GoodLuck!").arg(bb));

    //RestartButton Initialize
    QPushButton *actionReboot=new QPushButton();
    actionReboot->setText("Restart");
    connect( actionReboot, SIGNAL (released()),this, SLOT (slotReboot()));

    //Time Initialize
    timer=new QTimer(this);
    connect(timer, SIGNAL (timeout()),this, SLOT(setTimer()));
    timer->start(1000);

    //Setflag Button Initialize
    QPushButton *flag= new QPushButton();
    flag->setText("SetFlag");
    connect(flag, SIGNAL (released()),this, SLOT(setFlag()));

    //RemoveFlag Button Initialize
    QPushButton *removeF= new QPushButton();
    removeF->setText("RemoveFlag");
    connect(removeF, SIGNAL (released()),this, SLOT(removeFlag()));

    //Quit Button Initialize
    QPushButton *quitButton= new QPushButton();
    quitButton->setText("Quit");
    connect(quitButton, SIGNAL (released()),this, SLOT(quitApp()));

    timeDisplay->setPalette(Qt::black);
    timeDisplay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //Toolbar Initialize
    QToolBar *toolbr=new QToolBar();
    toolbr->addWidget(actionReboot);
    toolbr->addWidget(quitButton);
    toolbr->addWidget(timeDisplay);
    toolbr->addWidget(flag);
    toolbr->addWidget(removeF);

    //Button Board Initialize
    QGridLayout *grd=new QGridLayout;
    QPixmap pixmap(QString(path+"closed.png"));
    QIcon ButtonIcon(pixmap);

    for(int x=0;x<bx;x++){
        for(int y=0;y<by;y++){
            board[x][y]=new QPushButton;
            board[x][y]->setIcon(ButtonIcon);
            board[x][y]->setIconSize(pixmap.rect().size());
            backBoard[x][y]='0';
            connect(board[x][y], SIGNAL (released()),this, SLOT (handleButton()));
            grd->addWidget(board[x][y],x,y);
        }
    }
    grd->setVerticalSpacing(0);
    grd->setHorizontalSpacing(0);

    QWidget *window = new QWidget();
    window->setLayout(grd);

    generate_Bombs();

    //Window initialize
    this->setWindowTitle(QString("Minesweeper"));
    this->addToolBar(toolbr);
    setCentralWidget(window);
    setWindowIcon(QIcon(QPixmap(QString(path+"X.png"))));
    //this->setFixedSize(QSize(420,380));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTimer()
{
    if(sec<59)
        sec++;
    else{
        sec=0;
        min++;
    }
    if(sec<10)
        timeDisplay->display(QString("0%1:0%2").arg(min).arg(sec));
    else
        timeDisplay->display(QString("0%1:%2").arg(min).arg(sec));
}

void MainWindow::handleButton()//Action listener for the buttons.
 {
    QObject* obj = sender(); // Get the name of the button clicked.
    QString str;
    for(int x=0;x<bx;x++){
        for(int y=0;y<by;y++){
                if(obj==board[x][y]) //When you find it:
                {
                    if(flagValue==1 && backBoard[x][y]!='*') //If setFlag was pressed, the next button going to change img(with flag).
                    {
                        QPixmap pixmap(QString(path+"F.png"));
                        QIcon ButtonIcon(pixmap);
                        board[x][y]->setIcon(QIcon());
                        board[x][y]->repaint();

                        board[x][y]->setIcon(ButtonIcon);
                        board[x][y]->setIconSize(pixmap.rect().size());
                        flagValue=0;
                    }
                    else if(flagValue==0 && backBoard[x][y]!='*') // If neither setFlag | removeFlag pressed and the cell isnt opened,then OpenCell.
                    {
                        open_cell(x,y);
                    }
                    else if(flagValue==2 && backBoard[x][y]!='*') //If the removeFlag was pressed, the next button going to change img(with noflag,if has one).
                    {
                        QPixmap pixmap(QString(path+"closed.png"));
                        QIcon ButtonIcon(pixmap);
                        board[x][y]->setIcon(QIcon());
                        board[x][y]->repaint();

                        board[x][y]->setIcon(ButtonIcon);
                        board[x][y]->setIconSize(pixmap.rect().size());
                        flagValue=0;
                    }
            }
        }
    }
 }


void MainWindow::slotReboot()
{
 qDebug() << "Performing application reboot...";
 qApp->exit( EXIT_CODE_REBOOT );
}

void MainWindow::quitApp()
{
    QApplication::quit();
}


void MainWindow::setFlag() //On the next button pressed gonna set flag, not open.
 {
    flagValue=1;
}

void MainWindow::removeFlag() //On the next button pressed gonna remove flag(if exists),not open.
 {
    flagValue=2;
 }


generate_Bombs::generate_Bombs(){ //Randomly places the bombs on the backboard.
        srand(time(0));

        for(cnt=0;cnt<bb;cnt++){   //Randomly selects 10 cells.
            x=rand()%10;
            y=rand()%10;

        if(backBoard[x][y]=='X'){  //Checks if the cell already has a bomb or
            cnt--;				   //if it is out of the borders
            continue;			   //so it reselects a new random cell.
        }

        backBoard[x][y]='X';      //Gives the cell a bomb.

        //For each cell around the bomb , it calls the function
        //update_Board ( it counts the bombs around the cell ).
        update_Board(x-1,y-1);
        update_Board(x-1,y);
        update_Board(x-1,y+1);

        update_Board(x,y-1);
        update_Board(x,y+1);

        update_Board(x+1,y-1);
        update_Board(x+1,y);
        update_Board(x+1,y+1);
        }
}

void generate_Bombs::update_Board(int x, int y){ //Counts how many bombs are around a cell.
    if(backBoard[x][y]!='X' && x>=0 && x<bx && y<by && y>=0){
        backBoard[x][y]++;
    }
}

void endGame(){ //Called When you hit a bomb and reveals the cells.

    timer->stop();

    for(int x=0;x<bx;x++){
        for(int y=0;y<by;y++){
            if(backBoard[x][y]=='X'){
                QPixmap pixmap(QString(path+"X.png"));
                QIcon ButtonIcon(pixmap);
                board[x][y]->setIcon(QIcon());
                board[x][y]->repaint();

                board[x][y]->setIcon(ButtonIcon);
                board[x][y]->setIconSize(pixmap.rect().size());
                board[x][y]->setEnabled(false);
            }
            else if(backBoard[x][y]>='0'){
                QString url=path;
                switch( backBoard[x][y] ){
                case '0':
                    url+="0.png";
                    break;
                case '1':
                    url+="1.png";
                    break;
                case '2':
                    url+="2.png";
                    break;
                case '3':
                    url+="3.png";
                    break;
                case '4':
                    url+="4.png";
                    break;
                case '5':
                    url+="5.png";
                    break;
                case '6':
                    url+="6.png";
                    break;
                case '7':
                    url+="7.png";
                    break;
                case '8':
                    url+="8.png";
                    break;
                }
                QPixmap pixmap(url);
                QIcon ButtonIcon(pixmap);

                board[x][y]->setIcon(QIcon());
                board[x][y]->repaint();
                board[x][y]->setIcon(ButtonIcon);
                board[x][y]->setIconSize(pixmap.rect().size());
            }
        }
    }
}


void MainWindow::open_cell(int x,int y){ //Reveals what the cell contains (bomb or number).
    if(x>=0 && x<bx && y>=0 && y<by)
    {
        if(backBoard[x][y]=='X' && backBoard[x][y]!='*')  //You hit bomb!
        {
            QPixmap pixmap(QString(path+"X.png"));
            QIcon ButtonIcon(pixmap);
            board[x][y]->setIcon(QIcon());
            board[x][y]->repaint();

            backBoard[x][y]='*'; //cell opened mark.
            board[x][y]->setIcon(ButtonIcon);
            board[x][y]->setIconSize(pixmap.rect().size());

            QMessageBox *dialog=new QMessageBox();
            dialog->information(this,tr("GameManager"),tr(" >>You Lose!           "));

            board[x][y]->setEnabled(false);
            endGame();
        }
        else if(backBoard[x][y]>'0' && backBoard[x][y]!='*')
        {
            QString url=path;
            cells--; //a cell opened.

            switch( backBoard[x][y] )
            {
                case '1':
                    url+="1.png";
                    break;
                case '2':
                    url+="2.png";
                    break;
                case '3':
                    url+="3.png";
                    break;
                case '4':
                    url+="4.png";
                    break;
                case '5':
                    url+="5.png";
                    break;
                case '6':
                    url+="6.png";
                    break;
                case '7':
                    url+="7.png";
                    break;
                case '8':
                    url+="8.png";
                    break;
            }
            QPixmap pixmap(url);
            QIcon ButtonIcon(pixmap);

            backBoard[x][y]='*';  //cell opened mark.
            board[x][y]->setIcon(QIcon());
            board[x][y]->repaint();
            board[x][y]->setIcon(ButtonIcon);
            board[x][y]->setIconSize(pixmap.rect().size());

            if(cells==0){//If all cells without bomb have been opened, You win.
                QMessageBox *dialog=new QMessageBox();
                dialog->information(this,tr("GameManager"),tr(" >>Winner Winner! Chicken Dinner!! :D        "));
                timer->stop();
            }
        }

        if(backBoard[x][y]=='0' && backBoard[x][y]!='*'){  // If its 0, recall this function for every cell around it.
                    cells--; //a cell opened.
                    QPixmap pixmap(QString(path+"0.png"));
                    QIcon ButtonIcon(pixmap);

                    backBoard[x][y]='*'; //cell opened mark.
                    board[x][y]->setIcon(QIcon());
                    board[x][y]->repaint();
                    board[x][y]->setIcon(ButtonIcon);
                    board[x][y]->setIconSize(pixmap.rect().size());

                    open_cell(x-1,y-1);
                    open_cell(x-1,y);
                    open_cell(x-1,y+1);

                    open_cell(x,y-1);
                    open_cell(x,y+1);

                    open_cell(x+1,y-1);
                    open_cell(x+1,y);
                    open_cell(x+1,y+1);

                    if(cells==0){ //If all cells without bomb have been opened, You win.
                        QMessageBox *dialog=new QMessageBox();
                        dialog->information(this,tr("GameManager"),tr(" >>Winner Winner! Chicken Dinner!! :D        "));
                        timer->stop();
                    }
                }

    }
}
