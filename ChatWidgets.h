#ifndef GENERALCHATWIDGET_H
#define GENERALCHATWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
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
    void setTheme(QString theme);
private:
    ChannelThemeDialog m_dialog;
private slots:
    void changeTheme(QString theme);
protected:
    virtual void mousePressEvent(QMouseEvent *ev);
signals:
    void themeChanged(QString theme);
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
    QIcon m_statusOnline;
    QIcon m_statusAway;
    QIcon m_statusDTD;
private slots:
    void replyUserInformationClicked(QString nick);
    void replyPrivateMessageClicked(QString nick);
    void replyNickClicked(QString nick);
    void replyLastMessage(QString message);
    void replyThemeChanged(QString theme);
public:
    explicit GeneralChatWidget(QWidget *parent = 0, QMap<QString, QString> *smilesMap = NULL, QString nick = "");
signals:
    void onUserInformationClicked(QString nick);
    void onPrivateMessageClicked(QString nick);
    void onNickClicked(QString nick);
    void lastMessage(QString message);
    void themeChanged(QString theme);
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
    QMap<QString, QString> *m_smilesMap;
    QString m_ownerNick;
    QIcon m_channelIcon;
    QIcon m_PMIcon;
    QIcon m_newMsgIcon;
    QIcon m_newMsgPMIcon;
private slots:
    void tabCloseResult(int index);
    void currentChangedHandler(int index);
    void replyUserInformationClicked(QString nick);
    void replyPrivateMessageClicked(QString nick);
    void replyNickClicked(QString nick);
    void replyLastMessage(QString message);
    void replyThemeChanged(QString theme);
public:
    explicit ChatTabWidget(QWidget *parent = 0);
    explicit ChatTabWidget(QWidget *parent = 0, QMap<QString, QString> *smilesMap = NULL);
    QString currentChannel();
    void setOwnerNick(QString nick);
public slots:
    void appendMessage(QString channelname, QString nick, QString message);
    void appendSystemMessage(QString channelname, QString message);
    void changeTheme(QString channelname, QString theme);
    void setUserList(QString channelname, QStringList list);
    void setUserStatus(QString nick, QString status);
    void setChannelsList(QMap<QString, QString> list);
    void joinChannel(QString name);
    void joinPrivateChannel(QString name);
signals:
    void leaveChannel(QString channelname);
    void userInfoRequest(QString nick);
    void themeChanged(QString channelname, QString theme);
    void onUserInformationClicked(QString nick);
    void onPrivateMessageClicked(QString nick);
    void onNickClicked(QString nick);
    void lastMessage(QString message);
    void showOwnerInfo();
};

#endif // GENERALCHATWIDGET_H
