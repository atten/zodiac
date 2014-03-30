#ifndef SLIDEWIDGET_H
#define SLIDEWIDGET_H

#include <QWidget>

class QHBoxLayout;

/* =========================== SLIDE WIDGET ========================================= */

class SlideWidget : public QWidget
{
  Q_OBJECT

    public:
        enum Transitions { Transition_NoAnimation,
                           Transition_HorizontalSlide,
                           Transition_Overlay };

        SlideWidget(QWidget *parent = 0);

        void addSlide(QWidget* wdg, int number = -1);
        void setTransitionEffect(Transitions effect)  { this->effect = effect; }
        void setSlide(int number);
        void setSlide(QWidget* wdg);
        QWidget* currentSlide();
        int currentSlideIndex()                       { return currentIndex; }
        int count()                                   { return slides.count(); }

    public slots:
        void nextSlide();

    private slots:
        void transitionDone();

    private:
        QList<QWidget*> slides;
        Transitions effect;
        int currentIndex;
        QHBoxLayout* layout;
        bool animating;

        void slideSimple     (int index);
        void slideHorizontal (int index);
        void slideOverlay    (int index);
};

#endif // SLIDEWIDGET_H
