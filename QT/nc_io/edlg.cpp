#include <QMainWindow>
#include <QtGui>
#include <QColorDialog>
#include <qapplication.h>
#include <qmainwindow.h>
#include <qtoolbar.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <qtoolbar.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtoolbar.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include "edlg.h"

class SpinBox: public QSpinBox
{
public:
    SpinBox(int min, int max, int step, QWidget *parent):
        QSpinBox(parent)
    {
        setRange(min, max);
        setSingleStep(step);
    }
};


class CheckBox: public QCheckBox
{
public:
    CheckBox(const QString &title, QWidget *parent):
        QCheckBox(title, parent)
    {
    }

    void setChecked(bool checked)
    {
        setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    }

    bool isChecked() const
    {
        return checkState() == Qt::Checked;
    }
};

EDlg::~EDlg()
{
    int i;
    for (i = 0; i < 3; i++)
    {
        free (par[i].rx_buff);
        free (par[i].tx_buff);
        //par[i].settings->setValue("PORT/BAUD",par[i].uart_baud);
        //par[i].settings->setValue("PORT/STOP",par[i].uart_stop);
        //par[i].settings->setValue("PORT/DBITS",par[i].uart_databits);
        //par[i].settings->setValue("PORT/PARITY",par[i].uart_parity);
        //par[i].settings->setValue("PORT/FORMAT",par[i].uart_format);
        //par[i].settings->sync();
    }
}

void EDlg::timeoutSlot()
{
    tabChanged(0);
}

