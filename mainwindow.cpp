#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "savedialog.h"
#include "printprogress.h"
#include "fpga_dot_font.h"
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

#include <QPixmap>

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    showMaximized();

    ui->fileName->setText(this->fileControl.get_filename());
    if(this->fileControl.get_size() != 0)
    {

        QPixmap pix("pictures/"+this->fileControl.get_filename());
        ui->picLabel->setPixmap(pix);
    }
    this->captured = false;
    this->revived = false;
    this->currentFileName = "pictures/"+this->fileControl.get_filename_native();
    this->blurSetting = false;

    ui->gageName->hide();
    ui->gageLevel->hide();
    ui->gageSlider->hide();

    // set timer
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);

    //push device
    this->push_dev = open(FPGA_PUSH_SWITCH_DEVICE, O_RDWR);
    if(this->push_dev < 0)
    {
        this->deviceBox.setText("Device Open Error => push button");
        this->deviceBox.setInformativeText("Check your push button");
        this->deviceBox.exec();
        exit(1);
    }

    this->push_buff_size = sizeof(push_sw_buff);

    //matrix device
    this->matrix_dev = open(FGPA_DOT_DEVICE, O_WRONLY);
    if( this->matrix_dev < 0)
    {
        this->deviceBox.setText("Device Open Error -> matrix");
        this->deviceBox.setInformativeText("Check your matrix");
        this->deviceBox.exec();
        exit(1);
    }
    this->matrix_str_size = sizeof(fpga_number[0]);

    //fnd device
    this->fnd_dev = open(FND_DEVICE, O_RDWR);
    this->fnd_str_size = (sizeof(unsigned char)*4);

    if( this->fnd_dev < 0)
    {
        this->deviceBox.setText("Device Open Error -> matrix");
        this->deviceBox.setInformativeText("Check your matrix");
        this->deviceBox.exec();
        exit(1);
    }

    //led device
    this->led_dev = open(LED_DEVICE, O_RDWR);
    if( this->led_dev < 0)
    {
        this->deviceBox.setText("Device Open Error -> led");
        this->deviceBox.setInformativeText("Check your led");
        this->deviceBox.exec();
        exit(1);
    }

}
MainWindow::~MainWindow()
{
    //close device
     //close(this->push_dev);
    delete ui;
}

// EXIT BTN
void MainWindow::on_exitBtn_clicked()
{
    //close device for exit
    exit(0);
}

//capture button
void MainWindow::on_captureBtn_clicked()
{

    pid_t cam = 0;
    cam = fork();
    if(cam == 0)
    {
        execlp("raspistill", "raspistill", "-o", "tmp.jpg",
              "-w", "560", "-h", "280", NULL);
        exit(0);
    }
    else
    {
        for(int j = 0; j < 5; j++)
        {
            //matrix change
            write(this->matrix_dev, fpga_number[5-j], this->matrix_str_size);
            this_thread::sleep_for(chrono::seconds(1));
        }
        waitpid(cam, 0, 0);
        QPixmap pix("tmp.jpg");
        this->ui->picLabel->setPixmap(pix);
        this->captured = true; //set to capture mode

        //Change Label
        this->ui->fileName->setText("CAPTURED FILE!!");
        this->currentCaptured = imread("tmp.jpg", IMREAD_COLOR);
        this->currentFileName = "tmp.jpg";

        //copy
        imwrite("tmp_changed.jpg", this->currentCaptured);
        this->currentFileName = "tmp_changed.jpg";
    }
}

