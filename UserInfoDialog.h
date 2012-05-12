#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include <QDialog>

namespace Ui {
class UserInfoDialog;
}

class UserInfoDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UserInfoDialog(QWidget *parent = 0);
    ~UserInfoDialog();
public slots:
    void showInfo(QString username, QString info);

private slots:
    void on_okButton_clicked();

private:
    Ui::UserInfoDialog *ui;
};

#endif // USERINFODIALOG_H
