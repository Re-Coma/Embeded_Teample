#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <opencv2/opencv.hpp>

#include <string>

#include <stdlib.h>
#include <unistd.h>

#include "libs/filecontrol.hpp"

namespace Ui {
class SaveDialog;
}

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDialog(FileControl* fileControl, QWidget *parent = 0);
    ~SaveDialog();

private slots:
    void on_saveBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::SaveDialog *ui;
    FileControl* fileControl;
};

#endif // SAVEDIALOG_H
