#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>

#define LOOP(var, i, end_plus1) for (var = i; var < end_plus1; var++)

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setButtons();

private:
    Ui::MainWindow *ui;

    QWidget* wdg;


};
#endif // MAINWINDOW_H
