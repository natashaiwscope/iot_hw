/*****************************************************************************************
** Copyright (c) 2008 Pollak Inc.  All Rights Reserved
**
**   The information contained herein is the property of Pollak and is not
**   to be disclosed or used without prior written permission of Pollak Inc.
**   This copyright extends to all media in which this information may be preserved,
**   including magnetic storage, verbal, computer print-out or visual display.
**
** File    : mainApp.c
** Created : 12/24/2009
**
** Description:
**   This contains ...
**
** Contributors:
**   Vijayandra Singh
*****************************************************************************************/
#include <stdlib.h>
#include <math.h>
#include <Qt/qmainwindow.h>
#include <Qt/qapplication.h>
#include <Qt/qmainwindow.h>
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <qwt_math.h>
#include <qwt_plot_grid.h>
#include <qwt_counter.h>
#include <qaction.h>
#include <qmenu.h>
#include <qscrollarea.h>
#include <qscrollbar.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qlabel.h>

#include "modeselect.h"
#include "ext_udp.h"

ModeSelect::ModeSelect(QWidget *parent)
{
    scopeSingleChnl = new QAction(tr("&Scope"), this);
    scopeSingleChnl->setShortcut(tr("Ctrl+O"));

    signalSingleChnl = new QAction(tr("&Signal Generator"), this);
    signalSingleChnl->setShortcut(tr("Ctrl+S"));

    inCap0        = new QAction(tr("&Input Capture 1"), this);
    inCap0->setShortcut(tr("Ctrl+I,1"));

    inCap1        = new QAction(tr("&Input Capture 2"), this);
    inCap1->setShortcut(tr("Ctrl+I,2"));

    outCap0        = new QAction(tr("&Output Capture 1"), this);
    outCap0->setShortcut(tr("Ctrl+C,1"));

    outCap1        = new QAction(tr("&Output Capture 2"), this);
    outCap1->setShortcut(tr("Ctrl+C,2"));

    i2cMasterMode  = new QAction(tr("&I2C Master"), this);
    i2cMasterMode->setShortcut(tr("Alt+M"));

    i2cSlaveMode  = new QAction(tr("&I2C Slave"), this);
    i2cSlaveMode->setShortcut(tr("Alt+S"));


    // System Menu
    versionInfo = new QAction(tr("&Version Info"), this);
    versionInfo->setShortcut(tr("Ctrl+V"));

    exitNow = new QAction(tr("E&xit"), this);
    exitNow->setShortcut(tr("Ctrl+X"));

    //Play record menu

    sRecord = new QAction(tr("&Record"), this);
    sRecord->setShortcut(tr("Ctrl+R"));

    sPlay = new QAction(tr("&Play"), this);
    sPlay->setShortcut(tr("Ctrl+P"));

    ModeMenu = new QMenu(tr("&Mode"), this);
    ModeMenu->addAction(scopeSingleChnl);
    //ModeMenu->addAction(scopeDualChnl);
    ModeMenu->addAction(signalSingleChnl);
    //ModeMenu->addAction(signalDualChnl);
    ModeMenu->addAction(inCap0);
    ModeMenu->addAction(inCap1);
    ModeMenu->addAction(outCap0);
    ModeMenu->addAction(outCap1);
    ModeMenu->addAction(i2cMasterMode);
    ModeMenu->addAction(i2cSlaveMode);

    //menuBar->addMenu(ModeMenu);

    setPreference = new QAction(tr("Set Prefrerences"), this);
    setPreference->setShortcut(tr("Ctrl+W"));

    // Timing Menu
#if 0
    sigWaveFormChA = new QAction(tr("Select Waveform ChA"), this);
    sigWaveFormChA->setShortcut(tr("Ctrl+W"));

    sigWaveFormChB = new QAction(tr("Select Waveform ChB"), this);
    sigWaveFormChB->setShortcut(tr("Alt+W"));

    imPortWaveForm = new QAction(tr("Import From Excel"), this);
    imPortWaveForm->setShortcut(tr("Alt+X"));

    exPortWaveForm = new QAction(tr("Export To Excel"), this);
    exPortWaveForm->setShortcut(tr("Ctrl+X"));
#endif

    saveSettings = new QAction(tr("Save Settings"), this);
    saveSettings->setShortcut(tr("F10"));

    fileMenu = new QMenu(tr("&Options"), this);
    fileMenu->addAction(setGainA);
    fileMenu->addAction(setOffsetA);
    fileMenu->addAction(setGainB);
    fileMenu->addAction(setOffsetB);

    lookMenu = new QMenu(tr("&View"), this);
    lookMenu->addAction(setPreference);
    lookMenu->addAction(saveSettings);

    timingMenu = new QMenu(tr("&Timing"), this);
#if 0
    timingMenu->addAction(sigWaveFormChA);
    timingMenu->addAction(sigWaveFormChB);
    timingMenu->addAction(imPortWaveForm);
    timingMenu->addAction(exPortWaveForm);
#endif


    RecordPlayMenu = new QMenu(tr("&Special Feature"), this);
    RecordPlayMenu->addAction(sRecord);
    RecordPlayMenu->addSeparator();
    RecordPlayMenu->addAction(sPlay);

    systemMenu = new QMenu(tr("&System"), this);
    systemMenu->addAction(exitNow);
    systemMenu->addSeparator();
    systemMenu->addAction(versionInfo);
    //menuBar()->addMenu(fileMenu);
    //menuBar()->addMenu(fileMenu);
    //menuBar->addMenu(fileMenu);

}

void ModeSelect::AddMenuSelect(QMenuBar *mQMenuBarPtr)
{
    menuBar = mQMenuBarPtr;
    menuBar->addMenu(ModeMenu);
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(timingMenu);
    menuBar->addMenu(lookMenu);

    menuBar->addMenu(RecordPlayMenu);
    menuBar->addMenu(systemMenu);
}


ModeSelect::~ModeSelect()
{

}

void ModeSelect::SelectMode(unsigned char  val)
{

}


/****************************************************************************************
* $Log: $
*****************************************************************************************/

