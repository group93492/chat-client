#ifndef CHATAUTHWINDOW_H
#define CHATAUTHWINDOW_H

#include <QDialog>
#include "RegisterBot.h"

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
    RegisterBot *m_bot;
    QString m_host;
    quint32 m_port;
signals:
    void authorized(QString name, QString password, QTcpSocket *socket);
private slots:
    void setConnectionProperty(QString host, quint32 port);
    void on_registerButton_clicked();
    void registrationHandler(bool registrationResult, QString &denialReason);
    void on_propertiesButton_clicked();
    void replyClientAuthorized(QTcpSocket *socket);
    void on_connectButton_clicked();
};

#endif // CHATAUTHWINDOW_H
