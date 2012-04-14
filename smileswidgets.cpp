#include "smileswidgets.h"

chatTextBrowser::chatTextBrowser(QWidget *parent, QMap<QString, QString> *smilesMap) :
    QTextBrowser(parent)
{
    m_smilesMap = smilesMap;
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
    append(msg);

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
    delete m_smilesMap;
    delete m_layout;
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

