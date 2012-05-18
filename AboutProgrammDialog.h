#ifndef ABOUTPROGRAMMDIALOG_H
#define ABOUTPROGRAMMDIALOG_H

#include <QDialog>

namespace Ui {
class AboutProgrammDialog;
}

class AboutProgrammDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AboutProgrammDialog(QWidget *parent = 0);
    ~AboutProgrammDialog();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::AboutProgrammDialog *ui;
};

#endif // ABOUTPROGRAMMDIALOG_H
