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

#include "ext_udp.h"

IWExtern::IWExtern(QWidget *parent)
    : QWidget(parent)
{
    unsigned short j;

    ui.setupUi(this);


    ui.P1_led_x1->setColor(Qt::white);
    ui.P1_led_x1->setChecked(true);
    ui.P1_led_x2->setColor(Qt::white);
    ui.P1_led_x2->setChecked(true);
    ui.P1_led_x3->setColor(Qt::white);
    ui.P1_led_x3->setChecked(true);
    ui.P1_led_y1->setColor(Qt::white);
    ui.P1_led_y1->setChecked(true);
    ui.P1_led_y2->setColor(Qt::white);
    ui.P1_led_y2->setChecked(true);
    ui.P1_led_y3->setColor(Qt::white);
    ui.P1_led_y3->setChecked(true);

    QPalette pal(Qt::black);
    pal.setColor(QPalette::WindowText, Qt::green);
    ui.Dec_LCD->setPalette(pal);
    ui.Dec_LCD->setDecMode();

    pal.setColor(QPalette::WindowText, Qt::yellow);
    ui.Hex_LCD->setPalette(pal);
    ui.Hex_LCD->setHexMode();

    pal.setColor(QPalette::WindowText, Qt::cyan);
    ui.Bin_LCD->setPalette(pal);
    ui.Bin_LCD->setBinMode();

    connect(ui.startCalButton_Ex1, SIGNAL(clicked()), SLOT(Ex1_Procedure()));
    connect(ui.startCalButton_Ex2, SIGNAL(clicked()), SLOT(Ex2_Procedure()));
    connect(ui.startCalButton_Ex3, SIGNAL(clicked()), SLOT(Ex3_Procedure()));
    connect(ui.startBoot, SIGNAL(clicked()), SLOT(BootMode()));
    connect(ui.softReset, SIGNAL(clicked()), SLOT(SoftReset()));

    ui.Dec_LCD->setNumDigits(12);
    ui.Hex_LCD->setNumDigits(12);
    ui.Bin_LCD->setNumDigits(32);

    //ui.qwtPlotA->replot();
    //ui.qwtPlotB->replot();
}

IWExtern::~IWExtern()
{
}

void IWExtern::BootMode(void)
{
    unsigned char bMode;

#ifdef FIXME_NOW
    bMode = vb_BootProcedure();
    if (bMode)
    {
        ui.colorLabel->setStyleSheet("QLineEdit{background: red;}");
        ui.colorLabel->setAutoFillBackground(true);
        ui.colorLabel->setText("Enterning Boot Mode");
    }
#endif
}
void IWExtern::SoftReset(void)
{
    unsigned char bMode;

#ifdef FIXME_NOW
    bMode = vb_SoftReset();
    if (bMode)
    {
        ui.colorLabel->setStyleSheet("QLineEdit{background: red;}");
        ui.colorLabel->setAutoFillBackground(true);
        ui.colorLabel->setText("SoftReset Mode");
    }
#endif
}

void IWExtern::LED_Action(volatile int led_Num, volatile int xColor)
{
    switch (led_Num)
    {
    case 1:
        ui.P1_led_x1->setColor(Qt::GlobalColor(xColor));
        break;
    case 2:
        ui.P1_led_x2->setColor(Qt::GlobalColor(xColor));
        break;
    case 3:
        ui.P1_led_x3->setColor(Qt::GlobalColor(xColor));

        break;
    case 4:
        ui.P1_led_y1->setColor(Qt::GlobalColor(xColor));

        break;
    case 5:
        ui.P1_led_y2->setColor(Qt::GlobalColor(xColor));
        break;
    case 6:
        ui.P1_led_y3->setColor(Qt::GlobalColor(xColor));
        break;
    default:
        break;
    }

}

void IWExtern::Banner_Action(int type_Text)
{
    char banner_text[1023];
    unsigned char xcolor;

#if 0
    if (type_Text == 0xffff)
    {
        ui.MainBox_Msg->clear();
    }
    else if (type_Text == 1)
    {
#ifdef FIXME_NOW
        vb_ReadText(banner_text, &xcolor);
#endif
        ui.MainBox_Msg->append(banner_text);
    }
    else
    {
#ifdef FIXME_NOW
        vb_ReadBanner(banner_text, &xcolor);
#endif
        ui.colorLabel->setText(banner_text);

        if (xcolor == IW_GREEN)
        {
            ui.colorLabel->setStyleSheet("QLineEdit{background: green;}");
            ui.colorLabel->setAutoFillBackground(true);
        }
        else if (xcolor == IW_RED)
        {
            ui.colorLabel->setStyleSheet("QLineEdit{background: red;}");
            ui.colorLabel->setAutoFillBackground(true);
        }
        else if (xcolor == IW_BLUE)
        {
            ui.colorLabel->setStyleSheet("QLineEdit{background: blue;}");
            ui.colorLabel->setAutoFillBackground(true);
        }
        else if (xcolor == IW_YELLOW)
        {
            ui.colorLabel->setStyleSheet("QLineEdit{background: yellow;}");
            ui.colorLabel->setAutoFillBackground(true);
        }
        else if (xcolor == IW_BLACK)
        {
            ui.colorLabel->setStyleSheet("QLineEdit{background: black;}");
            ui.colorLabel->setAutoFillBackground(true);
        }
        else
        {
            ui.colorLabel->setStyleSheet("QLineEdit{background: white;}");
            ui.colorLabel->setAutoFillBackground(true);
        }
    }

#endif

}

void IWExtern::Winlog_Action(void)
{
    ui.P1_led_y3->setColor(Qt::green);
}

void IWExtern::UserIn(void)
{
}

void IWExtern::slot_CSCRIPT_Extern(int i)
{

    switch (i)
    {
    case CSCRIPT_LED:
    {
        Pull_CSCRIPT(i, vPtrUchar);
        LED_Action(vPtrUchar[0], vPtrUchar[1]);
    }
    break;
    case CSCRIPT_LCD_DEC:
    {
        unsigned long  lVal;
        Pull_CSCRIPT(i, &lVal);
        ui.Dec_LCD->display((int)lVal);
    }
    break;
    case CSCRIPT_LCD_HEX:
    {
        unsigned long  lVal;
        Pull_CSCRIPT(i, &lVal);
        ui.Hex_LCD->display((int)lVal);
    }
    break;
    case CSCRIPT_LCD_BIN:
    {
        unsigned long  lVal;
        Pull_CSCRIPT(i, &lVal);
        ui.Bin_LCD->display((int)lVal);
    }
    break;
    case CSCRIPT_BANNER:
    {
        char str[64];
        Pull_CSCRIPT(i, str);
        ui.superBanner->setText(str);
    }
    break;
    case CSCRIPT_GRAPH1:
        break;
    case CSCRIPT_GRAPH2:
        break;
    case CSCRIPT_TEXT:
    {
        char str[256];
        memset(str, '\0', sizeof(str));
        Pull_CSCRIPT(i, str);
        ui.MainBox_Msg->append(str);
    }
    break;
    case CSCRIPT_CLEAR_TEXT:
        ui.MainBox_Msg->clear();
        break;
    default:
        break;

    }
}

void IWExtern::Ex1_Procedure(void)
{
    //RunCFile("cscript1.c");
}

void IWExtern::Ex2_Procedure(void)
{
    //RunCFile("cscript2.c");
}

void IWExtern::Ex3_Procedure(void)
{
    //RunCFile("cscript3.c");
}
