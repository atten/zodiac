#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>
#include <QDebug>
#include "slidewidget.h"


/* =========================== SLIDE WIDGET ========================================= */

SlideWidget :: SlideWidget(QWidget *parent) : QWidget(parent)
 {
  currentIndex = -1;
  effect = Transition_HorizontalSlide;
  animating = false;

  layout = new QHBoxLayout(this);
  layout->setMargin(0);
 }

void SlideWidget :: addSlide(QWidget* wdg, int number)
 {
  if (wdg == 0) return;

  slides.removeOne(wdg);

  if (number >= 0)
    slides.insert(number, wdg);
  else
   slides << wdg;

  wdg->setParent(this);
  wdg->hide();

  if (slides.count() == 1) setSlide(wdg);
 }

void SlideWidget :: setSlide(QWidget* wdg)
 {
  setSlide(slides.indexOf(wdg));
 }

void SlideWidget :: setSlide(int number)
 {
  if (currentIndex == number || number < 0 || number >= slides.count())
    return;


  if (currentIndex < 0)
   {
    layout->addWidget(slides[number]);
    slides[number]->show();
   }
  else if (!isVisible())
   {
    slideSimple(number);
   }
  else
   {
    switch (effect)
     {
      case Transition_HorizontalSlide: slideHorizontal(number); break;
      case Transition_Overlay:         slideOverlay(number); break;
      default: slideSimple(number); break;
     }
   }

  currentIndex = number;
 }

void SlideWidget :: nextSlide()
 {
  int i = (currentSlideIndex() + 1) % count();
  setSlide(i);
 }

QWidget* SlideWidget :: currentSlide()
 {
  if (slides.count() < currentIndex || currentIndex < 0)
    return 0;

  return slides[currentIndex];
 }

void SlideWidget :: slideSimple(int index)
 {
  QWidget* lastWdg = slides[currentIndex];
  QWidget* newWdg  = slides[index];

  layout->removeWidget(lastWdg);
  layout->addWidget(newWdg);
  lastWdg->hide();
  newWdg->show();
 }

void SlideWidget :: slideHorizontal(int index)
 {
  QWidget* lastWdg = slides[currentIndex];
  QWidget* newWdg  = slides[index];

  layout->removeWidget(lastWdg);

  QRect leftArea;   leftArea.setSize(geometry().size()); leftArea.moveRight(0);
  QRect centerArea; centerArea.setSize(geometry().size());
  QRect rightArea;  rightArea.setSize(geometry().size()); rightArea.moveLeft(geometry().width());


  QPropertyAnimation* slide1 = new QPropertyAnimation(lastWdg, "geometry");
  QPropertyAnimation* slide2 = new QPropertyAnimation(newWdg, "geometry");

  int duration = geometry().width() * 0.5;
  slide1->setDuration(duration);
  slide2->setDuration(duration);

  slide1->setEasingCurve(QEasingCurve::OutQuad);
  slide2->setEasingCurve(QEasingCurve::OutQuad);

  connect (slide2, SIGNAL(finished()), this, SLOT(transitionDone()));


  if (index > currentIndex)   // | <---
   {
    if (!animating) slide1->setStartValue(centerArea);
    if (!animating) slide2->setStartValue(rightArea);
    slide1->setEndValue(leftArea);
    slide2->setEndValue(centerArea);
   }
  else                         // | --->
   {
    if (!animating) slide1->setStartValue(centerArea);
    if (!animating) slide2->setStartValue(leftArea);
    slide1->setEndValue(rightArea);
    slide2->setEndValue(centerArea);
   }


  animating = true;
  slide1->start(QAbstractAnimation::DeleteWhenStopped);
  slide2->start(QAbstractAnimation::DeleteWhenStopped);
  newWdg->show();
 }

void SlideWidget :: slideOverlay(int index)
 {
  QWidget* lastWdg = slides[currentIndex];
  QWidget* newWdg  = slides[index];

  layout->removeWidget(lastWdg);
  lastWdg->setGeometry(rect());
  layout->addWidget(newWdg);
  newWdg->show();

  QGraphicsOpacityEffect* ef1 = new QGraphicsOpacityEffect;
  QGraphicsOpacityEffect* ef2 = new QGraphicsOpacityEffect;

  lastWdg->setGraphicsEffect(ef1);
  newWdg->setGraphicsEffect(ef2);

  QPropertyAnimation* anim1 = new QPropertyAnimation(ef1, "opacity");
  QPropertyAnimation* anim2 = new QPropertyAnimation(ef2, "opacity");

  int duration = 200;
  anim1->setDuration(duration);
  anim2->setDuration(duration);

  anim1->setEasingCurve(QEasingCurve::InQuad);
  anim2->setEasingCurve(QEasingCurve::InQuad);

  connect (anim1, SIGNAL(finished()), this, SLOT(transitionDone()));
  connect (anim1, SIGNAL(finished()), ef1, SLOT(deleteLater()));
  connect (anim2, SIGNAL(finished()), ef2, SLOT(deleteLater()));

  anim1->setStartValue(1);
  anim2->setStartValue(0);
  anim1->setEndValue(0);
  anim2->setEndValue(1);

  animating = true;
  anim1->start(QAbstractAnimation::DeleteWhenStopped);
  anim2->start(QAbstractAnimation::DeleteWhenStopped);
 }

void SlideWidget :: transitionDone()
 {
  animating = false;
  layout->addWidget(slides[currentIndex]);          // ensure to show current slide

  foreach (QWidget* wdg, slides)
    if (wdg != slides[currentIndex]) wdg->hide();   // hide others
 }
