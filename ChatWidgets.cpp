#include "ChatWidgets.h"
#include <QDebug>

GeneralChatWidget::GeneralChatWidget(QWidget *parent, QMap<QString, QString> *smilesMap, QString nick) :
    QWidget(parent)
{
    m_users = new QStringList;
    m_theme = new ThemeLabel(this);
    m_textBrowser = new chatTextBrowser(this, smilesMap);
    m_userList = new UserListWidget(this, m_users);
    m_label = new QLabel("Users in channel: " + QString::number(m_userList->count()), this);
    m_statusOnline.addFile("status_availible.png");
    m_statusAway.addFile("status_away.png");
    m_statusDTD.addFile("status_dnd.png");
    m_theme->setFrameShape(QFrame::StyledPanel);
    m_theme->setFrameShadow(QFrame::Raised);
    m_theme->setAlignment(Qt::AlignHCenter);
    m_textBrowser->setOwnerNick(nick);
    QVBoxLayout *generalLayout = new QVBoxLayout();
    QHBoxLayout *layoutOne = new QHBoxLayout();
    QHBoxLayout *layoutTwo = new QHBoxLayout();
    layoutOne->addWidget(m_theme, 3);
    layoutOne->addWidget(m_label, 1);
    layoutTwo->addWidget(m_textBrowser, 3);
    layoutTwo->addWidget(m_userList, 1);
    generalLayout->addLayout(layoutOne);
    generalLayout->addLayout(layoutTwo, 10);
    this->setLayout(generalLayout);
    connect(m_userList, SIGNAL(onUserInformationClicked(QString)), this, SLOT(replyUserInformationClicked(QString)));
    connect(m_userList, SIGNAL(onPrivateMessageClicked(QString)), this, SLOT(replyPrivateMessageClicked(QString)));
    connect(m_textBrowser, SIGNAL(onNickClicked(QString)), this, SLOT(replyNickClicked(QString)));
    connect(m_textBrowser, SIGNAL(lastMessage(QString)), this, SLOT(replyLastMessage(QString)));
    connect(m_theme, SIGNAL(themeChanged(QString)), this, SLOT(replyThemeChanged(QString)));
}

void GeneralChatWidget::setUserList(QStringList list)
{
    list.sort();
    m_users->clear();
    m_users->append(list);
    m_userList->clear();
    m_userList->addItems(list);
    m_label->setText("Users in channel: " + QString::number(m_users->count()));
}

void GeneralChatWidget::setUserStatus(QString nick, QString status)
{
    if(m_users->contains(nick))
    {
        QListWidgetItem *item = m_userList->takeItem(m_users->indexOf(nick));
        if(status != "Online" && status != "")
        {
            item->setText(nick + "[" + status + "]");
            if(status == "Away")
                item->setIcon(m_statusAway);
            else
                if(status == "Don't disturb")
                    item->setIcon(m_statusDTD);
                else
                    item->setIcon(m_statusOnline);
        }
        else
        {
            item->setText(nick);
            item->setIcon(m_statusOnline);
        }
        m_userList->addItem(item);
        m_userList->sortItems();
    }
}

void GeneralChatWidget::appendMessage(QString nick, QString msg)
{
    m_textBrowser->appendMessage(nick, msg);
}

void GeneralChatWidget::appendSystemMessage(QString msg)
{
    m_textBrowser->appendSystemMessage(msg);
}

void GeneralChatWidget::setChannelTheme(QString theme)
{
    m_theme->setTheme(theme);
}

void GeneralChatWidget::replyUserInformationClicked(QString nick)
{
    emit onUserInformationClicked(nick);
}

void GeneralChatWidget::replyPrivateMessageClicked(QString nick)
{
    emit onPrivateMessageClicked(nick);
}

void GeneralChatWidget::replyNickClicked(QString nick)
{
    emit onNickClicked(nick);
}

