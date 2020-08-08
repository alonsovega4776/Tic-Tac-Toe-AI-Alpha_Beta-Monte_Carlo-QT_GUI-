#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;


}

void MainWindow::set_buttons()
{
    int i, j;
    LOOP(i, 0, 3)
        LOOP(j, 0, 3)
        {
            botton[i][j] = new QPushButton(this);

        }


}



