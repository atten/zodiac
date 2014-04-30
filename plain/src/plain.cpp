#include <QLabel>
#include <QFile>
#include <QCheckBox>
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QDebug>
#include <Astroprocessor/Output>
#include "plain.h"


/* ================================== WIDGET ======================================== */

Plain              :: Plain               ( QWidget* parent ) : AstroFileHandler (parent)
 {
  QLabel* label1  = new QLabel(tr("Show:"));
  describeInput   = new QCheckBox(tr("input;"));
  describePlanets = new QCheckBox(tr("planets;"));
  describeHouses  = new QCheckBox(tr("houses;"));
  describeAspects = new QCheckBox(tr("aspects;"));
  describePower   = new QCheckBox(tr("affetic"));
  view            = new QTextBrowser();

  describeInput   -> setChecked(false);
  describePlanets -> setChecked(true);
  describeHouses  -> setChecked(true);
  describeAspects -> setChecked(true);
  describePower   -> setChecked(false);

  describeInput   -> setStatusTip(tr("Show input data"));
  describePlanets -> setStatusTip(tr("Show planets"));
  describeHouses  -> setStatusTip(tr("Show houses"));
  describeAspects -> setStatusTip(tr("Show aspects"));
  describePower   -> setStatusTip(tr("Show dignity and deficient points for each planet"));

  QHBoxLayout* l = new QHBoxLayout();
    l->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding, QSizePolicy::Preferred));
    l->addWidget(label1);
    l->addWidget(describeInput);
    l->addWidget(describePlanets);
    l->addWidget(describeHouses);
    l->addWidget(describeAspects);
    l->addWidget(describePower);

  QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,5,0,0);
    layout->setSpacing(0);
    layout->addLayout(l);
    layout->addWidget(view);


  connect(describeInput,   SIGNAL(toggled(bool)), this, SLOT(refresh()));
  connect(describePlanets, SIGNAL(toggled(bool)), this, SLOT(refresh()));
  connect(describeHouses,  SIGNAL(toggled(bool)), this, SLOT(refresh()));
  connect(describeAspects, SIGNAL(toggled(bool)), this, SLOT(refresh()));
  connect(describePower,   SIGNAL(toggled(bool)), this, SLOT(refresh()));

  QFile cssfile ( "plain/style.css" );
  cssfile.open  ( QIODevice::ReadOnly | QIODevice::Text );
  setStyleSheet  ( cssfile.readAll() );
 }

void Plain         :: filesUpdated(MembersList m)
 {
  if (!file())
   {
    view->clear();
    return;
   }

  if (m[0] == 0) return;

  refresh();
 }

void Plain         :: refresh()
 {
  qDebug() << "Plain::refresh";
  int articles = (A::Article_Input   * describeInput->isChecked())   |
                 (A::Article_Planet  * describePlanets->isChecked()) |
                 (A::Article_Houses  * describeHouses->isChecked())  |
                 (A::Article_Aspects * describeAspects->isChecked())  |
                 (A::Article_Power   * describePower->isChecked());

  view->setText(A::describe(file()->horoscope(), (A::Article)articles));
 }

