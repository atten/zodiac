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
  currentFile = 0;

  tabs              = new QTabBar;
  addFileBtn        = new QPushButton(tr("+"));
  name              = new QLineEdit;
  type              = new QComboBox;
  dateTime          = new QDateTimeEdit;
  timeZone          = new QDoubleSpinBox;
  geoSearch         = new GeoSearchWidget;
  comment           = new QPlainTextEdit;

  QPushButton* ok     = new QPushButton(tr("OK"));
  QPushButton* cancel = new QPushButton(tr("Cancel"));
  QPushButton* apply  = new QPushButton(tr("Apply"));

  tabs     -> setTabsClosable(true);
  tabs     -> setMovable(true);
  addFileBtn -> setMaximumWidth(32);
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

  QHBoxLayout* lay4 = new QHBoxLayout;
   lay4->addWidget(tabs);
   lay4->addWidget(addFileBtn);

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
   layout->addLayout(lay4);
   layout->addLayout(lay1);
   layout->addLayout(buttons);


  connect(tabs,       SIGNAL(currentChanged(int)),    this, SLOT(currentTabChanged(int)));
  connect(tabs,       SIGNAL(tabMoved(int,int)),      this, SLOT(swapFilesSlot(int,int)));
  connect(tabs,       SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));
  connect(addFileBtn, SIGNAL(clicked()), this, SIGNAL(appendFile()));
  connect(ok,         SIGNAL(clicked()), this, SLOT(applyToFile()));
  connect(apply,      SIGNAL(clicked()), this, SLOT(applyToFile()));
  connect(ok,         SIGNAL(clicked()), this, SLOT(close()));
  connect(cancel,     SIGNAL(clicked()), this, SLOT(close()));
  connect(timeZone,   SIGNAL(valueChanged(double)), this, SLOT(timezoneChanged()));

  dateTime -> setFocus();
 }

void AstroFileEditor :: timezoneChanged()
 {
  if (timeZone->value() < 0)
    timeZone->setPrefix("");
  else
    timeZone->setPrefix("+");
 }

void AstroFileEditor :: updateTabs()
 {
  for (int i = 0; i < filesCount(); i++)
   {
    QString txt = file(i)->getName();
    if (tabs->count() <= i)
      tabs->addTab(txt);
    else
      tabs->setTabText(i, txt);
   }

  for (int i = filesCount(); i < tabs->count(); i++)    // remove unused tabs
    tabs->removeTab(i);

  addFileBtn->setVisible(filesCount() < 2);
 }

void AstroFileEditor :: update(AstroFile::Members m)
 {
  qDebug() << "AstroFileEditor: show file" << currentFile;
  AstroFile* source =  file(currentFile);

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

void AstroFileEditor :: currentTabChanged(int index)
 {
  if (currentFile == index) return;
  int oldFile = currentFile;
  currentFile = index;
  update(file(index)->diff(file(oldFile)));
 }

void AstroFileEditor :: removeTab(int index)
 {
  if (filesCount() < 2) return;
  //currentFile = 0;
  tabs->removeTab(index);
  file(index)->deleteLater();
 }

void AstroFileEditor :: swapFilesSlot(int i, int j)
 {
  currentFile = i;
  emit swapFiles(i, j);
 }

void AstroFileEditor :: setCurrentFile(int index)
 {
  tabs->setCurrentIndex(index);
 }

void AstroFileEditor :: filesUpdated(MembersList members)
 {
  updateTabs();
  if (!filesCount())
    close();
  else if (currentFile >= filesCount())
    setCurrentFile(0);
  else if(members[currentFile])
    update(members[currentFile]);
 }

void AstroFileEditor :: applyToFile()
 {
  AstroFile* dst = file(currentFile);

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

/*void AstroFileEditor :: showEvent(QShowEvent* e)
 {
  AstroFileHandler::showEvent(e);
  if (!secondFile()) reset2ndFile();
 }*/
