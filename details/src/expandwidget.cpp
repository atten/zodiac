#include <QVBoxLayout>
#include <QPropertyAnimation>
#include "expandwidget.h"


/* =========================== EXPAND WIDGET TITLE ================================== */

Title :: Title (const QString text, QWidget *parent) : QPushButton(text, parent)
 {
 }


/* =========================== EXPAND WIDGET ======================================== */

ExpandWidget :: ExpandWidget(QWidget *parent) : QFrame(parent)
 {
  constructor();
 }

ExpandWidget :: ExpandWidget(QString title, QWidget *parent) : QFrame(parent)
 {
  constructor();
  setTitle(title);
 }

void ExpandWidget :: constructor()
 {
  expanded = true;
  expandedSize = -1;

  title = new Title(metaObject()->className());

  title->setCheckable(true);
  title->setChecked(true);

  layout = new QVBoxLayout(this);
   layout->setMargin(0);
   layout->setSpacing(0);
   layout->addWidget(title, 0, Qt::AlignTop);

  connect(title, SIGNAL(toggled(bool)), this, SLOT(setExpanded(bool)));
 }

void ExpandWidget :: setTitle(QString t)
 {
  title->setText(t);
 }

void ExpandWidget :: setWidget(QWidget* w)
 {
  clear();
  addWidget(w);
 }

void ExpandWidget :: setLayout(QLayout* l)
 {
  clear();
  addLayout(l);
 }

void ExpandWidget :: addWidget(QWidget* w)
 {
  layout->addWidget(w, 0, Qt::AlignTop);
  w->lower();
  contents.append(layout->itemAt(layout->count() - 1));
 }

void ExpandWidget :: addLayout(QLayout* l)
 {
  layout->addLayout(l);
  contents.append(layout->itemAt(layout->count() - 1));
 }

void ExpandWidget :: addItem(QLayoutItem* i)
 {
  layout->addItem(i);
  contents.append(layout->itemAt(layout->count() - 1));
 }

void ExpandWidget :: clear()
 {
  foreach (QLayoutItem* item, contents)
    layout->removeItem(item);

  contents.clear();
 }

void ExpandWidget :: expand()
 {
  if (expanded) return;

  QRect g = geometry();
  QRect g1 = g;
  g1.setHeight(expandedSize);

  if (!isVisible()) { setGeometry(g1); transitionDone(); return; } // skip animation if widget is hidden

  QPropertyAnimation* anim = new QPropertyAnimation(this, "geometry");
  anim->setDuration(g1.height());
  anim->setEasingCurve(QEasingCurve::OutQuad);
  anim->setStartValue(g);
  anim->setEndValue(g1);

  connect (anim, SIGNAL(finished()), this, SLOT(transitionDone()));

  anim->start(QAbstractAnimation::DeleteWhenStopped);
 }

void ExpandWidget :: collapse()
 {
  if (!expanded) return;

  QRect g = geometry();
  QRect g1 = g;
  g1.setHeight(title->height() + frameWidth() * 2);

  expandedSize = g.height();
  if (!isVisible()) { setGeometry(g1); transitionDone(); return; } // skip animation if widget is hidden

  QPropertyAnimation* anim = new QPropertyAnimation(this, "geometry");
  anim->setDuration(g.height());
  anim->setEasingCurve(QEasingCurve::OutQuad);
  anim->setStartValue(g);
  anim->setEndValue(g1);

  foreach (QLayoutItem* item, contents)
    layout->removeItem(item);


  connect (anim, SIGNAL(finished()), this, SLOT(transitionDone()));

  anim->start(QAbstractAnimation::DeleteWhenStopped);
 }

void ExpandWidget :: transitionDone()
 {
  expanded = !expanded;

  title->setChecked(expanded);

  foreach (QLayoutItem* item, contents)
    if (expanded)
      layout->addItem(item);
    else
      layout->removeItem(item);
 }
