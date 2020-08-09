#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QtGui>
#include <QLabel>

#include "TTT_state.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QPushButton *button[TTT_SIZE][TTT_SIZE];

    QHBoxLayout *row_layout[TTT_SIZE];
    QHBoxLayout *main;
    QVBoxLayout *vertical_layout;

    QWidget *ventana;




private slots:
     void button_interrupt();
     void reset();

private:
    Ui::MainWindow *ui;
    void define();




};
#endif // MAINWINDOW_H
