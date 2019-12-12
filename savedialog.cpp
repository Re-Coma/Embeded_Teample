#include "savedialog.h"
#include "ui_savedialog.h"

SaveDialog::SaveDialog(FileControl* fileControl, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::SaveDialog)
{
    ui->setupUi(this);
    this->fileControl = fileControl;
}

SaveDialog::~SaveDialog()
{
    delete ui;
}

void SaveDialog::on_saveBtn_clicked()
{
    QString str = ui->nameEdit->toPlainText();
    if(str.length() == 0)
    {
        QMessageBox warningBox;
        warningBox.setText("FILE NAME EMPTY");
        warningBox.setInformativeText("input new file name");
        warningBox.setStandardButtons(QMessageBox::Ok);
        warningBox.exec();
    }
    else
    {
        string filename = str.toStdString();

    }
}

void SaveDialog::on_cancelBtn_clicked()
{

}