void GeneralChatWidget::replyLastMessage(QString message)
{
    emit lastMessage(message);
}

void GeneralChatWidget::replyThemeChanged(QString theme)
{
    emit themeChanged(theme);
}

UserListWidget::UserListWidget(QWidget *parent, QStringList *list) :
    QListWidget(parent)
{
    m_list = list;
}

void UserListWidget::contextMenuEvent(QContextMenuEvent *e)
{
    if(this->itemAt(e->pos()) != NULL)
    {
        QMenu *menu = new QMenu();
        //menu->addAction("Send private message", this, SLOT(sendPMSignal()));
        menu->addAction("Profile", this, SLOT(sendInfoSignal()));
        menu->exec(e->globalPos());
        delete menu;
    }
}

void UserListWidget::sendInfoSignal()
{
    QString nick = m_list->value(this->row(this->selectedItems().first()));
    emit onUserInformationClicked(nick);
}

void UserListWidget::sendPMSignal()
{
    QString nick = m_list->value(this->row(this->selectedItems().first()));
    emit onPrivateMessageClicked(nick);
}

ThemeLabel::ThemeLabel(QWidget *parent) :
    QLabel(parent)
{
    m_dialog.setModal(true);
    connect(&m_dialog, SIGNAL(onOkButtonClicked(QString)), this, SLOT(changeTheme(QString)));
}

void ThemeLabel::changeTheme(QString theme)
{
    if(!m_dialog.isHidden())
        m_dialog.hide();
    if(this->text() != theme)
        emit themeChanged(theme);
}

void ThemeLabel::setTheme(QString theme)
{
    setText(theme);
}

void ThemeLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        m_dialog.setTheme(this->text());
        m_dialog.show();
    }
}

ChatTabWidget::ChatTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    setTabsClosable(true);
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseResult(int)));
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentChangedHandler(int)));
}

ChatTabWidget::ChatTabWidget(QWidget *parent, QMap<QString, QString> *smilesMap) :
    QTabWidget(parent)
{
    m_smilesMap = smilesMap;
    setTabsClosable(true);
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseResult(int)));
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentChangedHandler(int)));
    m_channelIcon.addFile("common_channel.png");
    m_PMIcon.addFile("private_channel.png");
    m_newMsgIcon.addFile("common_channel_new_message.png");
    m_newMsgPMIcon.addFile("private_channel_new_message.png");
}

QString ChatTabWidget::currentChannel()
{
    return tabText(currentIndex());
}

void ChatTabWidget::setOwnerNick(QString nick)
{
    m_ownerNick = nick;
}

void ChatTabWidget::joinChannel(QString name)
{
    GeneralChatWidget *channel = new GeneralChatWidget(this, m_smilesMap, m_ownerNick);
    connect(channel, SIGNAL(onPrivateMessageClicked(QString)), this, SLOT(replyPrivateMessageClicked(QString)));
    connect(channel, SIGNAL(onUserInformationClicked(QString)), this, SLOT(replyUserInformationClicked(QString)));
    connect(channel, SIGNAL(onNickClicked(QString)), this, SLOT(replyNickClicked(QString)));
    connect(channel, SIGNAL(lastMessage(QString)), this, SLOT(replyLastMessage(QString)));
    connect(channel, SIGNAL(themeChanged(QString)), this, SLOT(replyThemeChanged(QString)));
    m_channels.insert(name, channel);
    addTab(channel, m_channelIcon, name);
}

void ChatTabWidget::joinPrivateChannel(QString name)
{
    GeneralChatWidget *channel = new GeneralChatWidget(this, m_smilesMap);
    connect(channel, SIGNAL(onPrivateMessageClicked(QString)), this, SLOT(replyPrivateMessageClicked(QString)));
    connect(channel, SIGNAL(onUserInformationClicked(QString)), this, SLOT(replyUserInformationClicked(QString)));
    connect(channel, SIGNAL(onNickClicked(QString)), this, SLOT(replyNickClicked(QString)));
    connect(channel, SIGNAL(lastMessage(QString)), this, SLOT(replyLastMessage(QString)));
    connect(channel, SIGNAL(themeChanged(QString)), this, SLOT(replyThemeChanged(QString)));
    m_privateChannels.insert(name, channel);
    addTab(channel, m_PMIcon, name);
}

