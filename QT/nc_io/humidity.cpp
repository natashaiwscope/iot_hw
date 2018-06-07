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

#include "humidity.h"
#include "ext_udp.h"

HUMIDITY::HUMIDITY(QWidget *parent) : QWidget(parent)
{
    unsigned short j;

    ui.setupUi(this);

    isCont = 0;
    lastCmd =0;

}

void HUMIDITY::createPushButtons()
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

HUMIDITY::~HUMIDITY()
{
}
