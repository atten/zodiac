#include <QApplication>
#include <QFile>
#include <QDir>
//#include <QTextCodec>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Zodiac");
    a.setApplicationVersion("build 2014-03-24 alpha");

    //QTextCodec* codec = QTextCodec::codecForName ( "CP1251" );
    //QTextCodec::setCodecForCStrings ( codec );
    //QTextCodec::setCodecForTr ( codec );
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP1251"));

    QDir::setCurrent(a.applicationDirPath());

    if (QLocale::system().name().contains("ru", Qt::CaseInsensitive))
     {
      QDir dir("i18n");
      foreach (QString s, dir.entryList(QStringList("*.qm")))
       {
        QTranslator* t = new QTranslator;
        qDebug() << "load translation file" << s << ":"
                 << (t->load(dir.absolutePath() + '/' + s) && a.installTranslator(t) ? "success" : "failed");
       }
     }

    A::load();
    MainWindow w;

    QFile cssfile ( "style/style.css" );
    cssfile.open  ( QIODevice::ReadOnly | QIODevice::Text );
    w.setStyleSheet  ( cssfile.readAll() );

    w.show();
    return a.exec();
}
