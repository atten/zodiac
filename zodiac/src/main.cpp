#include <QApplication>
#include <QFile>
#include <QDir>
#include <QTextCodec>
#include <QTranslator>
#include <QFontDatabase>
#include <QDebug>
#include "mainwindow.h"

//#define QT_NO_DEBUG_OUTPUT

void loadTranslations(QApplication* a, QString lang)
 {
  QDir dir("i18n");
  foreach (QString s, dir.entryList(QStringList("*" + lang + ".qm")))
   {
    QTranslator* t = new QTranslator;
    qDebug() << "load translation file" << s << ":"
                 << (t->load(dir.absolutePath() + '/' + s) ? "success" : "failed");
    a->installTranslator(t);
   }
 }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Zodiac");
    a.setApplicationVersion("v0.6.3 (build 2014-04-16)");

#if (QT_VERSION < QT_VERSION_CHECK(5, 2, 0))
    QTextCodec* codec = QTextCodec::codecForName ( "UTF-8" );
    QTextCodec::setCodecForCStrings ( codec );
    QTextCodec::setCodecForTr ( codec );
#endif

    QDir::setCurrent(a.applicationDirPath());
    QString lang = "";
    if (!a.arguments().contains("nolocale"))
     {
      if (QLocale::system().name().contains("ru", Qt::CaseInsensitive))
       {
        loadTranslations(&a, "ru");
        lang = "ru";
       }
      else
        loadTranslations(&a, "en");
     }

    QFontDatabase::addApplicationFont("fonts/Almagest.ttf");
    A::load(lang);
    MainWindow w;

    QFile cssfile ( "style/style.css" );
    cssfile.open  ( QIODevice::ReadOnly | QIODevice::Text );
    w.setStyleSheet  ( cssfile.readAll() );

    w.show();
    return a.exec();
}
