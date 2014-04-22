#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QComboBox>
#include <QListWidget>
#include <QSplitter>
#include <QDeclarativeContext>
#include <QDeclarativeItem>
#include <Astroprocessor/Calc>
#include <QDir>
#include "expandwidget.h"
#include "planets.h"

/* =========================== ASTRO QML VIEW ======================================= */

AstroQmlView :: AstroQmlView (QWidget *parent) : QDeclarativeView(parent)
 {
  rootContext()->setContextProperty("view", this);       // enables calling setHelpTag() procedure from qml;
  setResizeMode(QDeclarativeView::SizeRootObjectToView); // prevent unnecessary resizing of widget
 }

void AstroQmlView :: fitSceneInView()
 {
  fitInView(QRect(QPoint(0,0), initialSize()), Qt::KeepAspectRatio);
 }

void AstroQmlView :: setSource(const QUrl& url)
 {
  ((QDeclarativeView*)this)->setSource(url);
  fitSceneInView();                             // update content size after loading scene
 }


/* =========================== ASTRO PLANETS SHOW =================================== */

Planets :: Planets(QWidget *parent) : AstroFileHandler(parent)
 {
  expandedAspects = false;
  selectedPlanet = A::Planet_None;

  QLabel* label1 = new QLabel(tr("Sort by:"));
  sortByPlanets  = new QRadioButton(tr("planet"));
  sortByHouses   = new QRadioButton(tr("house"));
  sortByPower    = new QRadioButton(tr("dignity"));
  sortByElement  = new QRadioButton(tr("element"));
  view           = new AstroQmlView;
  planetSelector = new QComboBox;
  position       = new ExpandWidget(tr("Astronomic data"));
  aspects        = new ExpandWidget(tr("Aspects"));
  power          = new ExpandWidget(tr("Affetic"));
  positionLabel  = new QLabel;
  aspectsList    = new QListWidget;
  powerLabel     = new QLabel;

  view          -> setAlignment(Qt::AlignTop);
  planetSelector-> setIconSize(QSize(24,24));
  planetSelector-> setToolTip(tr("Select a planet"));
  planetSelector-> addItem(tr("Select a planet"));
  sortByPlanets -> setChecked(true);
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

  QHBoxLayout* l1 = new QHBoxLayout();
    l1->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding, QSizePolicy::Preferred));
    l1->addWidget(label1);
    l1->addWidget(sortByPlanets);
    l1->addWidget(sortByHouses);
    l1->addWidget(sortByPower);
    l1->addWidget(sortByElement);
    l1->addSpacing(10);

  QScrollArea* area = new QScrollArea;
  QWidget* areaWidget = new QWidget;
  areaWidget->setObjectName("areaWidget");
  QVBoxLayout* l2 = new QVBoxLayout(areaWidget);
    area->setWidget(areaWidget);
    area->setWidgetResizable(true);
    l2->setMargin(0);
    l2->addWidget(planetSelector, 0, Qt::AlignTop);
    l2->addWidget(position,       0, Qt::AlignTop);
    l2->addWidget(aspects,        0, Qt::AlignTop);
    l2->addWidget(power,          0, Qt::AlignTop);
    l2->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Preferred, QSizePolicy::Expanding));

  QWidget* wdg = new QWidget;
  QVBoxLayout* l3 = new QVBoxLayout(wdg);
    l3->setContentsMargins(0,5,0,0);
    l3->addLayout(l1);
    l3->addWidget(view);

  splitter = new QSplitter(this);
    splitter->addWidget(wdg);
    splitter->addWidget(area);

  QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(splitter);

  connect(sortByPlanets,  SIGNAL(released()), this, SLOT(orderChanged()));
  connect(sortByHouses,   SIGNAL(released()), this, SLOT(orderChanged()));
  connect(sortByPower,    SIGNAL(released()), this, SLOT(orderChanged()));
  connect(sortByElement,  SIGNAL(released()), this, SLOT(orderChanged()));
  connect(aspectsList,    SIGNAL(clicked(QModelIndex)), this, SLOT(aspectClicked(QModelIndex)));
  connect(view,           SIGNAL(objectClicked(QString)), this, SLOT(planetClicked(QString)));
  connect(view,           SIGNAL(requestHelp(QString)), this, SIGNAL(requestHelp(QString)));  //  pass signal through
  connect(planetSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(planetSelected(int)));

  QFile cssfile ( "planets/style.css" );
  cssfile.open  ( QIODevice::ReadOnly | QIODevice::Text );
  setStyleSheet  ( cssfile.readAll() );
 }

