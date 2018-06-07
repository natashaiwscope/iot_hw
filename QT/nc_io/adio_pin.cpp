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

#include "adio_pin.h"
#include "ext_udp.h"

ADIO_PIN::ADIO_PIN(QWidget *parent)
    : QWidget(parent)
{
    unsigned short j;

    ui.setupUi(this);

    ui.led0->setStyleSheet("background: lightGreen");
    ui.led1->setStyleSheet("background: gray");
    ui.led2->setStyleSheet("background: gray");
    ui.led3->setStyleSheet("background: gray");
    ui.led4->setStyleSheet("background: gray");
    ui.led5->setStyleSheet("background: gray");
    ui.led6->setStyleSheet("background: gray");

    QPalette pal(Qt::black);
    pal.setColor(QPalette::WindowText, Qt::green);

    ui.select1->setEditable(false); ui.select1->addItem("INPUT"); ui.select1->addItem("ANALOGIN"); ui.select1->addItem("OUTPUT");
    ui.select2->setEditable(false); ui.select2->addItem("INPUT"); ui.select2->addItem("ANALOGIN"); ui.select2->addItem("OUTPUT");
    ui.select3->setEditable(false); ui.select3->addItem("INPUT"); ui.select3->addItem("ANALOGIN"); ui.select3->addItem("OUTPUT");
    ui.select4->setEditable(false); ui.select4->addItem("INPUT"); ui.select4->addItem("ANALOGIN"); ui.select4->addItem("OUTPUT");

    ui.Dec_LCD1->setPalette(pal); ui.Dec_LCD1->setDecMode();
    ui.Dec_LCD2->setPalette(pal); ui.Dec_LCD2->setDecMode();
    ui.Dec_LCD3->setPalette(pal); ui.Dec_LCD3->setDecMode();
    ui.Dec_LCD4->setPalette(pal); ui.Dec_LCD4->setDecMode();


    ui.ILED_4->setStyleSheet("background: red");

    //connect(ui.startCalButton_Ex1, SIGNAL(clicked()), SLOT(Ex1_Procedure()));
    //connect(ui.startCalButton_Ex2, SIGNAL(clicked()), SLOT(Ex2_Procedure()));
    //connect(ui.startCalButton_Ex3, SIGNAL(clicked()), SLOT(Ex3_Procedure()));
    connect(ui.startBoot, SIGNAL(clicked()), SLOT(BootMode()));
    connect(ui.RefreshGPIO, SIGNAL(clicked()), SLOT(RefreshADIO()));

    //ui.Dec_LCD1->setNumDigits(12);

    connect(ui.led1, SIGNAL(clicked()), this, SLOT(led1Action()));
    connect(ui.led2, SIGNAL(clicked()), this, SLOT(led2Action()));
    connect(ui.led3, SIGNAL(clicked()), this, SLOT(led3Action()));
    connect(ui.led4, SIGNAL(clicked()), this, SLOT(led4Action()));
    connect(ui.led5, SIGNAL(clicked()), this, SLOT(led5Action()));
    connect(ui.led6, SIGNAL(clicked()), this, SLOT(led6Action()));
    //ui.qwtPlotA->replot();

    connect(ui.select1, SIGNAL(currentIndexChanged(int)), this, SLOT(selectIndex1(int)));
    connect(ui.select2, SIGNAL(currentIndexChanged(int)), this, SLOT(selectIndex2(int)));
    connect(ui.select3, SIGNAL(currentIndexChanged(int)), this, SLOT(selectIndex3(int)));
    connect(ui.select4, SIGNAL(currentIndexChanged(int)), this, SLOT(selectIndex4(int)));


    connect(this, SIGNAL(adio_data()),this,SLOT(adio_data_slot()));
    //ui.qwtPlotB->replot();
    memset(led_arr, '\0', sizeof(led_arr));

    mode1=0;
    mode2=0;
    mode3=0;
    mode4=0;
}


void ADIO_PIN::selectIndex1(int index)
{
    mode1=index;
    usGenPortConfig(0,mode1);
    //qDebug() << "Index = " << index; fflush(stdout); 
}

void ADIO_PIN::selectIndex2(int index)
{
    mode2=index;
    usGenPortConfig(1,mode2);
}
void ADIO_PIN::selectIndex3(int index)
{
    mode3=index;
    usGenPortConfig(2,mode3);
}

void ADIO_PIN::selectIndex4(int index)
{
    mode4=index;
    usGenPortConfig(3,mode4);
}


ADIO_PIN::~ADIO_PIN()
{
}

void ADIO_PIN::BootMode(void)
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

void ADIO_PIN::LED_Action(volatile int led_Num, volatile int xColor)
{
    switch (led_Num)
    {
    case 1:
        break;
    case 2:
        break;
    case 3:

        break;
    case 4:

        break;
    case 5:
        break;
    case 6:
        break;
    default:
        break;
    }

}

void ADIO_PIN::Banner_Action(int type_Text)
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

void ADIO_PIN::Winlog_Action(void)
{
}

void ADIO_PIN::UserIn(void)
{
}

void ADIO_PIN::slot_CSCRIPT_Extern(int i)
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
        ui.Dec_LCD1->display((int)lVal);
        ui.Dec_LCD2->display((int)lVal);
    }
    break;
    case CSCRIPT_LCD_HEX:
    {
        unsigned long  lVal;
        Pull_CSCRIPT(i, &lVal);
    }
    break;
    case CSCRIPT_LCD_BIN:
    {
        unsigned long  lVal;
        Pull_CSCRIPT(i, &lVal);
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
        //ui.MainBox_Msg->append(str);
    }
    break;
    case CSCRIPT_CLEAR_TEXT:
        //ui.MainBox_Msg->clear();
        break;
    default:
        break;

    }
}

