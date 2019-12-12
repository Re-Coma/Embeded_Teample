#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "savedialog.h"
#include "printprogress.h"
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <iostream>

#include <QPixmap>

using namespace std;

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
}
MainWindow::~MainWindow()
{
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
        waitpid(cam, 0, 0);
        QPixmap pix("tmp.jpg");
        this->ui->picLabel->setPixmap(pix);
        this->captured = true; //set to capture mode

        //Change Label
        this->ui->fileName->setText("CAPTURED FILE!!");
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
