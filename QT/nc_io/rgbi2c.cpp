#include <qmainwindow.h>
#include <qapplication.h>
#include <qtoolbar.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmainwindow.h>
#include <qcombobox.h>
#include <qevent.h>
#if QT_VERSION < 0x040000
#include <qwhatsthis.h>
#endif
#include <QtGui>

#include "rgbi2c.h"
#include "ext_udp.h"

RGBI2C::RGBI2C(QWidget *parent) : QWidget(parent)
{
    unsigned short j;

    ui.setupUi(this);

    isCont = 0;
    lastCmd =0;

    QPalette pal(Qt::black);
    pal.setColor(QPalette::WindowText, Qt::green);

    connect(ui.readCHIP_ID,     SIGNAL(clicked()), SLOT(readCHIP_ID()));
    connect(ui.Enable_RGB,      SIGNAL(clicked()), SLOT(Enable_RGB()));
    connect(ui.ReadRGB,         SIGNAL(clicked()), SLOT(ReadRGB()));

    //connect(ui.scanI2CDevice, SIGNAL(clicked()), SLOT(scanI2CDevice()));
    connect(this, SIGNAL(ci2c_data()), this, SLOT(ci2c_data_slot()));
    //connect(this, SIGNAL(ci2c_result()), this, SLOT(ci2c_result_slot()));

    QFont font3;
    font3.setPointSize(24);
    font3.setBold(true);
    font3.setWeight(75);
    ui.R->setFont(font3);
    ui.G->setFont(font3);
    ui.B->setFont(font3);
    ui.chipID->setFont(font3);
}

void RGBI2C::readCHIP_ID()
{
    lastCmd =0;
    i2c_read(0x72, 0x92, 1, b, 1);
}

void RGBI2C::Enable_RGB()
{
    lastCmd =1;
    // Enable 

    uint8_t enable = 0b00000001;
    uint8_t val;
    enable = enable << 4;
    val &= 0b11101111;
    val |= enable;
    b[0]=0x03;
    i2c_write(0x72,0x80,1,b,1);

    // Enable, Set GAIN 
    b[0]=0x03;
    i2c_write(0x72,0x8f,1,b,1);

}

int RGBI2C::i2c_write(unsigned char chip_addr, unsigned short int_addr, unsigned short int_addr_len, unsigned char *buff, unsigned short dlen)
{
    i2c_mem_write(chip_addr, int_addr, int_addr_len, buff, dlen);
    return 1;
}

int RGBI2C::i2c_read(unsigned char chip_addr, unsigned short int_addr, unsigned short int_addr_len, unsigned char *buff, unsigned short dlen)
{
    unsigned char buffer[1024];
    i2c_mem_read(chip_addr, int_addr, int_addr_len, buffer, dlen);
    return 1;
}



void RGBI2C::ReadRGB()
{
    lastCmd =2;
    i2c_read(0x72, 0x94, 1, b, 10);
}

void RGBI2C::scanI2CDevice()
{
    fun_i2c_scan();
}

void RGBI2C::ReadDeviceContinouous()
{
    unsigned char b[2];
    i2c_mem_read(0xB4, 0x07, 1, b, 2);
}

void RGBI2C::paintEvent(QPaintEvent *event)
{
    QWidget *wdg = findChild<QWidget *>("buttonWidget");//--> I catch my widget
    QList<QPushButton *> buttons = wdg->findChildren < QPushButton *>();//--> I find pushButtons

    for (int i = 0; i < buttons.size(); i++)
    {
        int x = buttons.at(i)->pos().x();//--> measurements for triangle
        int y = buttons.at(i)->pos().y();
        int w = buttons.at(i)->width();
        int h = buttons.at(i)->height();

        QRect r(x + w, y, triangleWidth, h); //--> I create rectangular area between pushButtons

        QPainter painter(this);
        qreal point3X = x + w + triangleWidth; //--> triangle corners
        qreal point3Y = y + h / 2 ;
        qreal point1X = x + w;
        qreal point1Y = y;
        qreal point2X = x + w;
        qreal point2Y = y + h;

        QPainterPath path;
        path.moveTo (point1X, point1Y);
        path.lineTo (point2X, point2Y);
        path.lineTo (point3X, point3Y);

        painter.setPen (Qt :: NoPen);
        if (i != buttons.size() - 1) //--> I paint rectangular and triangle
        {
            painter.fillRect(r, QBrush (colors.at(i + 1)));
        }
        painter.fillPath (path, QBrush (colors.at(i)));
    }
}

void RGBI2C::createPushButtons()
{
    QWidget *wdg = new QWidget(this); //--> widget contains pushButtons
    wdg->setObjectName("buttonWidget");//--> I set object name in order to catch widget and pushButtons in PaintEvent()
    //setCentralWidget(wdg);//--> I add widget to app main layout

    for (int i = 0; i < colors.size(); i++) //--> Button count and color count must be same.
    {
        QPushButton *btn = new QPushButton(QString::number(i) + ". button", wdg); //--> create pushButtons in parent wdg
        btn->setGeometry(firstButtonX + (buttonWidth + triangleWidth)*i, firstButtonY, buttonWidth, buttonHeight); //--> I set geometry for pushButtons

        btn->setStyleSheet(QString("background-color: %1;border-style: outset;border-width: 0px;").arg(colors.at(i).name()));//--> I change background color and clear border
    }
}

RGBI2C::~RGBI2C()
{
}


void RGBI2C::ci2c_data_slot()
{
    unsigned short x;
    unsigned short r;
    unsigned short g;
    unsigned short b;
    unsigned char buf[20];
    char buffer[256];

    switch(lastCmd)
    {
        case 0:
            {
                i2c_read_buffer(buf, 10);
                sprintf(buffer, "0x%02X",0xff & buf[0]);
                ui.chipID->setText(buffer);

            }
            break;
        case 2:
            {
                i2c_read_buffer(buf, 10);
                r = ((0xff & buf[1]) << 8) | (0xff & buf[0]);
                g = ((0xff & buf[3]) << 8) | (0xff & buf[2]);
                b = ((0xff & buf[5]) << 8) | (0xff & buf[4]);

                sprintf(buffer, "%d", r); ui.R->setText(buffer);
                sprintf(buffer, "%d", g); ui.G->setText(buffer); 
                sprintf(buffer, "%d", b); ui.B->setText(buffer);
                ReadRGB();
            }
            break;
        default:
            break;
    }
#if 0
    char buffer[256];
    unsigned char b[2];
    unsigned char buf[2];
    unsigned short x;
    float temp;
    static unsigned int iCntr=0;

    //printf("0x%02x 0x%02x Res=%d\r\n" ,b[0] ,b[1],i2c_read_buffer(b,2));
    i2c_read_buffer(b, 2);
    x = ((0xff & b[1]) << 8) | (0xff & b[0]);
    temp = x;
    temp *= .02;
    temp  -= 273.15;

    //printf("-----------------------\r\n");
    //printf("[%d] F Temp=%f\r\n",++iii,((temp)*9.0f/5.0f)+32);
    fflush(stdout);
    sprintf(buffer, "%.3fF [%d]", ((temp) * 9.0f / 5.0f) + 32,++iCntr);
    i2c_mem_read(0xB4, 0x07, 1, buf, 2);
    ui.tempText->setText(buffer);
#endif

}

void RGBI2C::ci2c_result_slot()
{

}