void Planets :: drawPlanets()
 {
  if (view->source().isEmpty())
   {
    view->setSource(QUrl::fromLocalFile("planets/Planets.qml"));
    cardItems  = view->rootObject()->findChildren<QDeclarativeItem*>(QRegExp("card*"));   // select all items where
    labelItems = view->rootObject()->findChildren<QDeclarativeItem*>(QRegExp("label*"));  // objectName = "card..." and "label..."
   }

  A::PlanetList planets = file()->horoscope().planets.values();   // create list of planets
  while(planets.count() > cardItems.count()) planets.removeLast();
  A::sortPlanets(planets, order);

  int i = 0;
  foreach (QDeclarativeItem* item, cardItems)
   {
    if (i >= planets.count())
     {
      item->hide();                                  // ensure item hide
      continue;
     }

    const A::Planet& planet = planets[i];

    item->show();                                  // ensure item visible
    item->setProperty("objectName",   QString("card_%1").arg(planet.id));
    item->setProperty("title",        planet.name.toUpper());
    item->setProperty("house",        A::houseNum(planet));
    item->setProperty("sign",         planet.sign->id);
    item->setProperty("degreeStr",    A::zodiacPosition(planet,file()->horoscope().zodiac).toLower());
    item->setProperty("isRetro",      planet.eclipticSpeed.x() < 0);
    item->setProperty("dignityVal",   planet.power.dignity);
    item->setProperty("deficientVal", planet.power.deficient);
    item->setProperty("imageTip",     planet.name + "+" + planet.sign->name);
    item->setProperty("imageSignTip", planet.sign->name);
    item->setProperty("degreeTip",    QString("%1°").arg((int)planet.eclipticPos.x()));

    if (planet.houseRuler > 0)
     {
      item->setProperty("ruler",   tr("ruler of %1").arg(A::romanNum(planet.houseRuler)));
      item->setProperty("ruleTip",    planet.name + "+" + A::houseNum(planet) + "+" +
                                      "ruler" + A::romanNum(planet.houseRuler));
     }
    else
     {
      item->setProperty("ruler", "");
     }

    // TODO: check through all data and verify a result
    QString imgSrc = "file:///" + QDir::currentPath() + '/' + planet.userData["iconHuge"].toString();

    if (planet.id == A::Planet_Moon)
     {
      int sunAngle = A::angle(planet, file()->horoscope().sun);

      for (int i = 0; i < 360; i+=30)
       {
        int i2 = (i+30) % 360;

        if (i <= sunAngle && i2 > sunAngle)
         {
          if (qAbs(i-sunAngle) > qAbs(i2-sunAngle)) i = i2;
          imgSrc = imgSrc.arg(i);
         }
       }
     }

    item->setProperty("imageSrc", imgSrc);


    if (planet.id == selectedPlanet)      // highlight current planet
      item->setScale(1.07);
    else
      item->setScale(1);


    QStringList labels;
    QString tip;
    if (planet.id == A::almuten(file()->horoscope())->id)
     {
      labels << tr("Almuten");
      tip = planet.name + "+" + tr("Almuten");
     }
    if (planet.id == A::auriga(file()->horoscope())->id)
     {
      labels << tr("Auriga");
      tip = planet.name + "+" + tr("Auriga");
     }
    if (planet.id == A::doryphoros(file()->horoscope())->id)
     {
      labels << tr("Doryphoros");
      tip = planet.name + "+" + tr("Doryphoros");
     }
    labelItems[i]->setProperty("text", labels.join(", "));
    labelItems[i]->setProperty("helpTip", tip);

    i++;
   }
 }

