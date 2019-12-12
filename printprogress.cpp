#include "printprogress.h"
#include "ui_printprogress.h"

#include <chrono>
#include <thread>
#include <string>

#include <stdlib.h>

using namespace std;

PrintProgress::PrintProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrintProgress)
{
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(update()));
    ui->setupUi(this);
    ui->okBtn->hide();

    this->i = 1;
    timer->start(50);
}

PrintProgress::~PrintProgress()
{
    delete ui;
}

void PrintProgress::on_okBtn_clicked()
{
    this->close();
}

void PrintProgress::update()
{
    if(this->i < 100)
    {
        this->ui->progressBar->setValue(this->i);
        this->i++;
    }
    else if(this->i == 100)
    {
        this->ui->progressBar->setValue(this->i);
        ui->label->setText("Print Complete!");
        ui->okBtn->show();
        this->i++;
    }
}
