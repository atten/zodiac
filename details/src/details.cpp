#include <QFile>
#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <Astroprocessor/Calc>
#include <Astroprocessor/Output>
#include "expandwidget.h"
#include "details.h"

Details :: Details(QWidget* parent) : AstroFileHandler(parent)
 {
  expandedAspects = false;
  planet          = A::Planet_None;
  fileIndex       = 0;

  planetSelector = new QComboBox;
  position       = new ExpandWidget(tr("Astronomic data"));
  aspects        = new ExpandWidget(tr("Aspects"));
  power          = new ExpandWidget(tr("Affetic"));
  positionLabel  = new QLabel;
  aspectsList    = new QListWidget;
  powerLabel     = new QLabel;

  planetSelector-> setIconSize(QSize(24,24));
  planetSelector-> setToolTip(tr("Select a planet"));
  planetSelector-> addItem(tr("Select a planet"));
  positionLabel -> setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
  powerLabel    -> setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);
  aspectsList   -> setIconSize(QSize(20,20));
  aspectsList   -> setAlternatingRowColors(true);
  aspectsList   -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  aspectsList   -> setMouseTracking(true);         // for help tips work
  positionLabel -> setWordWrap(true);
  powerLabel    -> setWordWrap(true);
  position      -> setWidget(positionLabel);
  aspects       -> setWidget(aspectsList);
  power         -> setWidget(powerLabel);

  QVBoxLayout* l2 = new QVBoxLayout(this);
    l2->setMargin(0);
    l2->addWidget(planetSelector, 0, Qt::AlignTop);
    l2->addWidget(position,       0, Qt::AlignTop);
    l2->addWidget(aspects,        0, Qt::AlignTop);
    l2->addWidget(power,          0, Qt::AlignTop);
    l2->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Preferred, QSizePolicy::Expanding));

  connect(aspectsList,    SIGNAL(clicked(QModelIndex)), this, SLOT(aspectClicked(QModelIndex)));
  connect(planetSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(planetSelected(int)));

  QFile cssfile ( "details/style.css" );
  cssfile.open  ( QIODevice::ReadOnly | QIODevice::Text );
  setStyleSheet  ( cssfile.readAll() );
 }

void Details :: describePlanet()
 {
  fileIndex = qBound(0, fileIndex, filesCount() - 1);

  bool exists = planet != A::Planet_None;
  positionLabel -> setVisible(exists);
  aspectsList   -> setVisible(exists);
  powerLabel    -> setVisible(exists);
  if (!exists) return;

  const A::Planet& p = file(fileIndex)->horoscope().planets[planet];


  QString powerText = A::describePowerInHtml(p, file(fileIndex)->horoscope());
  if (!powerText.isEmpty())
    powerText = "<p align='center'><b><i>" +
                QString("<font color='#71aeec' size='+2'>+%1</font> | "
                        "<font color='#dfb096' size='+2'>%2</font>")
                         .arg(p.power.dignity)
                         .arg(p.power.deficient) +
                "</i></b></p>" + powerText;

  positionLabel -> setText(A::describePlanetCoordInHtml(p));
  powerLabel    -> setText(powerText);


  aspectsList->clear();
  A::AspectList list;
  QString tag1, tag2;

  if (filesCount() == 1)
   {
    list = file()->horoscope().aspects;
    aspects->setTitle(tr("Aspects"));
   }
  else
   {
    list = calculateSynastryAspects();
    aspects->setTitle(tr("Synastry aspects"));
    tag1 = "#1";
    tag2 = "#2";
   }

  foreach (const A::Aspect& asp, list)
   {
    if (*asp.planet1 != p && *asp.planet2 != p) continue;

    QListWidgetItem* item = new QListWidgetItem;
    item->setIcon(QIcon(asp.d->userData["icon"].toString()));
    item->setText(A::describeAspect(asp));
    item->setToolTip(A::describeAspectFull(asp, tag1, tag2));
    item->setStatusTip(QString("%1+%2+%3").arg(asp.d->name)
                                          .arg(asp.planet1->name)
                                          .arg(asp.planet2->name));
    aspectsList->addItem(item);
   }

  QListWidgetItem* item = new QListWidgetItem("...");
  item->setToolTip(tr("more"));
  item->setWhatsThis("more");
  item->setTextAlignment(Qt::AlignCenter);
  aspectsList->addItem(item);

  if (expandedAspects)
    expandAspects();
  else
    updateListHeight(aspectsList);
 }

