/*
 * MainWindow.cpp
 *
 *  Created on: Aug 5, 2020
 *  Author: Alonso Vega
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString titulo = "%1,%2,%3 Game";
    titulo = titulo.arg(TTT_SIZE).arg(TTT_SIZE).arg(TTT_WIN_SIZE);
    this->setWindowTitle(titulo);
    this->setGeometry(0,0,this->minimumWidth(),this->minimumHeight());

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    this->setAutoFillBackground(true);
    this->setPalette(pal);


    define();
    qDebug() <<"\n First Player: " << state.get_player();
    if (state.get_player() == 'X') emit ai_play_interrupt();


}


MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::reset()
{
    int i, j;
    LOOP(i, 0, TTT_SIZE)
        LOOP(j, 0, TTT_SIZE)
        {
            button[i][j] = new QPushButton(this);

            QString label = "%1,%2";
            label = label.arg(i).arg(j);
            button[i][j]->setText(label);

            //QPixmap pixmap("/Users/xXxMrMayhemxXx/Documents/GitHub/Tic-Tac-Toe-AI-Alpha_Beta-Monte_Carlo-QT_GUI-/TTTGame/img/nada.png");
            //QIcon ButtonIcon(pixmap);
            //button[i][j]->setIcon(QIcon());

            button[i][j]->setStyleSheet("font: bold;"
                                        "background-color: gray;"
                                        "font-size: 15px;"
                                        "font-family: Times New Roman;"
                                        "font-style: italic;"
                                        "color: white;"
                                        "border-width: 1px;"
                                        "border-radius: 0px;"
                                        "border-style: outset;"
                                        "border-color: gray;"
                                        "height: 50px;"
                                        "width: 50px;");

            button[i][j]->setIconSize(QSize(ICON_SIZE, ICON_SIZE));

            button[i][j]->setProperty("row", i);
            button[i][j]->setProperty("col", j);

            connect(button[i][j], SIGNAL(clicked()), this, SLOT(button_ISR()));

        }
    connect(this, &MainWindow::ai_play_interrupt, this, &MainWindow::ai_play_ISR);
    connect(this, &MainWindow::terminal_interrupt, this, &MainWindow::terminal_ISR);

}


void MainWindow::define()
{
    reset();

    ventana = new QWidget();



    int i,j;
    LOOP(i,0,TTT_SIZE)
    {
            row_layout[i] = new QHBoxLayout();
            row_layout[i]->setSpacing(0);
    }
    vertical_layout = new QVBoxLayout();
    vertical_layout->setSpacing(0);
    main = new QHBoxLayout();
    main->setSpacing(0);

    LOOP(i,0,TTT_SIZE)
            LOOP(j,0,TTT_SIZE)
                row_layout[i]->addWidget(button[i][j]);

    LOOP(i,0,TTT_SIZE) vertical_layout->addLayout(row_layout[i]);


    main->addLayout(vertical_layout);


    ventana->setLayout(main);
    MainWindow::setCentralWidget(ventana);
}

void MainWindow::enable_buttons(bool en)
{
    int i, j;
    LOOP(i,0,TTT_SIZE)
            LOOP(j,0,TTT_SIZE)
                button[i][j]->setEnabled(en);
}

void MainWindow::disconnect_everything()
{
    int i,j;
    LOOP(i, 0, TTT_SIZE)
        LOOP(j, 0, TTT_SIZE)
            disconnect(button[i][j], SIGNAL(clicked()), this, SLOT(button_ISR()));

    disconnect(this, &MainWindow::ai_play_interrupt, this, &MainWindow::ai_play_ISR);
    disconnect(this, &MainWindow::terminal_interrupt, this, &MainWindow::terminal_ISR);

}



//----------------------------------------------AI Interrupt Handler-----------------------------------------------------
void MainWindow::ai_play_ISR()
{
    int i, j;
    enable_buttons(false);

    MCTree mytree;
    tuple<int, int> optimal;

    int t = state.get_time();
    if (t > 50)      optimal = MinMax::αβ_search(state);
    else if (t < 50)  optimal = mytree.UCT_search(state, 0.5, 1000); //satisfies Hoeffding Ineqality: 1.0/sqrt(2.0)
    else             optimal = mytree.UCT_search(state, 0.85, 5000);

    i = std::get<0>(optimal);
    j = std::get<1>(optimal);

    qDebug() << "\n" << state.get_player() << ": ";
    qDebug() << "(" << i << ", " << j << ")";

    state.transition(i, j);

    button[i][j]->setText(QString());
    QPixmap pixmap("/Users/xXxMrMayhemxXx/Documents/GitHub/Tic-Tac-Toe-AI-Alpha_Beta-Monte_Carlo-QT_GUI-/Source/NMK_Game/img/blanca.png");
    QIcon ButtonIcon(pixmap);

    button[i][j]->setIcon(ButtonIcon);
    button[i][j]->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
    button[i][j]->disconnect();


    if (state.get_termina()) emit terminal_interrupt();
    enable_buttons(true);
}
//-------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------Button Interrupt Handler---------------------------------------------------
void MainWindow::button_ISR()
{
    QPushButton * button_pushed = ((QPushButton *)sender());

    int i_1 = button_pushed->property("row").toInt();
    int i_2 = button_pushed->property("col").toInt();


    qDebug() << "\n" << state.get_player() << ": ";
    qDebug() << "(" << i_1 << ", " << i_2 << ")";

    state.transition(i_1, i_2);


    button_pushed->setText(QString());
    QPixmap pixmap("/Users/xXxMrMayhemxXx/Documents/GitHub/Tic-Tac-Toe-AI-Alpha_Beta-Monte_Carlo-QT_GUI-/Source/NMK_Game/img/negra.png");
    QIcon ButtonIcon(pixmap);
    button_pushed->setIcon(ButtonIcon);
    button_pushed->setIconSize(QSize(ICON_SIZE, ICON_SIZE));

    button_pushed->disconnect();

    if (state.get_termina()) emit terminal_interrupt();
    emit ai_play_interrupt();

}
//--------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------Terminal Interrupt Handler--------------------------------------------------
void MainWindow::terminal_ISR()
{
    enable_buttons(false);
    disconnect_everything();


    char winner = state.get_player();
    if (winner == 'O')
    {
        QMessageBox::information(this, "Lose", "ΑΙ has won :(");
    }
    else if (winner == 'X')
    {
        QMessageBox::information(this, "Win", "You has won :)");
    }
    else thrrow("ERROR: TERMINAL ISR")
}
//--------------------------------------------------------------------------------------------------------------------------


