#ifndef GENERALCHATWIDGET_H
#define GENERALCHATWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include "smileswidgets.h"

class GeneralChatWidget : public QWidget
{
    Q_OBJECT
private:
    chatTextBrowser *m_textBrowser;
    QLabel *m_theme;
    QLabel *m_label;
    QListWidget *m_userList;
    QStringList m_users;
public:
    explicit GeneralChatWidget(QWidget *parent = 0);
signals:
    
public slots:
    void setUserList(QStringList list);
    void setUserStatus(QString nick, QString status);
    void appendMessage(QString nick, QString msg);
};

#endif // GENERALCHATWIDGET_H