void Details :: aspectClicked (QModelIndex)
 {
  if (aspectsList->currentItem()->whatsThis() == "more")
   {
    if (!expandedAspects)
      expandAspects();
    else
      collapseAspects();

    aspectsList->clearSelection();
   }
 }

void Details :: planetSelected(int index)
 {
  if (index <= 0) return;
  A::PlanetId planetNum = planetSelector->itemData(index).toInt();
  setCurrentPlanet(planetNum, fileIndex);
 }

void Details :: setCurrentPlanet(A::PlanetId p, int file)
 {
  if (planet == p && fileIndex == file) return;
  planet = p;
  fileIndex = file;
  describePlanet();

  for (int i = 0; i < planetSelector->count(); i++)
    if (planetSelector->itemData(i) == p)
     {
      planetSelector->setCurrentIndex(i);
      return;
     }
 }

void Details :: expandAspects()
 {
  expandedAspects = true;
  aspectsList->item(aspectsList->count() - 1)->setText("^^^");

  const A::Planet& pl = file(fileIndex)->horoscope().planets[planet];
  const A::AspectsSet& set = file(fileIndex)->getAspetSet();

  foreach (const A::Planet& p, file(fileIndex)->horoscope().planets)
   {
    if (p == pl || A::aspect(p, pl, set) != A::Aspect_None) continue;

    A::Aspect asp = A::calculateAspect(set, pl, p);

    QListWidgetItem* item = new QListWidgetItem;
    item->setIcon(QIcon("images/aspects/none.png"));
    item->setText(A::describeAspect(asp));
    aspectsList->insertItem(aspectsList->count() - 1, item);
   }

  updateListHeight(aspectsList);
 }

void Details :: collapseAspects()
 {
  expandedAspects = false;
  aspectsList->item(aspectsList->count() - 1)->setText("...");

  int i = 0;
  while (i < aspectsList->count())
   {
    if (aspectsList->item(i)->toolTip().isEmpty())
      delete aspectsList->takeItem(i);
    else
      i++;
   }

  updateListHeight(aspectsList);
 }

void Details :: updateListHeight(QListWidget* w)
 {
  w->setFixedHeight(w->sizeHintForRow(0) * w->count());
 }

void Details :: clear()
 {
  planet = A::Planet_None;
  planetSelector->clear();
  planetSelector->hide();
 }

void Details :: filesUpdated(MembersList m)
 {
  if (!filesCount())
   {
    clear();
    return;
   }

  if (planetSelector->count() == 1)
   {
    foreach (const A::Planet& p, file(fileIndex)->horoscope().planets)
     {
      QString imgSrc = p.userData["icon"].toString();
      planetSelector->addItem(QIcon(imgSrc), p.name, p.id);
     }
   }

  if (m[0]) describePlanet();
 }

AppSettings Details :: defaultSettings ()
 {
  AppSettings s;
  s.setValue ( "Details/ShowPosition",  true );
  s.setValue ( "Details/ShowAspects",   true );
  s.setValue ( "Details/ShowPower",     false );
  return s;
 }

AppSettings Details :: currentSettings ()
 {
  AppSettings s;
  s.setValue ( "Details/ShowPosition",  position->isExpanded() );
  s.setValue ( "Details/ShowAspects",   aspects->isExpanded() );
  s.setValue ( "Details/ShowPower",     power->isExpanded() );
  return s;
 }

void Details :: applySettings ( const AppSettings& s )
 {
  position -> setExpanded  ( s.value ( "Details/ShowPosition"  ).toBool() );
  aspects  -> setExpanded  ( s.value ( "Details/ShowAspects"   ).toBool() );
  power    -> setExpanded  ( s.value ( "Details/ShowPower"     ).toBool() );
 }
