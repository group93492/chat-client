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
#include "ChannelThemeDialog.h"

class UserListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit UserListWidget(QWidget *parent = 0, QStringList *list = NULL);
private:
    QStringList *m_list;
protected:
    virtual void contextMenuEvent(QContextMenuEvent * e);
private slots:
    void sendInfoSignal();
    void sendPMSignal();
signals:
    void onUserInformationClicked(QString nick);
    void onPrivateMessageClicked(QString nick);
};

class ThemeLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ThemeLabel(QWidget *parent = 0);
private:
    ChannelThemeDialog m_dialog;
public slots:
    void changeTheme(QString theme);
protected:
    virtual void mousePressEvent(QMouseEvent *ev);
};

class GeneralChatWidget : public QWidget
{
    Q_OBJECT
private:
    chatTextBrowser *m_textBrowser;
    ThemeLabel *m_theme;
    QLabel *m_label;
    UserListWidget *m_userList;
    QStringList *m_users;
private slots:
    void replyUserInformationClicked(QString nick);
    void replyPrivateMessageClicked(QString nick);
public:
    explicit GeneralChatWidget(QWidget *parent = 0);
signals:
    void onUserInformationClicked(QString nick);
    void onPrivateMessageClicked(QString nick);
public slots:
    void setUserList(QStringList list);
    void setUserStatus(QString nick, QString status);
    void appendMessage(QString nick, QString msg);
    void appendSystemMessage(QString msg);
    void setChannelTheme(QString theme);
};

class ChatTabWidget : public QTabWidget
{
    Q_OBJECT
private:
    QMap<QString, GeneralChatWidget *> m_channels;
    QMap<QString, GeneralChatWidget *> m_privateChannels;
    QIcon m_channelIcon;
    QIcon m_PMIcon;
    QIcon m_newMsgIcon;
    QIcon m_newMsgPMIcon;
private slots:
    void tabCloseResult(int index);
    void currentChangedHander(int index);
    void replyUserInformationClicked(QString nick);
    void replyPrivateMessageClicked(QString nick);
public:
    explicit ChatTabWidget(QWidget *parent = 0);
public slots:
    void joinChannel(QString name);
    void joinPrivateChannel(QString name);
    void appendMessage(QString channelname, QString nick, QString message);
    void appendSystemMessage(QString channelname, QString message);
    void setUserList(QString channelname, QStringList list);
    void changeTheme(QString channelname, QString theme);
    void setUserStatus(QString nick, QString status);
signals:
    void leaveChannel(QString channelname);
    void userInfoRequest(QString nick);
    void themeChanged(QString channelname, QString theme);
    void onUserInformationClicked(QString nick);
    void onPrivateMessageClicked(QString nick);
};

#endif // GENERALCHATWIDGET_H
