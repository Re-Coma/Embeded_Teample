#ifndef PRINTPROGRESS_H
#define PRINTPROGRESS_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class PrintProgress;
}

class PrintProgress : public QDialog
{
    Q_OBJECT

public:
    explicit PrintProgress(QWidget *parent = 0);
    ~PrintProgress();

private slots:
    void on_okBtn_clicked();
    void update();

private:
    Ui::PrintProgress *ui;
    int i;
};

#endif // PRINTPROGRESS_H
