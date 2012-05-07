#include "ChatWidgets.h"
#include <QDebug>

GeneralChatWidget::GeneralChatWidget(QWidget *parent) :
    QWidget(parent)
{
    m_users = new QStringList;
    m_theme = new ThemeLabel(this);
    m_textBrowser = new chatTextBrowser(this);
    m_userList = new UserListWidget(this, m_users);
    m_label = new QLabel("Users in channel: " + QString::number(m_userList->count()), this);
    m_theme->setFrameShape(QFrame::StyledPanel);
    m_theme->setFrameShadow(QFrame::Raised);
    m_theme->setAlignment(Qt::AlignHCenter);
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
}

void GeneralChatWidget::setUserList(QStringList list)
{
    list.sort();
    m_users->clear();
    m_users->append(list);
    m_userList->addItems(list);
    m_label->setText("Users in channel: " + QString::number(m_users->count()));
}

void GeneralChatWidget::setUserStatus(QString nick, QString status)
{
    if(m_users->contains(nick))
    {
        if(status != "")
            m_userList->item(m_users->indexOf(nick))->setText(nick + "[" + status + "]");
        else
            m_userList->item(m_users->indexOf(nick))->setText(nick);
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
    m_theme->changeTheme(theme);
}

void GeneralChatWidget::replyUserInformationClicked(QString nick)
{
    emit onUserInformationClicked(nick);
}

void GeneralChatWidget::replyPrivateMessageClicked(QString nick)
{
    emit onPrivateMessageClicked(nick);
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
        menu->addAction("Send private message", this, SLOT(sendPMSignal()));
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
}

void ThemeLabel::changeTheme(QString theme)
{
    setText(theme);
    if(!m_dialog.isHidden())
        m_dialog.hide();
}

void ThemeLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        m_dialog.setTheme(this->text());
        m_dialog.show();
        connect(&m_dialog, SIGNAL(onOkButtonClicked(QString)), this, SLOT(changeTheme(QString)));
    }
}

ChatTabWidget::ChatTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    setTabsClosable(true);
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseResult(int)));
    joinChannel("main");
}

void ChatTabWidget::joinChannel(QString name)
{
    GeneralChatWidget *channel = new GeneralChatWidget(this);
    connect(channel, SIGNAL(onPrivateMessageClicked(QString)), this, SLOT(replyPrivateMessageClicked(QString)));
    connect(channel, SIGNAL(onUserInformationClicked(QString)), this, SLOT(replyUserInformationClicked(QString)));
    m_channels.insert(name, channel);
    addTab(channel, m_channelIcon, name);
}

void ChatTabWidget::joinPrivateChannel(QString name)
{
    GeneralChatWidget *channel = new GeneralChatWidget(this);
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

void ChatTabWidget::tabCloseResult(int index)
{
    if(tabText(index) != "main")
    {
        QString channelName = tabText(index);
        emit leaveChannel(channelName);
        if(m_channels.contains(channelName))
            m_channels.remove(channelName);
        else
            m_privateChannels.remove(channelName);
        removeTab(index);
    }
}

void ChatTabWidget::currentChangedHander(int index)
{
    if(m_channels.contains(tabText(index)))
        setTabIcon(index, m_channelIcon);
    else
        setTabIcon(index, m_PMIcon);
}

void ChatTabWidget::replyUserInformationClicked(QString nick)
{
    emit onUserInformationClicked(nick);
}

void ChatTabWidget::replyPrivateMessageClicked(QString nick)
{
    emit onPrivateMessageClicked(nick);
}