EDlg::EDlg(QWidget *parent): QTabWidget(parent)
{
    currTab = 0;

    setTabPosition(QTabWidget::West);

    addTab(createRS485Tab(this), "RS485");
    addTab(createUART2Tab(this), "UART2");
    addTab(createUART3Tab(this), "UART3");
    //addTab(createUART23Tab(this),"UART23");
    guiTimer.setInterval(200);
    guiTimer.setSingleShot(true);


    connect(this, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    connect(&guiTimer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
    guiTimer.start();
}

QWidget *EDlg::createRS485Tab(QWidget *parent)
{
    QWidget *page = new QWidget(parent);

    int row = 0;

    PopulateDlg(page, 0);

    return page;
}

QWidget *EDlg::createUART3Tab(QWidget *parent)
{
    QWidget *page = new QWidget(parent);

    PopulateDlg(page, 2);

    return page;
}

QWidget *EDlg::createUART23Tab(QWidget *parent)
{
    QWidget *page = new QWidget(parent);
    return page;
}


QWidget *EDlg::createUART2Tab(QWidget *parent)
{
    QWidget *page = new QWidget(parent);

    PopulateDlg(page, 1);

    return page;
}

void EDlg::tabChanged(int tabNum)
{
    //QFont font;=QFont(par[currTab].fontLabel->text()), this);
    currTab = tabNum;
    //printf("tab changed %d\n",tabNum);
    //fflush(stdout);

    QFont dum;
    dum.fromString(par[currTab].fontStr);
    par[currTab].fontLabel->setFont(dum);
    Q_EMIT fontChanged(dum, currTab);
}

void EDlg::startClicked()
{
    printf("start clicked");
    fflush(stdout);
    par[currTab].led->On();

    qDebug() << "Baud Bit " <<  par[currTab].d_BaudRate->currentIndex()   << "\n";
    qDebug() << "Stop Bit " <<  par[currTab].d_StopBit->currentIndex()    << "\n";
    qDebug() << "Data Bit " <<  par[currTab].d_DataBits->currentIndex()   << "\n";
    qDebug() << "Parity  "  <<  par[currTab].d_Parity->currentIndex()     << "\n";
    qDebug() << "Format "   <<  par[currTab].d_FmtOptions->currentIndex() << "\n";
}

void EDlg::stopClicked()
{
    printf("stop clicked");
    fflush(stdout);
    par[currTab].led->Off();
}

void EDlg::saveClicked()
{
    int j;
    unsigned short len;

    printf("save clicked %d", currTab);
    fflush(stdout);

    // First baud rate
    if (par[currTab].d_BaudRate->currentIndex() < 14)
    {
        par[currTab].settings->setValue("PORT/BAUD", par[currTab].d_BaudRate->currentIndex());
        par[currTab].uart_baud = par[currTab].d_BaudRate->currentIndex();
    }

    if (par[currTab].uart_baud == 0) par[currTab].uart_baud = 110;
    if (par[currTab].uart_baud == 1) par[currTab].uart_baud = 150;
    if (par[currTab].uart_baud == 2) par[currTab].uart_baud = 300;
    if (par[currTab].uart_baud == 3) par[currTab].uart_baud = 1200;
    if (par[currTab].uart_baud == 4) par[currTab].uart_baud = 2400;
    if (par[currTab].uart_baud == 5) par[currTab].uart_baud = 4800;
    if (par[currTab].uart_baud == 6) par[currTab].uart_baud = 9600;
    if (par[currTab].uart_baud == 7) par[currTab].uart_baud = 19200;
    if (par[currTab].uart_baud == 8) par[currTab].uart_baud = 38400;
    if (par[currTab].uart_baud == 9) par[currTab].uart_baud = 57600;
    if (par[currTab].uart_baud == 10) par[currTab].uart_baud = 115200;
    if (par[currTab].uart_baud == 11) par[currTab].uart_baud = 230400;
    if (par[currTab].uart_baud == 12) par[currTab].uart_baud = 460800;
    if (par[currTab].uart_baud == 13) par[currTab].uart_baud = 921600;

    //par[currTab].uart_baud=par[currTab].d_BaudRate->currentIndex();

    par[currTab].uart_stop = par[currTab].d_StopBit->currentIndex();
    par[currTab].uart_databits = par[currTab].d_DataBits->currentIndex();
    par[currTab].uart_parity = par[currTab].d_Parity->currentIndex();

    par[currTab].settings->setValue("PORT/STOP",   par[currTab].d_StopBit->currentIndex());
    par[currTab].settings->setValue("PORT/DBITS",  par[currTab].d_DataBits->currentIndex());
    par[currTab].settings->setValue("PORT/PARITY", par[currTab].d_Parity->currentIndex());
    par[currTab].settings->setValue("PORT/FORMAT", par[currTab].d_FmtOptions->currentIndex());
    par[currTab].settings->setValue("PORT/FONT",   par[currTab].fontStr);
    par[currTab].settings->setValue("PORT/DSIZE",  par[currTab].d_BuffSize->value());


    par[currTab].settings->sync();

    printf("baud=%d\n",     par[currTab].uart_baud);
    printf("stopbit=%d\n",  par[currTab].uart_stop);
    printf("bits=%d\n",     par[currTab].uart_databits);
    printf("parity=%d\n",   par[currTab].uart_parity);
    printf("baud=%d\n",     par[currTab].uart_baud);

    tx_glu.io_uart.uart_num      = currTab;
    tx_glu.io_uart.uart_stop     = par[currTab].uart_stop;
    tx_glu.io_uart.uart_databits = par[currTab].uart_databits;
    tx_glu.io_uart.uart_parity   = par[currTab].uart_parity;
    tx_glu.io_uart.uart_baud     = par[currTab].uart_baud;


}


void EDlg::pushsettings()
{
    printf("push settings\n");
    fflush(stdout);
}

void EDlg::PopulateDlg(QWidget *page, int i)
{
    int row = 0;
    int j;
    char inifilename[20];

    //QWidget *page       = new QWidget(parent);
    QGridLayout *layout = new QGridLayout(page);

    //layout->setSpacing(5);
    //layout->setVerticalSpacing(5);
    //layout->setHorizontalSpacing(5);


#define   INI0_FILE  "rs485.ini"
#define   INI1_FILE  "ttl2_uart.ini"
#define   INI2_FILE  "ttl3_uart.ini"

    par[i].but_Start   = new QPushButton("START", this);
    par[i].but_Stop    = new QPushButton("Stop", this);
    par[i].but_Save    = new QPushButton("Save", this);
    par[i].but_Font    = new QPushButton("Font", this);

    par[i].led         = new LEDWidget(this);
    par[i].but_Start->resize(20, 20);

    layout->addWidget(par[i].but_Start, row, 0);
    layout->addWidget(par[i].led, row++, 1);
    layout->addWidget(par[i].but_Stop, row, 0);
    layout->addWidget(par[i].but_Save, row++, 1);


    if (i == 0)      par[i].settings     = new QSettings(INI0_FILE, QSettings::IniFormat);
    else if (i == 1) par[i].settings     = new QSettings(INI1_FILE, QSettings::IniFormat);
    else if (i == 2) par[i].settings     = new QSettings(INI2_FILE, QSettings::IniFormat);

    par[i].uart_baud    = par[i].settings->value("PORT/BAUD", 10).toInt();
    par[i].uart_stop    = par[i].settings->value("PORT/STOP").toInt();
    par[i].uart_databits = par[i].settings->value("PORT/DBITS").toInt();
    par[i].uart_parity  = par[i].settings->value("PORT/PARITY").toInt();
    par[i].uart_format  = par[i].settings->value("PORT/FORMAT").toInt();
    par[i].uart_dsize   = par[i].settings->value("PORT/DSIZE", 1000).toInt();

    par[i].rx_buff = new unsigned char(par[i].uart_dsize);
    par[i].tx_buff = new unsigned char(par[i].uart_dsize);

    //init_sq_mutex(&par[i].tx,par[i].tx_buff,par[i].uart_dsize,"");
    //init_sq_mutex(&par[i].rx,par[i].rx_buff,par[i].uart_dsize,"");

    QVariant valApp0    = par[i].settings->value("PORT/FONT");
    if (valApp0.type() == QVariant::StringList)
    {
        par[i].fontStr = valApp0.toStringList().join(",");
    }
    else
    {
        par[i].fontStr = valApp0.toString();
    }

    // Baud button & Baudrate combo box
    par[i].but_Baud   = new QPushButton("Baud", this);
    par[i].but_Baud->resize(20, 20);
    layout->addWidget(par[i].but_Baud, row, 0);

    par[i].d_BaudRate = new QComboBox(page);
    par[i].d_BaudRate->resize(20, 20);

    par[i].d_BaudRate->addItem("110");
    par[i].d_BaudRate->addItem("150");
    par[i].d_BaudRate->addItem("300");
    par[i].d_BaudRate->addItem("1200");
    par[i].d_BaudRate->addItem("2400");
    par[i].d_BaudRate->addItem("4800");
    par[i].d_BaudRate->addItem("9600");
    par[i].d_BaudRate->addItem("19200");
    par[i].d_BaudRate->addItem("38400");
    par[i].d_BaudRate->addItem("57600");
    par[i].d_BaudRate->addItem("115200");
    par[i].d_BaudRate->addItem("230400");
    par[i].d_BaudRate->addItem("460800");
    par[i].d_BaudRate->addItem("921600");
    par[i].d_BaudRate->addItem("1000000");

    par[i].d_BaudRate->resize(20, 20);

    qDebug()  << par[i].d_BaudRate->count();

    if (par[i].uart_baud >= par[i].d_BaudRate->count())
    {
        char buff[200];
        sprintf(buff, "%d", par[i].uart_baud);
        par[i].d_BaudRate->addItem(buff);
        par[i].d_BaudRate->setCurrentIndex(par[i].d_BaudRate->count() - 1);
    }
    else
    {
        par[i].d_BaudRate->setCurrentIndex(par[i].uart_baud);
    }

    layout->addWidget(par[i].d_BaudRate, row++, 1);


    // Stop button & Stopbit combo box
    par[i].but_StopBit   = new QPushButton("StopBits", this);
    par[i].but_StopBit->resize(20, 20);

    layout->addWidget(par[i].but_StopBit, row, 0);
    par[i].d_StopBit = new QComboBox(page);
    par[i].d_StopBit->resize(20, 20);
    par[i].d_StopBit->addItem("1");
    par[i].d_StopBit->addItem("1.5");
    par[i].d_StopBit->addItem("2");
    layout->addWidget(par[i].d_StopBit, row++, 1);

    par[i].d_StopBit->setCurrentIndex(par[i].uart_stop);

    // Parity Button  & Parity combo box
    par[i].but_Parity   = new QPushButton("Parity", this);
    par[i].but_Parity->resize(20, 20);
    layout->addWidget(par[i].but_Parity, row, 0);
    par[i].d_Parity = new QComboBox(page);
    par[i].d_Parity->resize(20, 20);
    par[i].d_Parity->addItem("None");
    par[i].d_Parity->addItem("Odd");
    par[i].d_Parity->addItem("Even");
    layout->addWidget(par[i].d_Parity, row++, 1);
    par[i].d_Parity->setCurrentIndex(par[i].uart_parity);

    // Parity Button  & Parity combo box
    par[i].but_DBits   = new QPushButton("DataBits", this);
    par[i].but_DBits->resize(20, 20);
    layout->addWidget(par[i].but_DBits, row, 0);
    par[i].d_DataBits = new QComboBox(page);
    par[i].d_DataBits->addItem("7");
    par[i].d_DataBits->addItem("8");
    par[i].d_DataBits->addItem("9");
    layout->addWidget(par[i].d_DataBits, row++, 1);
    par[i].d_DataBits->setCurrentIndex(par[i].uart_databits);

    // Parity Button  & Parity combo box
    par[i].but_Fmt   = new QPushButton("Format", this);
    par[i].but_Fmt->resize(20, 20);
    layout->addWidget(par[i].but_Fmt, row, 0);
    par[i].d_FmtOptions = new QComboBox(page);
    par[i].d_FmtOptions->addItem("hex");
    par[i].d_FmtOptions->addItem("0xhex");
    par[i].d_FmtOptions->addItem("dec");
    par[i].d_FmtOptions->addItem("char");
    layout->addWidget(par[i].d_FmtOptions, row++, 1);
    par[i].d_FmtOptions->setCurrentIndex(par[i].uart_format);

    par[i].fontLabel = new QLabel(page);


    layout->addWidget(par[i].but_Font, row, 0);
    layout->addWidget(par[i].fontLabel, row++, 1);


    par[i].fontLabel->setText(par[i].fontStr);

    layout->setColumnStretch(1, 10);
    layout->setRowStretch(row, 10);

    QFont dum;
    dum.fromString(par[i].fontStr);
    par[i].fontLabel->setFont(dum);

    qDebug() << par[i].fontStr;


    par[i].but_Buff   = new QPushButton("Buf Size", page);
    par[i].but_Buff->resize(20, 20);
    layout->addWidget(par[i].but_Buff, row, 0);

    //par[i].d_BuffSize = new SpinBox(0,par[i].uart_dsize,1, page);
    par[i].d_BuffSize = new SpinBox(0, 50000, 1, page);
    layout->addWidget(par[i].d_BuffSize, row++, 1);

    par[i].d_BuffSize->setValue(par[i].uart_dsize);

    connect(par[i].but_Start,  SIGNAL(clicked()), SLOT(startClicked()));
    connect(par[i].but_Stop,   SIGNAL(clicked()), SLOT(stopClicked()));
    connect(par[i].but_Save,   SIGNAL(clicked()), SLOT(saveClicked()));
    connect(par[i].but_Font,   SIGNAL(clicked()), SLOT(setFont()));

    par[i].led->Sync();
    par[i].led->Off();

    par[i].lcd_Rx = new QLCDNumber(this);
    par[i].lcd_Tx = new QLCDNumber(this);


    QPalette pal(Qt::black);
    pal.setColor(QPalette::WindowText, Qt::green);
    par[i].lcd_Rx->setPalette(pal);
    par[i].lcd_Tx->setPalette(pal);

    par[i].lcd_Tx->setDecMode();
    par[i].lcd_Rx->setDecMode();

    par[i].lcd_Rx->setGeometry(QRect(0, 390, 271, 71));
    par[i].lcd_Rx->setSmallDecimalPoint(true);
    par[i].lcd_Rx->setNumDigits(6);

    par[i].lcd_Tx->setGeometry(QRect(0, 390, 271, 71));
    par[i].lcd_Tx->setSmallDecimalPoint(true);
    par[i].lcd_Tx->setNumDigits(6);


    par[i].but_RxCnt = new QPushButton("RxCnt", this);
    par[i].but_TxCnt = new QPushButton("TxCnt", this);
    par[i].but_ClrCnt = new QPushButton("ClrCnt", this);

    layout->addWidget(par[i].but_RxCnt, row, 0);
    layout->addWidget(par[i].lcd_Rx, row++, 1);
    layout->addWidget(par[i].but_TxCnt, row, 0);
    layout->addWidget(par[i].lcd_Tx, row++, 1);
    layout->addWidget(par[i].but_ClrCnt, row, 0);

    //for(j=0;j<par[i].d_BaudRate->count();j++) { qDebug() << par[i].d_BaudRate->itemText(j); }

}

void EDlg::setFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont(par[currTab].fontLabel->text()), this);
    if (ok)
    {
        par[currTab].fontLabel->setText(font.key());
        par[currTab].fontLabel->setFont(font);
        par[currTab].fontStr = font.toString();

        QFont dum;
        dum.fromString(par[currTab].fontStr);
        Q_EMIT fontChanged(dum, currTab);
    }
}

void EDlg::updateTxLCD(int i, int j)
{
    //qDebug() << "LCD needs to be changed" << i << "  " << j << "\n";
    par[currTab].lcd_Tx->display(j);
}

void EDlg::updateRxLCD(int i, int j)
{
    //qDebug() << "LCD needs to be changed" << i << "  " << j << "\n";
    par[i].lcd_Rx->display(j);
}

