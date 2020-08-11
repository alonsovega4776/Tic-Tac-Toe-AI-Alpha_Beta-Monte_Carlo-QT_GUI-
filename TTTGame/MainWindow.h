#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/*
 * TTT_state.h
 *
 *  Created on: Aug 5, 2020
 *  Author: Alonso Vega
 */
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QtGui>
#include <QLabel>
#include <QDebug>

#include "MCTree.h"
#include "MinMax.h"

#define ICON_SIZE 50


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


signals:
    void ai_play_interrupt();
    void terminal_interrupt();


private slots:
    void button_ISR();
    void ai_play_ISR();
    void terminal_ISR();

private:
    Ui::MainWindow *ui;

    void reset();
    void define();
    void enable_buttons(bool en);
    void disconnect_everything();

    TTT_state state;




};
#endif // MAINWINDOW_H
