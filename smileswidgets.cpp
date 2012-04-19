#include "smileswidgets.h"
#include <QDebug>

chatTextBrowser::chatTextBrowser(QWidget *parent, QMap<QString, QString> *smilesMap) :
    QTextBrowser(parent)
{
    m_smilesMap = smilesMap;
    m_color = "navy";
    m_timePattern = "[hh:mm:ss]";
    m_ownerColor = "red";
}

void chatTextBrowser::setOwnerNick(QString nick)
{
    m_ownerNick = nick;
}

void chatTextBrowser::appendMessage(QString nick, QString msg)
{
    //:smile_name:
        if(m_smilesMap)
        {
            QString smileString;
            for(quint16 index = 0; index < msg.size(); index++)
            {
                if(msg[index] == QChar(':'))
                {
                    smileString = isSmile(index, msg);
                    if(!(smileString).isNull() && m_smilesMap->contains(smileString))
                        msg.replace(index, smileString.length() + 2, m_smilesMap->value(smileString));
                }
            }
        }
        if(msg.contains(m_ownerNick))
            msg.replace(m_ownerNick, QString("<FONT color=\"%1\">%2</FONT>").arg(m_ownerColor).arg(m_ownerNick));
        msg = QString("<FONT color=\"%1\">%2%3</FONT>: %4")
                .arg(m_color)
                .arg(QTime::currentTime().toString(m_timePattern))
                .arg(nick)
                .arg(msg);
        append(msg);
}

void chatTextBrowser::appendMessage(QString msg)
{
    //:smile_name:
        if(m_smilesMap)
        {
            QString smileString;
            for(quint16 index = 0; index < msg.size(); index++)
            {
                if(msg[index] == QChar(':'))
                {
                    smileString = isSmile(index, msg);
                    if(!(smileString).isNull() && m_smilesMap->contains(smileString))
                        msg.replace(index, smileString.length() + 2, m_smilesMap->value(smileString));
                }
            }
        }
        if(msg.contains(m_ownerNick))
            msg.replace(m_ownerNick, QString("<FONT color=\"%1\">%2</FONT>").arg(m_ownerColor).arg(m_ownerNick));
        msg = QString("<FONT color=\"%2\">%1%3</FONT>: %4")
                .arg(QTime::currentTime().toString(m_timePattern))
                .arg(m_color)
                .arg(m_ownerNick)
                .arg(msg);
        append(msg);
}

void chatTextBrowser::setNickColor(QString color)
{
    m_color = color;
}

void chatTextBrowser::setTimePattern(QString pattern)
{
    m_timePattern = pattern;
}

void chatTextBrowser::setOwnerColor(QString color)
{
    m_ownerColor = color;
}

QString chatTextBrowser::isSmile(quint16 index, QString msg)
{
    QString smileString;
    index++;
    while(msg[index] != QChar(':'))
    {
        if(index == msg.size() || msg[index] == QChar(' '))
            return smileString = "";
        smileString.append(msg[index]);
        index++;
    }
    return smileString;
}

void chatTextBrowser::mousePressEvent(QMouseEvent *ev)
{

    if(ev->button() == Qt::LeftButton)
    {
        quint8 size = m_timePattern.size();
        QString str;
        QTextCursor cursor = cursorForPosition(QPoint(ev->x(), ev->y()));
        str = cursor.block().text();
        str.remove(0, size);
        quint8 pos = cursor.positionInBlock();
        if(pos >= size && pos <= size + str.indexOf(':'))
            emit onNickClicked(str.mid(0, str.indexOf(':')));
    }
    QTextBrowser::mousePressEvent(ev);
}

void chatTextBrowser::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::NoButton)
    {
        quint8 size = m_timePattern.size();
        QTextCursor cursor = cursorForPosition(QPoint(ev->x(), ev->y()));
        quint8 pos = cursor.positionInBlock();
        if(pos >= size && pos <= size + cursor.block().text().remove(0, size).indexOf(':'))
        {
            if(QApplication::overrideCursor() == 0 || QApplication::overrideCursor()->shape() != Qt::PointingHandCursor)
                QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
        }
        else
        {
            QApplication::restoreOverrideCursor();
        }
    }
    QTextBrowser::mouseMoveEvent(ev);
}

smileImage::smileImage(QString filename)
{
    m_image.load(filename);
    setFixedWidth(m_image.width());
    setFixedHeight(m_image.height());
}

void smileImage::setSmileName(QString name)
{
    m_name = name;
}

void smileImage::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        emit smileImageClicked(m_name);
}

void smileImage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(m_image.rect(), m_image);
}

smilesWidget::smilesWidget(QWidget *parent) :
    QDialog(parent)
{
    m_layout = new QGridLayout;
    m_smilesMap = new QMap<QString, QString>;
    QString name;
    QString htmltag;
    QVector<smileImage *> vector;
    QDir dir("smiles");
    QStringList List;
    smileImage *smile;
    List = dir.entryList(QDir::Files, QDir::Name);
    QStringList::iterator itr = List.begin();
    for(;itr != List.end(); itr++)
    {

        name = itr[0];
        name = name.left(name.length() - 4);
        smile = new smileImage("smiles/" + itr[0]);
        smile->setSmileName(":" + name + ":");
        vector.append(smile);
        smile->setParent(this);
        smile->show();
        connect(smile, SIGNAL(smileImageClicked(QString)), this, SLOT(smileName(QString)));
        htmltag = QString("<img src=\"smiles/%1\">")
                .arg(itr[0]);
        m_smilesMap->insert(name, htmltag);
    }
    quint8 it = 0;
    quint8 i = 0;
    quint8 j = 0;
    quint8 k = sqrt(vector.size());
    while(it < vector.size())
    {
        m_layout->addWidget(vector[it], i,j);
        j++;
        if(j == k)
        {
            j = 0;
            i++;
        }
        it++;
    }
    m_layout->setMargin(5);
    m_layout->setSpacing(5);
    setLayout(m_layout);
    setFixedSize(m_layout->minimumSize());
}

smilesWidget::~smilesWidget()
{

}

QMap<QString, QString> *smilesWidget::getSmiles()
{
    return m_smilesMap;
}

void smilesWidget::smileName(QString name)
{
    emit smileClicked(name);
    close();
}