void ChatTabWidget::appendMessage(QString channelname, QString nick, QString message)
{
    if(m_channels.contains(channelname))
    {
        m_channels.value(channelname)->appendMessage(nick, message);
        if(tabText(currentIndex()) != channelname)
            setTabIcon(indexOf(m_channels.value(channelname)), m_newMsgIcon);
    }
    else
    {
        m_privateChannels.value(channelname)->appendMessage(nick, message);
        if(tabText(currentIndex()) != channelname)
            setTabIcon(indexOf(m_privateChannels.value(channelname)), m_newMsgPMIcon);
    }
}

void ChatTabWidget::appendSystemMessage(QString channelname, QString message)
{
    if(m_channels.contains(channelname))
        m_channels.value(channelname)->appendSystemMessage(message);
    else
        m_privateChannels.value(channelname)->appendSystemMessage(message);
}

void ChatTabWidget::setUserList(QString channelname, QStringList list)
{
    if(m_channels.contains(channelname))
        m_channels.value(channelname)->setUserList(list);
    else
        m_privateChannels.value(channelname)->setUserList(list);
}

void ChatTabWidget::changeTheme(QString channelname, QString theme)
{
    if(m_channels.contains(channelname))
        m_channels.value(channelname)->setChannelTheme(theme);
    else
        m_privateChannels.value(channelname)->setChannelTheme(theme);
}

void ChatTabWidget::setUserStatus(QString nick, QString status)
{
    QMap<QString, GeneralChatWidget*>::iterator i = m_channels.begin();
    for(; i != m_channels.end(); ++i)
        i.value()->setUserStatus(nick, status);
    QMap<QString, GeneralChatWidget*>::iterator it = m_privateChannels.begin();
    for(; it != m_privateChannels.end(); ++it)
        it.value()->setUserStatus(nick, status);
}

void ChatTabWidget::setChannelsList(QMap<QString, QString> list)
{
    QMap<QString, QString>::iterator channel = list.begin();
    for(;channel != list.end(); ++channel)
        if(!m_channels.contains(channel.key()) && !m_privateChannels.contains(channel.key()))
            joinChannel(channel.key());
}

void ChatTabWidget::tabCloseResult(int index)
{
    if(tabText(index) != "main")
    {
        if (QMessageBox::question(this, "Exit confirmation.",
                                  "Are you sure you want to leave this channel?",
                                  QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
            return;
        QString channelName = tabText(index);
        emit leaveChannel(channelName);
        if(m_channels.contains(channelName))
            m_channels.remove(channelName);
        else
            m_privateChannels.remove(channelName);
        removeTab(index);
    }
}

void ChatTabWidget::currentChangedHandler(int index)
{
    if(m_channels.contains(tabText(index)))
        setTabIcon(index, m_channelIcon);
    else
        setTabIcon(index, m_PMIcon);
}

void ChatTabWidget::replyUserInformationClicked(QString nick)
{
    if(nick == m_ownerNick)
        emit showOwnerInfo();
    else
        emit onUserInformationClicked(nick);
}

void ChatTabWidget::replyPrivateMessageClicked(QString nick)
{
    emit onPrivateMessageClicked(nick);
}

void ChatTabWidget::replyNickClicked(QString nick)
{
    emit onNickClicked(nick + "> ");
}

void ChatTabWidget::replyLastMessage(QString message)
{
    emit lastMessage(message);
}

void ChatTabWidget::replyThemeChanged(QString theme)
{
    emit themeChanged(currentChannel(), theme);
}


