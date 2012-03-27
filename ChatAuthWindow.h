#ifndef CHATAUTHWINDOW_H
#define CHATAUTHWINDOW_H

#include <QDialog>

namespace Ui {
    class ChatAuthWindow;
}

class ChatAuthWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChatAuthWindow(QWidget *parent = 0);
    ~ChatAuthWindow();

private:
    Ui::ChatAuthWindow *ui;
};

#endif // CHATAUTHWINDOW_H
