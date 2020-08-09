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

            connect(button[i][j], SIGNAL(clicked()), this, SLOT(button_interrupt()));

        }



}

void MainWindow::button_interrupt()
{
   // QMessageBox::information(this, "d" , "Button has been clicked!");

    QPushButton * button_pushed = ((QPushButton *)sender());

    button_pushed->setText(QString());

    QPixmap pixmap("/Users/xXxMrMayhemxXx/Documents/GitHub/Tic-Tac-Toe-AI-Alpha_Beta-Monte_Carlo-QT_GUI-/TTTGame/img/p2.png");
    QIcon ButtonIcon(pixmap);
    button_pushed->setIcon(ButtonIcon);
    button_pushed->setIconSize(QSize(ICON_SIZE, ICON_SIZE));

    button_pushed->disconnect();

}




void MainWindow::define()
{
    ventana = new QWidget();

    reset();


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



