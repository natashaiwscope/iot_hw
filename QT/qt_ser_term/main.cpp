#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "heartbeat.h"


int main(int argc, char *argv[])
{
#if 1
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(communicator);
    //QCoreApplication a(argc, argv);

    //CoAPComm *lCoAP;
    //lCoAP=new CoAPComm("10.38.181.174");
    HeartBeat hComm;
    //hComm=new HeartBeat();
    //usleep(100);
    //lCoAP->test_write();
    hComm.show();
    hComm.resize(1040, 640);

    app.exec();
    //while(1);

    //exec();

#else
    QApplication app(argc, argv);

    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(translator);

    Dialog dialog;
    return dialog.exec();
#endif

}
