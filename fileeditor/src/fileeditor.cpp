#include <QLabel>
#include <QAction>
#include <QToolBar>
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
  currentFile1 = true;

  name              = new QLineEdit;
  type              = new QComboBox;
  dateTime          = new QDateTimeEdit;
  timeZone          = new QDoubleSpinBox;
  geoSearch         = new GeoSearchWidget;
  comment           = new QPlainTextEdit;

  QActionGroup* aGroup = new QActionGroup(this);
  QPushButton* ok     = new QPushButton(tr("OK"));
  QPushButton* cancel = new QPushButton(tr("Cancel"));
  QPushButton* apply  = new QPushButton(tr("Apply"));
  QToolBar* toolbar = new QToolBar;

  editData1  = toolbar->addAction(tr("Input"), this, SLOT(switchToFile1()));
  editData2  = toolbar->addAction(tr("Background"), this, SLOT(switchToFile2()));
  addData2   = toolbar->addAction("+");
  delData2   = toolbar->addAction("-", this, SLOT(remove2ndFile()));
  swapFiles  = toolbar->addAction(tr("Swap"));

  aGroup   -> setExclusive(true);
  editData1-> setCheckable(true);
  editData2-> setCheckable(true);
  editData1-> setChecked(true);
  editData1-> setActionGroup(aGroup);
  editData2-> setActionGroup(aGroup);
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
   layout->addWidget(toolbar, 0, Qt::AlignHCenter);
   layout->addLayout(lay1);
   layout->addLayout(buttons);


  connect(ok,       SIGNAL(clicked()), this, SLOT(applyToFile()));
  connect(apply,    SIGNAL(clicked()), this, SLOT(applyToFile()));
  connect(ok,       SIGNAL(clicked()), this, SLOT(close()));
  connect(cancel,   SIGNAL(clicked()), this, SLOT(close()));
  connect(timeZone, SIGNAL(valueChanged(double)), this, SLOT(timezoneChanged()));

  //resetFile();
  //reset2ndFile();
  //timezoneChanged();
  dateTime -> setFocus();
 }

void AstroFileEditor :: timezoneChanged()
 {
  if (timeZone->value() < 0)
    timeZone->setPrefix("");
  else
    timeZone->setPrefix("+");
 }

/*void AstroFileEditor :: resetFile()
 {
  name      -> setText("");
  type      -> setCurrentIndex(0);
  dateTime  -> setDateTime(QDateTime::currentDateTime());
  geoSearch -> setLocation(QVector3D(0,0,0),"");
  comment   -> clear();
 }*/

void AstroFileEditor :: update(AstroFile::Members m)
 {
  qDebug() << "AstroFileEditor: update file1:" << currentFile1;
  AstroFile* source = currentFile1 ? file() : secondFile();

  name      -> setText(source->getName());
  geoSearch -> setLocation(source->getLocation(), source->getLocationName());
  timeZone  -> setValue(source->getTimezone());
  dateTime  -> setDateTime(source->getLocalTime());
  comment   -> setPlainText(source->getComment());

  if (m & AstroFile::Type)
   {
    for (int i = 0; i < type->count(); i++)
      if (type->itemData(i) == source->getType())
        type->setCurrentIndex(i);
   }
 }

void AstroFileEditor :: fileUpdated(AstroFile::Members m)
 {
  if (currentFile1) update(m);
 }

void AstroFileEditor :: secondFileUpdated(AstroFile::Members m)
 {
  editData2->setVisible(true);
  delData2->setVisible(true);
  addData2->setVisible(false);
  swapFiles->setVisible(true);
  if (!currentFile1) update(m);
 }

void AstroFileEditor :: reset2ndFile()
 {
  editData2->setVisible(false);
  delData2->setVisible(false);
  addData2->setVisible(true);
  swapFiles->setVisible(false);
  switchToFile1();
 }

void AstroFileEditor :: switchToFile1()
 {
  if (currentFile1) return;
  currentFile1 = true;
  update(file()->diffFlags(secondFile()));
  editData1->setChecked(true);
 }

void AstroFileEditor :: switchToFile2()
 {
  if (!currentFile1) return;
  currentFile1 = false;
  update(file()->diffFlags(secondFile()));
  editData2->setChecked(true);
 }

void AstroFileEditor :: applyToFile()
 {
  //if (!file()) qDebug() << "wow, file() == 0!";// return;
  AstroFile* dst = currentFile1 ? file() : secondFile();

  dst->suspendUpdate();
  dst->setName(name->text());
  dst->setType((AstroFile::FileType)type->itemData(type->currentIndex()).toInt());
  dst->setLocationName(geoSearch->locationName());
  dst->setLocation(geoSearch->location());
  dst->setTimezone(timeZone->value());
  dst->setGMT(dateTime->dateTime().addSecs(timeZone->value() * -3600));
  dst->setComment(comment->document()->toPlainText());
  dst->resumeUpdate();
 }

void AstroFileEditor :: showEvent(QShowEvent* e)
 {
  AstroFileHandler::showEvent(e);
  if (!secondFile()) reset2ndFile();
 }
