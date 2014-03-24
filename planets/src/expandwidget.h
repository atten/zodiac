#ifndef EXPANDWIDGET_H
#define EXPANDWIDGET_H

#include <QFrame>
#include <QPushButton>

class QPushButton;
class QVBoxLayout;
class QLayoutItem;


/* =========================== EXPAND WIDGET TITLE ================================== */

class Title : public QPushButton
{
    Q_OBJECT           // this class is made for calling 'Title' from CSS

    public:
        Title (const QString text, QWidget* parent = 0);
};


/* =========================== EXPAND WIDGET ======================================== */

class ExpandWidget : public QFrame
{
    Q_OBJECT

    private:
        int expandedSize;
        bool expanded;
        QList<QLayoutItem*> contents;

        QVBoxLayout* layout;
        Title* title;

        void constructor();

    private slots:
        void transitionDone();

    public slots:
        void setExpanded(bool exp) { if (exp) expand(); else collapse(); }

    public:
        ExpandWidget(QWidget *parent = 0);
        ExpandWidget(QString title, QWidget *parent = 0);

        void setTitle(QString t);
        void expand();
        void collapse();
        bool isExpanded() { return expanded; }

        void setWidget(QWidget*);
        void setLayout(QLayout*);
        void addWidget(QWidget*);
        void addLayout(QLayout*);
        void addItem(QLayoutItem*);
        void clear();
};

#endif // EXPANDWIDGET_H