void Planets :: describePlanet()
 {
  bool exists = selectedPlanet != A::Planet_None;

  positionLabel -> setVisible(exists);
  aspectsList   -> setVisible(exists);
  powerLabel    -> setVisible(exists);
  if (!exists) return;

  const A::Planet& p = file()->horoscope().planets[selectedPlanet];


  QString powerText = A::describePowerInHtml(p, file()->horoscope());
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
  foreach (const A::Aspect& asp, file()->horoscope().aspects)
   {
    if (asp.planet1->id != p.id && asp.planet2->id != p.id) continue;

    QListWidgetItem* item = new QListWidgetItem;
    item->setIcon(QIcon(asp.d->userData["icon"].toString()));
    item->setText(A::describeAspect(asp));
    item->setToolTip(A::describeAspectFull(asp));
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

void Planets :: fileUpdated(AstroFile::Members)
 { 
  if (planetSelector->count() == 1)                   // fill planet list
   {
    foreach (const A::Planet& p, file()->horoscope().planets)
     {
      QString imgSrc = p.userData["icon"].toString();
      planetSelector->addItem(QIcon(imgSrc), p.name, p.id);
     }
   }

  drawPlanets();
  describePlanet();
 }

void Planets :: orderChanged()
 {
  if (sortByPlanets->isChecked())
    order = A::Order_NoOrder;
  else if (sortByHouses->isChecked())
    order = A::Order_House;
  else if (sortByPower->isChecked())
    order = A::Order_Power;
  else if (sortByElement->isChecked())
    order = A::Order_Element;

  drawPlanets();
 }

void Planets :: planetSelected(int index)
 {
  if (index <= 0) return;
  int planetNum = planetSelector->itemData(index).toInt();
  setCurrentPlanet(planetNum);
 }

void Planets :: planetClicked(QString name)
 {
  int num = name.remove("card_").toInt();
  setCurrentPlanet((A::PlanetId)num);
 }

void Planets :: setCurrentPlanet(A::PlanetId id)
 {
  if (selectedPlanet == id) return;
  selectedPlanet = id;

  for (int i = 0; i < planetSelector->count(); i++)
    if (planetSelector->itemData(i) == (int)id)
     {
      planetSelector->setCurrentIndex(i);
      break;
     }

  QString objName = QString("card_%1").arg(id);
  foreach (QDeclarativeItem* item, cardItems)
   {
    if (item->objectName() == objName)
      item->setScale(1.07);
    else
      item->setScale(1);
   }

  describePlanet();
 }

void Planets :: aspectClicked (QModelIndex)
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

void Planets :: expandAspects()
 {
  expandedAspects = true;
  aspectsList->item(aspectsList->count() - 1)->setText("^^^");

  const A::Planet& planet = file()->horoscope().planets[selectedPlanet];
  A::AspectLevel level = file()->horoscope().inputData.level;

  foreach (const A::Planet& p, file()->horoscope().planets)
   {
    if (p.name == planet.name ||
        A::aspect(p, planet, level) != A::Aspect_None) continue;

    A::Aspect asp = A::calculateAspect(level, planet, p);

    QListWidgetItem* item = new QListWidgetItem;
    item->setIcon(QIcon("images/aspects/none.png"));
    item->setText(A::describeAspect(asp));
    aspectsList->insertItem(aspectsList->count() - 1, item);
   }

  updateListHeight(aspectsList);
 }

void Planets :: collapseAspects()
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

void Planets :: updateListHeight(QListWidget* w)
 {
  w->setFixedHeight(w->sizeHintForRow(0) * w->count());
 }

void Planets :: resetFile()
 {
  view->setSource(QUrl());
  selectedPlanet = A::Planet_None;
  planetSelector->clear();
  planetSelector->hide();
 }

AppSettings Planets :: defaultSettings ()
 {
  AppSettings s;
  s.setValue ( "PlanetsShow/SplitterState", 0 );
  s.setValue ( "PlanetsShow/ShowPosition",  true );
  s.setValue ( "PlanetsShow/ShowAspects",   true );
  s.setValue ( "PlanetsShow/ShowPower",     false );
  return s;
 }

AppSettings Planets :: currentSettings()
 {
  AppSettings s;
  s.setValue ( "PlanetsShow/SplitterState", splitter->saveState() );
  s.setValue ( "PlanetsShow/ShowPosition",  position->isExpanded() );
  s.setValue ( "PlanetsShow/ShowAspects",   aspects->isExpanded() );
  s.setValue ( "PlanetsShow/ShowPower",     power->isExpanded() );
  return s;
 }

void Planets :: applySettings (const AppSettings& s)
 {
  splitter -> restoreState ( s.value ( "PlanetsShow/SplitterState" ).toByteArray() );
  position -> setExpanded  ( s.value ( "PlanetsShow/ShowPosition"  ).toBool() );
  aspects  -> setExpanded  ( s.value ( "PlanetsShow/ShowAspects"   ).toBool() );
  power    -> setExpanded  ( s.value ( "PlanetsShow/ShowPower"     ).toBool() );
 }
