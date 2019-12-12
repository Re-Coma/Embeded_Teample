#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <string>
#include <QMessageBox>
#include "libs/filecontrol.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_exitBtn_clicked();

    void on_captureBtn_clicked();

    //void on_prevBtn_clicked();

    //void on_nextBtn_clicked();

    void on_saveBtn_clicked();

    void on_printBtn_clicked();

private:
    bool captured; //captured!
    bool revived; //revived
    Ui::MainWindow *ui;
    FileControl fileControl;
};

#endif // MAINWINDOW_H