void ADIO_PIN::Ex1_Procedure(void)
{
    RunCFile("cscript1.c");
}

void ADIO_PIN::Ex2_Procedure(void)
{
    RunCFile("cscript2.c");
}

void ADIO_PIN::Ex3_Procedure(void)
{
    RunCFile("cscript3.c");
}

void ADIO_PIN::led1Action()
{
    if (led_arr[1])
    {
        usLED_Off(1);
        ui.ledStatus->setText("OFF");
        ui.led1->setStyleSheet("background: gray");
    }
    else
    {
        ui.ledStatus->setText("ON");
        usLED_On(1);
        ui.led1->setStyleSheet("background: lightGreen");
    }
    led_arr[1] ^= 0x01;
}
void ADIO_PIN::led2Action()
{
    if (led_arr[2])
    {
        usLED_Off(2);
        ui.ledStatus->setText("OFF");
        ui.led2->setStyleSheet("background: gray");
    }
    else
    {
        ui.ledStatus->setText("ON");
        usLED_On(2);
        ui.led2->setStyleSheet("background: lightGreen");
    }
    led_arr[2] ^= 0x01;
}
void ADIO_PIN::led3Action()
{
    if (led_arr[3])
    {
        usLED_Off(3);
        ui.ledStatus->setText("OFF");
        ui.led3->setStyleSheet("background: gray");
    }
    else
    {
        ui.ledStatus->setText("ON");
        usLED_On(3);
        ui.led3->setStyleSheet("background: lightGreen");
    }
    led_arr[3] ^= 0x01;
}
void ADIO_PIN::led4Action()
{
    if (led_arr[4])
    {
        usLED_Off(4);
        ui.ledStatus->setText("OFF");
        ui.led4->setStyleSheet("background: gray");
    }
    else
    {
        ui.ledStatus->setText("ON");
        usLED_On(4);
        ui.led4->setStyleSheet("background: lightGreen");
    }
    led_arr[4] ^= 0x01;
}
void ADIO_PIN::led5Action()
{
    if (led_arr[5])
    {
        usLED_Off(5);
        ui.ledStatus->setText("OFF");
        ui.led5->setStyleSheet("background: gray");
    }
    else
    {
        ui.ledStatus->setText("ON");
        usLED_On(5);
        ui.led5->setStyleSheet("background: lightGreen");
    }
    led_arr[5] ^= 0x01;
}
void ADIO_PIN::led6Action()
{
    if (led_arr[6])
    {
        usLED_Off(6);
        ui.ledStatus->setText("OFF");
        ui.led6->setStyleSheet("background: gray");
    }
    else
    {
        ui.ledStatus->setText("ON");
        usLED_On(6);
        ui.led6->setStyleSheet("background: lightGreen");
    }
    led_arr[6] ^= 0x01;
}

void ADIO_PIN::adio_data_slot(void)
{
    usGenPortConfig(0,mode1);
    usGenPortConfig(1,mode2);
    usGenPortConfig(2,mode3);
    usGenPortConfig(3,mode4);

    //qDebug() << "mode1=" << mode1 << "\r\n";
    //qDebug() << "mode2=" << mode2 << "\r\n";
    //qDebug() << "mode3=" << mode3 << "\r\n";
    //qDebug() << "mode4=" << mode4 << "\r\n";
    //fflush(stdout);
    switch(mode1)
    {
        case 1:
            ui.Dec_LCD1->display((int)get_gen_val(0));
            break;
        case 2:
            break;
        default:
            if(get_gen_val(0))
            {
                ui.LED_1->setStyleSheet("background: lightGreen");
            }
            else
            {
                ui.LED_1->setStyleSheet("background: gray");
            }
            break;
    }

    switch(mode2)
    {
        case 1:
            ui.Dec_LCD2->display((int)get_gen_val(1));
            break;
        case 2:
            break;
        default:
            if(get_gen_val(1))
            {
                ui.LED_2->setStyleSheet("background: lightGreen");
            }
            else
            {
                ui.LED_2->setStyleSheet("background: gray");
            }
            break;
    }
    switch(mode3)
    {
        case 1:
            ui.Dec_LCD3->display((int)get_gen_val(2));
            break;
        case 2:
            break;
        default:
            if(get_gen_val(2))
            {
                ui.LED_3->setStyleSheet("background: lightGreen");
            }
            else
            {
                ui.LED_3->setStyleSheet("background: gray");
            }
            break;
    }
    switch(mode4)
    {
        case 1:
            ui.Dec_LCD4->display((int)get_gen_val(3));
            break;
        case 2:
            break;
        default:
            if(get_gen_val(3))
            {
                ui.LED_4->setStyleSheet("background: lightGreen");
            }
            else
            {
                ui.LED_4->setStyleSheet("background: gray");
            }
            break;
    }

    usADIOPort(0,0,0,0);
}

void ADIO_PIN::RefreshADIO()
{
    usGenPortConfig(0,mode1);
    usGenPortConfig(1,mode2);
    usGenPortConfig(2,mode3);
    usGenPortConfig(3,mode4);

    usADIOPort(0,0,0,0);
}
