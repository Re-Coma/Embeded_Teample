#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <string>
#include <QMessageBox>
#include "libs/filecontrol.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

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

    void on_blackBtn_clicked();

    void on_returnBtn_clicked();

    void on_blurBtn_clicked();

    void on_gageSlider_valueChanged(int value);

    void on_sharpenBtn_clicked();

    void on_invertBtn_clicked();

    void on_detectionBtn_clicked();

private:
    bool captured; //captured!
    bool revived; //revived
    Ui::MainWindow *ui;
    FileControl fileControl;

    Mat currentCaptured; //captured picture
    Mat changedCaptured; //by opencv
    Mat gaussiantmp; //only Gaussian

    string currentFileName;

    //gaussian clicked
    bool blurSetting;
};

#endif // MAINWINDOW_H