//prev button click
/*
void MainWindow::on_prevBtn_clicked()
{
    if(!captured && !revived) {
        if(this->fileControl.prev())
        {
            this->ui->fileName->setText(this->fileControl.get_filename());
            QPixmap pix("pictures/"+this->fileControl.get_filename());
            this->ui->picLabel->setPixmap(pix);
        }
    }
}

//next button click
void MainWindow::on_nextBtn_clicked()
{
    if(!captured && !revived) {
        if(this->fileControl.next())
        {
            this->ui->fileName->setText(this->fileControl.get_filename());
            QPixmap pix("pictures/"+this->fileControl.get_filename());
            this->ui->picLabel->setPixmap(pix);
        }
    }
}
*/
void MainWindow::on_saveBtn_clicked()
{
    QMessageBox saveBox;
    SaveDialog saveDialog(&this->fileControl);
    if(this->captured)
    {
        //goto save message
        saveBox.setText("SAVE RESULT");
        saveBox.setInformativeText("Do you want to save file?");
        saveBox.setStandardButtons(QMessageBox::Save |
                                   QMessageBox::Discard |
                                   QMessageBox::Cancel);
        auto save_result = saveBox.exec();

        switch(save_result)
        {
            case QMessageBox::Save:
                saveDialog.setModal(true);
                saveDialog.exec();

            break;
            default:
            break;
        }

        //insert filename data

        //reload file to new filename

        //set interface by ner file name
    }
    else if(this->revived)
    {
        saveBox.setText("SAVE RESULT");
        saveBox.setInformativeText("Do you want to save or change file2?");
        saveBox.setStandardButtons(QMessageBox::Save |
                                   QMessageBox::Discard |
                                   QMessageBox::Cancel);
        auto save_result = saveBox.exec();
    }
}

void MainWindow::on_printBtn_clicked()
{
    PrintProgress progress;
    progress.setModal(true);
    progress.exec();
}

//to black
void MainWindow::on_blackBtn_clicked()
{
    //none capture
    if(!this->captured)
    {
        QMessageBox warningBox;
        warningBox.setText("Warning");
        warningBox.setInformativeText("Capture your Picture First!");
        warningBox.setStandardButtons(QMessageBox::Ok);
        warningBox.exec();
    }
    else //captured
    {
        this->changedCaptured = imread("tmp_changed.jpg", IMREAD_GRAYSCALE);
        imwrite("tmp_changed.jpg", this->changedCaptured);

        QPixmap pix("tmp_changed.jpg");
        this->ui->picLabel->setPixmap(pix);
    }
}

//reset
void MainWindow::on_returnBtn_clicked()
{

    //copy
    imwrite("tmp_changed.jpg", this->currentCaptured);
    QPixmap pix("tmp_changed.jpg");
    this->ui->picLabel->setPixmap(pix);
    this->currentFileName = "tmp_changed.jpg";

    //copy

    //reset gaussian gage
    this->ui->gageLevel->setText(QString::number(0));
    this->ui->gageName->hide();
    this->ui->gageLevel->hide();
    this->ui->gageSlider->hide();
    this->ui->gageSlider->setValue(0);
}

void MainWindow::on_blurBtn_clicked()
{
    //none capture
    if(!this->captured)
    {
        QMessageBox warningBox;
        warningBox.setText("Warning");
        warningBox.setInformativeText("Capture your Picture First!");
        warningBox.setStandardButtons(QMessageBox::Ok);
        warningBox.exec();
    }
    else
    {
        //none_click->click
        if(!this->blurSetting )
        {
            this->blurSetting = true;
            ui->gageName->show();
            ui->gageLevel->show();
            ui->gageSlider->show();
            gaussiantmp = imread(this->currentFileName);
        }
        //click->non_click
        else
        {
            this->blurSetting = false;
            ui->gageName->hide();
            ui->gageLevel->hide();
            ui->gageSlider->hide();
        }
    }

}

void MainWindow::on_gageSlider_valueChanged(int value)
{
    Mat filterImg;
    GaussianBlur(gaussiantmp, filterImg, Size(3,3), 0);
    for(int i = 0; i < value; i++)
        GaussianBlur(filterImg, filterImg, Size(3,3), 0);

    imwrite(this->currentFileName, filterImg);
    QPixmap pix(QString::fromStdString(this->currentFileName));
    this->ui->picLabel->setPixmap(pix);
    ui->gageLevel->setText(QString::number(value));
    this->fnd_data[0] = 0;
    this->fnd_data[1] = 0;
    this->fnd_data[2] = (unsigned char)(value/10);
    this->fnd_data[3] = (unsigned char)(value%10);

    write(this->fnd_dev, &this->fnd_data, 4);
}

