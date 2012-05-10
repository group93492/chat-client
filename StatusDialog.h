#ifndef STATUSDIALOG_H
#define STATUSDIALOG_H

#include <QDialog>

namespace Ui {
class StatusDialog;
}

class StatusDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StatusDialog(QWidget *parent = 0);
    ~StatusDialog();

private:
    Ui::StatusDialog *ui;

signals:
    void statusChanged(QString status);
private slots:
    void on_OKpushButton_clicked();
    void on_CancelPushButton_clicked();
};

#endif // STATUSDIALOG_H
