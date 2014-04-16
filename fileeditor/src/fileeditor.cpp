#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QDebug>
#include "geosearch.h"
#include "fileeditor.h"


/* =========================== ASTRO FILE EDITOR ==================================== */

AstroFileEditor :: AstroFileEditor (QWidget *parent) : AstroFileHandler(parent)
 {
  name              = new QLineEdit;
  type              = new QComboBox;
  dateTime          = new QDateTimeEdit;
  timeZone          = new QDoubleSpinBox;
  geoSearch         = new GeoSearchWidget;
  comment           = new QPlainTextEdit;

  QPushButton* ok     = new QPushButton(tr("OK"));
  QPushButton* cancel = new QPushButton(tr("Cancel"));
  QPushButton* apply  = new QPushButton(tr("Apply"));


  type     -> addItem(tr("male"),      AstroFile::TypeMale);
  type     -> addItem(tr("female"),    AstroFile::TypeFemale);
  type     -> addItem(tr("undefined"), AstroFile::TypeOther);
  timeZone -> setRange(-12, 12);
  timeZone -> setDecimals(1);
  dateTime -> setCalendarPopup(true);
  comment  -> setMaximumHeight(70);
  this     -> setWindowTitle(tr("Edit entry"));
  this     -> setWindowFlags(Qt::Dialog |
                             Qt::MSWindowsFixedSizeDialogHint |
                             Qt::WindowStaysOnTopHint);

  QHBoxLayout* lay3 = new QHBoxLayout;
   lay3->addWidget(name);
   lay3->addWidget(new QLabel(tr("Gender:")));
   lay3->addWidget(type);

  QHBoxLayout* lay2 = new QHBoxLayout;
   lay2->addWidget(dateTime);
   lay2->addWidget(new QLabel(tr("Time zone:")));
   lay2->addWidget(timeZone);

  QFormLayout* lay1 = new QFormLayout;
   lay1->addRow(tr("Name:"),       lay3);
   lay1->addRow(tr("Local time:"), lay2);
   lay1->addRow(tr("Location:"),   geoSearch);
   lay1->addItem(new QSpacerItem(1,20));
   lay1->addRow(tr("Comment:"),    comment);

  QHBoxLayout* buttons = new QHBoxLayout;
   buttons->addSpacerItem(new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Preferred));
   buttons->addWidget(ok);
   buttons->addWidget(cancel);
   buttons->addWidget(apply);

  QVBoxLayout* layout = new QVBoxLayout(this);
   layout->addLayout(lay1);
   layout->addLayout(buttons);


  connect(ok,       SIGNAL(clicked()), this, SLOT(applyToFile()));
  connect(apply,    SIGNAL(clicked()), this, SLOT(applyToFile()));
  connect(ok,       SIGNAL(clicked()), this, SLOT(close()));
  connect(cancel,   SIGNAL(clicked()), this, SLOT(close()));
  connect(timeZone, SIGNAL(valueChanged(double)), this, SLOT(timezoneChanged()));

  resetToDefault();
  timezoneChanged();
  dateTime -> setFocus();
 }

void AstroFileEditor :: timezoneChanged()
 {
  if (timeZone->value() < 0)
    timeZone->setPrefix("");
  else
    timeZone->setPrefix("+");
 }

void AstroFileEditor :: resetToDefault()
 {
  name      -> setText("");
  type      -> setCurrentIndex(0);
  dateTime  -> setDateTime(QDateTime::currentDateTime());
  geoSearch -> setLocation(QVector3D(0,0,0),"");
  comment   -> clear();
 }

void AstroFileEditor :: fileUpdated(AstroFile::Members m)
 {
  name      -> setText(file()->getName());
  geoSearch -> setLocation(file()->getLocation(), file()->getLocationName());
  timeZone  -> setValue(file()->getTimezone());
  dateTime  -> setDateTime(file()->getLocalTime());
  comment   -> setPlainText(file()->getComment());

  if (m & AstroFile::Type)
   {
    for (int i = 0; i < type->count(); i++)
      if (type->itemData(i) == file()->getType())
        type->setCurrentIndex(i);
   }

 }

void AstroFileEditor :: applyToFile()
 {
  qDebug() << file();

  if (!file()) return;

  file()->suspendUpdate();

  // todo: time mismatch
  qDebug() << file()->getGMT().toTime_t()
           << dateTime->dateTime().addSecs(timeZone->value() * -3600).toTime_t()
           << dateTime->dateTime().toUTC().addSecs(timeZone->value() * -3600).toTime_t();

  file()->setName(name->text());
  file()->setType((AstroFile::FileType)type->itemData(type->currentIndex()).toInt());
  file()->setLocationName(geoSearch->locationName());
  file()->setLocation(geoSearch->location());
  file()->setTimezone(timeZone->value());
  file()->setGMT(dateTime->dateTime().addSecs(timeZone->value() * -3600));
  file()->setComment(comment->document()->toPlainText());

  file()->resumeUpdate();
 }
