/*****************************************************************************************
** Copyright (c) 2008 Pollak Inc.  All Rights Reserved
**
**   The information contained herein is the property of Pollak and is not
**   to be disclosed or used without prior written permission of Pollak Inc.
**   This copyright extends to all media in which this information may be preserved,
**   including magnetic storage, verbal, computer print-out or visual display.
**
** File    : mainApp.h
** Created : 12/24/2009
**
** Description:
**   This defines a function
**
** Contributors:
**   Vijayandra Singh
**
*****************************************************************************************/

#ifndef MAINAPP_H_INCLUDED
#define MAINAPP_H_INCLUDED
#include <QtGui>
#include <qevent.h>
#if QT_VERSION < 0x040000
#include <qwhatsthis.h>
#endif
#include <stdlib.h>
#include <qlabel.h>
#include <qlabel.h>
#include <qlcdnumber.h>
#include <qthread.h>
#include <qmenubar.h>
#include <qaction.h>

QT_BEGIN_NAMESPACE
class QString;
class QAction;
class QLineEdit;
class QLabel;
class QPushButton;
class QGridLayout;
class QThread;
class QLCDNumber;
class QHBoxLayout;
QT_END_NAMESPACE

class ModeSelect : public QwtPlot
{
    Q_OBJECT

public:
    ModeSelect(QWidget * = NULL);
    ~ModeSelect();
    void AddMenuSelect(QMenuBar *mQMenuBarPtr);

public slots:

    void SelectMode(unsigned char val);

public:
    QMenu   *ModeMenu;
    QAction *scopeSingleChnl;
    QAction *scopeDualChnl;
    QAction *signalSingleChnl;
    QAction *signalDualChnl;
    QAction *inCap0;
    QAction *inCap1;
    QAction *outCap0;
    QAction *outCap1;
    QAction *i2cMasterMode;
    QAction *i2cSlaveMode;

    // File Menu
    QMenu   *systemMenu;
    QAction *versionInfo;
    QAction *exitNow;

    // File Menu
    QMenu   *fileMenu;
    QAction *setGainA;
    QAction *setGainB;
    QAction *setOffsetA;
    QAction *setOffsetB;

    // Timing,Trigger and Waveform
    QMenu   *timingMenu;
    QMenu   *lookMenu;
#if 0
    QAction *sigWaveFormChA;
    QAction *sigWaveFormChB;
    QAction *imPortWaveForm;
    QAction *exPortWaveForm;
#endif

    // Look Menu
    QAction *setPreference;

    QAction *editPlotFreq;
    QAction *editSignalFreq;
    QAction *editPlotCol0;
    QAction *editPlotCol1;
    QAction *editPlotTh0;
    QAction *editPlotTh1;
    QAction *saveSettings;

    QMenuBar  *menuBar;

    // Play record menu
    QMenu   *RecordPlayMenu;
    QAction *sRecord;
    QAction *sPlay;


public:
};


#endif /* MAINAPP_H_INCLUDED */

/****************************************************************************************
* $Log: $
*****************************************************************************************/

