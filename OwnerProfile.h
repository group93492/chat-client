#ifndef OWNERPROFILE_H
#define OWNERPROFILE_H

#include <QDialog>

namespace Ui {
class OwnerProfile;
}

class OwnerProfile : public QDialog
{
    Q_OBJECT
    
public:
    explicit OwnerProfile(QWidget *parent = 0);
    ~OwnerProfile();
    
signals:
    void changePassword(QString oldpass, QString newpass);
    void changeInfo(QString username, QString info);

private slots:
    void on_closeButton_clicked();
    void on_saveButton_clicked();
    void on_changeButton_clicked();

public slots:
    void setResult(QString text);
    void setInfo(QString username, QString info);

private:
    Ui::OwnerProfile *ui;
};

#endif // OWNERPROFILE_H
