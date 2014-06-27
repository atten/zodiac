#ifndef HELP_H
#define HELP_H

#include <QMap>
#include <QLabel>
//#include <QFrame>
#include "slidewidget.h"


/* =========================== HELP ARTICLE ========================================= */

class HelpWidget : public QWidget
{
    Q_OBJECT

    private:
        struct Article {
            QString key;
            QString title;
            QString content;

            bool loaded;
            QString filePath;
            quint32 pos;
            quint16 length;

            Article() { loaded = false;
                        pos    = 0;
                        length = 0; }
        };

        QMap<QString, Article> articles;

        SlideWidget* slides;
        QLabel* title;
        QLabel* label1;
        QLabel* label2;

        QString getTag(QString searchStr);
        void loadArticles(quint16 maxArticleLength = -1);
        void setContent(Article& article);
        void setContent(const QString& title, const QString& text);
        void clear();

    public slots:
        void searchFor(QString string);      // or multiple keys divided by space

    signals:
        void contentChanged();

    public:
        HelpWidget(QWidget *parent = 0);
        QString currentTitle() { return title->text(); }
        void setAlignment(Qt::Alignment);

};

#endif // HELP_H