void MainWindow::on_sharpenBtn_clicked()
{
    //none capture
    if(!this->captured)
    {
        QMessageBox warningBox;
        warningBox.setText("Warning");
        warningBox.setInformativeText("Capture your Picture First!");
        warningBox.setStandardButtons(QMessageBox::Ok);
        warningBox.exec();
    }
    else
    {
        auto img = imread("tmp_changed.jpg", CV_LOAD_IMAGE_COLOR);

        vector<Mat> channels;
        Mat img_hist_equalized;
        cvtColor(img, img_hist_equalized, CV_BGR2YCrCb);
        cv::split(img_hist_equalized, channels);
        equalizeHist(channels[0], channels[0]);
        merge(channels, img_hist_equalized);
        cvtColor(img_hist_equalized, img_hist_equalized,CV_YCrCb2BGR);

        imwrite("tmp_changed.jpg", img_hist_equalized);
        QPixmap pix("tmp_changed.jpg");
        this->ui->picLabel->setPixmap(pix);
    }

}

void MainWindow::on_invertBtn_clicked()
{
    //none capture
    if(!this->captured)
    {
        QMessageBox warningBox;
        warningBox.setText("Warning");
        warningBox.setInformativeText("Capture your Picture First!");
        warningBox.setStandardButtons(QMessageBox::Ok);
        warningBox.exec();
    }
    else
    {
        auto result = imread("tmp_changed.jpg", IMREAD_COLOR);
        bitwise_not(result, result);
        imwrite("tmp_changed.jpg", result);
        QPixmap pix("tmp_changed.jpg");
        this->ui->picLabel->setPixmap(pix);
    }
}

void MainWindow::on_detectionBtn_clicked()
{
    //none capture
    if(!this->captured)
    {
        QMessageBox warningBox;
        warningBox.setText("Warning");
        warningBox.setInformativeText("Capture your Picture First!");
        warningBox.setStandardButtons(QMessageBox::Ok);
        warningBox.exec();
    }
    else
    {
        auto result = imread("tmp_changed.jpg", IMREAD_COLOR);
        Canny(result, result, 50, 200);
        imwrite("tmp_changed.jpg", result);
        QPixmap pix("tmp_changed.jpg");
        this->ui->picLabel->setPixmap(pix);
    }
}


