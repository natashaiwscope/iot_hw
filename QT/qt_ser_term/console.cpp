#include <QMainWindow>
#include <QByteArray>
#include <QList>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <qcombobox.h>
#include "console.h"
#include "ext_udp.h"

void Console::pollSerial(void)
{
    unsigned short rxLen = 0;
    rxLen = uart_read(portNum, (unsigned char *)rx_buff);

    if (!rxLen) return;
    QByteArray bytes = QByteArray::fromRawData((const char *)rx_buff, rxLen);
    if (bytes.contains(8))
    {
        // Must parse backspace commands manually
        for (int i = 0; i < bytes.count(); i++)
        {
            char ch = bytes.at(i);
            if (ch == 8)
            {
                // Backspace
                QString s = this->toPlainText();
                s.chop(1);
                setPlainText(s);
            }
            else
            {
                // Add char to edit
                QString s(ch);
                insertPlainText(s);
            }
            moveCursor(QTextCursor::End);
        }
    }
    else
    {
        moveCursor(QTextCursor::End);
        insertPlainText(bytes);
    }
    ensureCursorVisible();
}
// Grab keypresses meant for edit, send to serial port.
bool Console::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (1)
        {
            QByteArray ba = (keyEvent->text()).toLatin1();
            const char *c_str2 = ba.data();

            ba.replace("\r", "\r\n");
            uart_write(portNum, (unsigned char *)c_str2, ba.count());

            //qDebug() << "console tx port Num=" << portNum << endl; fflush(stdout);
        }
        return true;
    }
    else
        // standard event processing
        return QObject::eventFilter(obj, event);
}


Console::Console(short pNum,QWidget *parent) : QPlainTextEdit(parent)
{
    prompt = "redis> ";
    portNum=pNum;

    QPalette p = palette();
    settingsUi = new Ui::SettingsDialog;

    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);

    installEventFilter(this);

    QFont font3;
    font3.setPointSize(14);
    font3.setBold(true);
    font3.setWeight(75);

    QTextDocument *doc = this->document();
    doc->setDefaultFont(font3);

    history = new QStringList;
    historyPos = 0;
    isLocked = false;

    dTimer.setInterval(1);
    dTimer.setSingleShot(true);
    dTimer.start();

    //connect(this,SIGNAL(onCommand(QString)),this,SLOT(OnActionCommand(QString)));
    connect(&dTimer, SIGNAL(timeout()), this, SLOT(dTimerExpired()));
}

#if 0
void Console::keyPressEvent(QKeyEvent *event)
{
    if (isLocked)
        return;
    if (event->key() >= 0x20 && event->key() <= 0x7e
            && (event->modifiers() == Qt::NoModifier || event->modifiers() == Qt::ShiftModifier))
        QPlainTextEdit::keyPressEvent(event);
    if (event->key() == Qt::Key_Backspace
            && event->modifiers() == Qt::NoModifier
            && textCursor().positionInBlock() > prompt.length())
        QPlainTextEdit::keyPressEvent(event);
    if (event->key() == Qt::Key_Return && event->modifiers() == Qt::NoModifier)
        onEnter();
    if (event->key() == Qt::Key_Up && event->modifiers() == Qt::NoModifier)
        historyBack();
    if (event->key() == Qt::Key_Down && event->modifiers() == Qt::NoModifier)
        historyForward();
    QString cmd = textCursor().block().text().mid(prompt.length());
    emit onChange(cmd);
}
#endif

void Console::mousePressEvent(QMouseEvent *)
{
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *) {}

void Console::contextMenuEvent(QContextMenuEvent *) {}

void Console::historyAdd(QString cmd)
{
    history->append(cmd);
    historyPos = history->length();
}

void Console::historyBack()
{
    if (!historyPos)
        return;
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(prompt + history->at(historyPos - 1));
    setTextCursor(cursor);
    historyPos--;
}

void Console::historyForward()
{
    if (historyPos == history->length())
        return;
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    if (historyPos == history->length() - 1)
        cursor.insertText(prompt);
    else
        cursor.insertText(prompt + history->at(historyPos + 1));
    setTextCursor(cursor);
    historyPos++;
}

void Console::onActionCommand(QString s)
{
    unsigned short len;
    Q2C(s, (char *)tmp_buff);
    len = strlen((char *)tmp_buff);
}

//qDebug() << "console inp" << endl; fflush(stdout);
void Console::consoleInpSlot()
{
    dTimer.start();
}

void Console::dTimerExpired()
{
    pollSerial();
}
