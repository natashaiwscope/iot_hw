#include <qmainwindow.h>
#include <qapplication.h>
#include <qwt_counter.h>
#include <qwt_slider.h>
#include <qtoolbar.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmainwindow.h>
#include <qwt_slider.h>
#include <qcombobox.h>
#include <qwt_analog_clock.h>
#include <qwt_plot.h>
#include <qevent.h>
#if QT_VERSION < 0x040000
#include <qwhatsthis.h>
#endif
#include <qwt_plot_layout.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_scale_widget.h>
#include <qwt_wheel.h>
#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_system_clock.h>
#include <QtGui>

#include "ci2c_io.h"
#include "ext_udp.h"

CI2C_IO::CI2C_IO(QWidget *parent) : QWidget(parent)
{
    unsigned short j;

    ui.setupUi(this);

    isCont = 0;

    QPalette pal(Qt::black);
    pal.setColor(QPalette::WindowText, Qt::green);

    //colors.append(Qt::red);
    //colors.append(Qt::blue);
    //colors.append(Qt::yellow);
    //createPushButtons();
    connect(ui.ReadI2CDevice, SIGNAL(clicked()), SLOT(ReadDeviceContinouous()));
    connect(ui.scanI2CDevice, SIGNAL(clicked()), SLOT(scanI2CDevice()));
    connect(this, SIGNAL(ci2c_data()), this, SLOT(ci2c_data_slot()));
    connect(this, SIGNAL(ci2c_result()), this, SLOT(ci2c_result_slot()));

    QFont font3;
    font3.setPointSize(48);
    font3.setBold(true);
    font3.setWeight(75);
    ui.tempText->setFont(font3);
}

void CI2C_IO::scanI2CDevice()
{
    fun_i2c_scan();
}

void CI2C_IO::ReadDeviceContinouous()
{
    unsigned char b[2];
    i2c_mem_read(0xB4, 0x07, 1, b, 2);
}

void CI2C_IO::paintEvent(QPaintEvent *event)
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

void CI2C_IO::createPushButtons()
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

CI2C_IO::~CI2C_IO()
{
}


void CI2C_IO::ci2c_data_slot()
{
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

}

void CI2C_IO::ci2c_result_slot()
{
    I2C_Scan_Result i2c_res;
    QString s;
    if (i2c_devices_cnt())
    {
        QString valueAddr1 = QString("0x%1").arg(i2c_devices_addr(0), 0, 16);
        QString valueAddr2 = QString("0x%1").arg(i2c_devices_addr(1), 0, 16);
        s = "Found I2C " + valueAddr1 + "," + valueAddr2;
    }
    else
    {
        s = "None I2C Device";
    }
    ui.tempText->setText(s);
}
