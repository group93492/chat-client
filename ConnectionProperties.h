#ifndef CONNECTIONPROPERTIES_H
#define CONNECTIONPROPERTIES_H

#include <QDialog>

namespace Ui {
    class ConnectionProperties;
}

class ConnectionProperties : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionProperties(QWidget *parent = 0);
    ~ConnectionProperties();

private:
    Ui::ConnectionProperties *ui;
};

#endif // CONNECTIONPROPERTIES_H
