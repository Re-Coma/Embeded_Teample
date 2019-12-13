#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <string>
#include <QMessageBox>
#include "libs/filecontrol.hpp"

#include <QTimer>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/io.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

/* Push button */
#define MAX_BUTTON  9
#define FPGA_PUSH_SWITCH_DEVICE "/dev/fpga_push_switch"

//MATRIX BUTTON
#define FGPA_DOT_DEVICE "/dev/fpga_dot"

//FND
#define FND_DEVICE "/dev/fpga_fnd"
#define MAX_DIGIT 4

//LED
#define LED_DEVICE  "/dev/fpga_led"

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
    void update();
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

    // PUSH BUTTON
    int push_i;
    int push_dev;
    int push_buff_size;
    unsigned char push_sw_buff[MAX_BUTTON];
    QMessageBox deviceBox;


    //MATRIX INSIDE
    int matrix_i;
    int matrix_dev;
    int matrix_str_size;
    int matrix_set_num;


    //FND INSIDE
    int fnd_i;
    int fnd_dev;
    unsigned char fnd_data[4];
    unsigned char retval;
    unsigned fnd_str_size;


    //LED INSIDE
    int led_dev;
    unsigned char led_data;
    unsigned char led_retval;

    //gaussian clicked
    bool blurSetting;
};

#endif // MAINWINDOW_H
