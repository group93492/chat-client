#ifndef CREATECHANNELDIALOG_H
#define CREATECHANNELDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class CreateChannelDialog;
}

class CreateChannelDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CreateChannelDialog(QWidget *parent = 0);
    ~CreateChannelDialog();
    
private slots:
    void on_cancelButton_clicked();

    void on_createButton_clicked();
signals:
    void createChannel(QString, QString, QString);
private:
    Ui::CreateChannelDialog *ui;
};

#endif // CREATECHANNELDIALOG_H