void MainWindow::update()
{
    read(push_dev, &push_sw_buff, this->push_buff_size);
    for(this->push_i = 0; this->push_i<MAX_BUTTON; this->push_i++)
    {
        if(this->push_sw_buff[this->push_i] == 1)
        {
            switch(this->push_i)
            {
                case 0: //capture
                {
                    unsigned char data = 0b00000001;
                    write(this->led_dev, &data, 1);
                    pid_t cam = 0;
                    cam = fork();
                    if(cam == 0)
                    {
                        execlp("raspistill", "raspistill", "-o", "tmp.jpg",
                          "-w", "560", "-h", "280", NULL);
                        exit(0);
                    }
                    else
                    {
                        for(int j = 0; j < 5; j++)
                        {
                            //matrix change
                            write(this->matrix_dev, fpga_number[5-j], this->matrix_str_size);
                            this_thread::sleep_for(chrono::seconds(1));
                        }

                        waitpid(cam, 0, 0);
                        QPixmap pix("tmp.jpg");
                        this->ui->picLabel->setPixmap(pix);
                        this->captured = true; //set to capture mode

                        //Change Label
                        this->ui->fileName->setText("CAPTURED FILE!!");
                        this->currentCaptured = imread("tmp.jpg", IMREAD_COLOR);
                        this->currentFileName = "tmp.jpg";

                        //copy
                        imwrite("tmp_changed.jpg", this->currentCaptured);
                        this->currentFileName = "tmp_changed.jpg";
                    }
                }
                break;
                case 1:
                {
                    unsigned char data = 0b00000010;
                    write(this->led_dev, &data, 1);
                    //none capture
                    if(!this->captured)
                    {
                        QMessageBox warningBox;
                        warningBox.setText("Warning");
                        warningBox.setInformativeText("Capture your Picture First!");
                        warningBox.setStandardButtons(QMessageBox::Ok);
                        warningBox.exec();
                    }
                    else //captured
                    {
                        this->changedCaptured = imread("tmp_changed.jpg", IMREAD_GRAYSCALE);
                        imwrite("tmp_changed.jpg", this->changedCaptured);

                        QPixmap pix("tmp_changed.jpg");
                        this->ui->picLabel->setPixmap(pix);
                    }
                }
                break;
                case 2:
                {
                    unsigned char data = 0b00000100;
                    write(this->led_dev, &data, 1);
                    //none capture
                    if(!this->captured)
                    {
                        QMessageBox warningBox;
                        warningBox.setText("Warning");
                        warningBox.setInformativeText("Capture your Picture First!");
                        warningBox.setStandardButtons(QMessageBox::Ok);
                        warningBox.exec();
                    }
                    else
                    {
                        //none_click->click
                        if(!this->blurSetting )
                        {
                            this->blurSetting = true;
                            ui->gageName->show();
                            ui->gageLevel->show();
                            ui->gageSlider->show();
                            gaussiantmp = imread(this->currentFileName);
                        }
                        //click->non_click
                        else
                        {
                            this->blurSetting = false;
                            ui->gageName->hide();
                            ui->gageLevel->hide();
                            ui->gageSlider->hide();
                        }
                    }
                }
                break;
                case 3:
                {
                    unsigned char data = 0b00001000;
                    write(this->led_dev, &data, 1);
                    //none capture
                    if(!this->captured)
                    {
                        QMessageBox warningBox;
                        warningBox.setText("Warning");
                        warningBox.setInformativeText("Capture your Picture First!");
                        warningBox.setStandardButtons(QMessageBox::Ok);
                        warningBox.exec();
                    }
                    else
                    {
                        auto img = imread("tmp_changed.jpg", CV_LOAD_IMAGE_COLOR);

                        vector<Mat> channels;
                        Mat img_hist_equalized;
                        cvtColor(img, img_hist_equalized, CV_BGR2YCrCb);
                        cv::split(img_hist_equalized, channels);
                        equalizeHist(channels[0], channels[0]);
                        merge(channels, img_hist_equalized);
                        cvtColor(img_hist_equalized, img_hist_equalized,CV_YCrCb2BGR);

                        imwrite("tmp_changed.jpg", img_hist_equalized);
                        QPixmap pix("tmp_changed.jpg");
                        this->ui->picLabel->setPixmap(pix);
                    }
                }
                break;
                case 4:
                {
                    unsigned char data = 0b00010000;
                    write(this->led_dev, &data, 1);
                    //none capture
                    if(!this->captured)
                    {
                        QMessageBox warningBox;
                        warningBox.setText("Warning");
                        warningBox.setInformativeText("Capture your Picture First!");
                        warningBox.setStandardButtons(QMessageBox::Ok);
                        warningBox.exec();
                    }
                    else
                    {
                        auto result = imread("tmp_changed.jpg", IMREAD_COLOR);
                        bitwise_not(result, result);
                        imwrite("tmp_changed.jpg", result);
                        QPixmap pix("tmp_changed.jpg");
                        this->ui->picLabel->setPixmap(pix);
                    }
                }
                break;
                case 5:
                {
                    unsigned char data = 0b00100000;
                    write(this->led_dev, &data, 1);
                    //none capture
                    if(!this->captured)
                    {
                        QMessageBox warningBox;
                        warningBox.setText("Warning");
                        warningBox.setInformativeText("Capture your Picture First!");
                        warningBox.setStandardButtons(QMessageBox::Ok);
                        warningBox.exec();
                    }
                    else
                    {
                        auto result = imread("tmp_changed.jpg", IMREAD_COLOR);
                        Canny(result, result, 50, 200);
                        imwrite("tmp_changed.jpg", result);
                        QPixmap pix("tmp_changed.jpg");
                        this->ui->picLabel->setPixmap(pix);
                    }
                }
                break;
                case 6:
                {
                    unsigned char data = 0b01000000;
                    write(this->led_dev, &data, 1);
                    //copy
                    imwrite("tmp_changed.jpg", this->currentCaptured);
                    QPixmap pix("tmp_changed.jpg");
                    this->ui->picLabel->setPixmap(pix);
                    this->currentFileName = "tmp_changed.jpg";

                    //copy

                    //reset gaussian gage
                    this->ui->gageLevel->setText(QString::number(0));
                    this->ui->gageName->hide();
                    this->ui->gageLevel->hide();
                    this->ui->gageSlider->hide();
                    this->ui->gageSlider->setValue(0);
                }
                break;
                case 7:
                break;

            }
        }
    }
}
