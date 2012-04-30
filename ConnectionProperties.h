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
signals:
    void properties(QString host, quint32 port);
};

#endif // CONNECTIONPROPERTIES_H
