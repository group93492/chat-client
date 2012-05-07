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
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::UserInfoDialog *ui;
};

#endif // USERINFODIALOG_H
