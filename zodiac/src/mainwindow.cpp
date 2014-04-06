#include <QToolBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QListWidget>
#include <QKeyEvent>
#include <QMenu>
#include <QDir>
#include <QApplication>
#include <QComboBox>
#include <QDesktopServices>
#include <QGraphicsBlurEffect>
#include <QDebug>
#include "../plain/src/plain.h"
#include "../chart/src/chart.h"
#include "../fileeditor/src/fileeditor.h"
#include "../fileeditor/src/geosearch.h"
#include "../planets/src/planets.h"
#include "mainwindow.h"


/* =========================== ASTRO FILE INFO ====================================== */

AstroFileInfo :: AstroFileInfo (QWidget *parent) : AstroFileHandler(parent)
 {
  edit   = new QPushButton(this);
  shadow = new QLabel(this);
  QGraphicsBlurEffect* ef = new QGraphicsBlurEffect();

  ef->setBlurRadius(5);
  shadow->setGraphicsEffect(ef);
  edit->setFlat(true);
  edit->raise();
  edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  edit->setCursor(Qt::PointingHandCursor);
  setStatusTip(tr("Input data"));

  QGridLayout* layout = new QGridLayout(this);
   layout->addWidget(edit,0,0,1,1);
   layout->addWidget(shadow,0,0,1,1);

  connect(edit, SIGNAL(clicked()), this, SIGNAL(clicked()));

  resetToDefault();
 }

void AstroFileInfo :: resetToDefault()
 {
  setText(tr("No data represented."));
 }

void AstroFileInfo :: fileUpdated(AstroFile::Members)
 {
  QString date      = file()->getLocalTime().date().toString(Qt::DefaultLocaleShortDate);
  QString dayOfWeek = file()->getLocalTime().date().toString("ddd");
  QString time      = file()->getLocalTime().time().toString();

  QString timezone;
  if (file()->getTimezone() > 0)
    timezone = QString("GMT +%1").arg(file()->getTimezone());
  else if (file()->getTimezone() < 0)
    timezone = QString("GMT %1").arg(file()->getTimezone());
  else
    timezone = "GMT";

  QString place;
  if (file()->getLocationName().isEmpty())
   {
    QString longitude = A::degreeToString(file()->getLocation().x(), A::HighPrecision);
    QString latitude  = A::degreeToString(file()->getLocation().y(), A::HighPrecision);
    place = QString("%1N  %2E").arg(latitude, longitude);
   }
  else
   {
    place = file()->getLocationName();
   }


  setText( QString("%1\n")          .arg(file()->getName()) +
           QString("%1 %2 %3 (%4)\n").arg(date, dayOfWeek, time, timezone) +
           place );
 }

void AstroFileInfo :: setText(const QString& str)
 {
  edit->setText(str);
  shadow->setText(str);
 }


/* =========================== ASTRO WIDGET ========================================= */

AstroWidget :: AstroWidget(QWidget *parent) : QWidget(parent)
 {
  f = 0;
  editor = 0;

  toolBar = new QToolBar(tr("Slides"), this);
  actionGroup = new QActionGroup(this);

  geoWdg     = new GeoSearchWidget;
  slides     = new SlideWidget;
  fileView   = new AstroFileInfo;

  toolBar->setObjectName("slides");
  actionGroup->setExclusive(true);
  slides->setTransitionEffect(SlideWidget::Transition_HorizontalSlide);

  QGridLayout* layout = new QGridLayout(this);
   layout->setMargin(0);
   layout->addWidget(slides,   0,0, 1,1);
   layout->addWidget(fileView, 0,0, 1,1, Qt::AlignLeft|Qt::AlignTop);

  addSlide(new Chart,   QIcon("style/natal.png"),   tr("Chart"));
  addSlide(new Planets, QIcon("style/planets.png"), tr("Planets"));
  addSlide(new Plain,   QIcon("style/plain.png"),   tr("Text"));

  addHoroscopeControls();

  connect(fileView, SIGNAL(clicked()), this, SLOT(editCurrentFile()));
 }

void AstroWidget :: setupFile (AstroFile* file)
 {
  if (!file) return;
  bool hasChanges = file->hasUnsavedChanges();
  file->suspendUpdate();

  if (file->getGMT() == QDateTime::fromTime_t(0))  // set current date, time, timezone
   {
    QDateTime current = QDateTime::currentDateTime();
    QDateTime currentUTC = QDateTime(current.toUTC().date(), current.toUTC().time());
    file->setGMT(currentUTC);
    file->setTimezone(currentUTC.secsTo(current) / 3600);
   }

  if (file->getLocation().isNull())                // set default location
   {
    file->setLocation(geoWdg->location());
    file->setLocationName(geoWdg->locationName());
   }

  file->setZodiac      (zodiacSelector->itemData(zodiacSelector->currentIndex()).toInt());  // set zodiac
  file->setHouseSystem (hsystemSelector->itemData(hsystemSelector->currentIndex()).toInt()); // set house system
  file->setAspectLevel (levelSelector->itemData(levelSelector->currentIndex()).toInt());   // set aspect level


  if (!hasChanges) file->clearUnsavedState();
  file->resumeUpdate();
 }

void AstroWidget :: setFile (AstroFile* file)
 {
  f = file;
  setupFile(file);
  fileView->setFile(file);

  foreach (AstroFileHandler* h, handlers)
    h->setFile(file);
 }

void AstroWidget :: editCurrentFile()
 {
  if (editor)
   {
    editor->setFile(file());
    editor->raise();
   }
  else
   {
    editor = new AstroFileEditor();
    editor->setFile(file());
    editor->move((topLevelWidget()->width()  - editor->width())  / 2 + topLevelWidget()->geometry().left(),
                 (topLevelWidget()->height() - editor->height()) / 2 + topLevelWidget()->geometry().top());
    editor->show();
    connect(editor, SIGNAL(windowClosed()), this, SLOT(destroyEditor()));
   }
 }

void AstroWidget :: destroyEditor()
 {
  editor->deleteLater();
  editor = 0;
 }

void AstroWidget :: addSlide(AstroFileHandler* w, const QIcon& icon, QString title)
 {
  QAction* act = toolBar->addAction(icon, title, this, SLOT(toolBarActionClicked()));
  act->setCheckable(true);
  act->setActionGroup(actionGroup);
  if (!slides->count()) act->setChecked(true);

  slides->addSlide(w);
  handlers << w;

  connect(w, SIGNAL(requestHelp(QString)), this, SIGNAL(helpRequested(QString)));
 }

void AstroWidget :: addHoroscopeControls()
 {
  zodiacSelector  = new QComboBox;
  hsystemSelector = new QComboBox;
  levelSelector   = new QComboBox;

  zodiacSelector  -> setToolTip(tr("Sign"));
  hsystemSelector -> setToolTip(tr("House system"));
  levelSelector   -> setToolTip(tr("Aspects and orbs level\n(by A.Podvodny)"));

  foreach (const A::Zodiac& z, A::getZodiacs())
    zodiacSelector->addItem(z.name, z.id);                  // create combo box with zodiacs

  foreach (const A::HouseSystem& sys, A::getHouseSystems())
    hsystemSelector->addItem(sys.name, sys.id);             // create combo box with house systems

  foreach (const A::AspectLevel& l, A::getLevels())
    levelSelector->addItem(tr("%1 initiation").arg(A::romanNum(l)), l);  // create combo box with aspect levels

  horoscopeControls << zodiacSelector << hsystemSelector << levelSelector;

  foreach (QComboBox* c, horoscopeControls)
   {
    c->setEditable(false);
    connect (c, SIGNAL(currentIndexChanged(int)), this, SLOT(horoscopeControlChanged()));
   }
 }

void AstroWidget :: toolBarActionClicked()
 {
  QAction* s = (QAction*)sender();
  int i = toolBar->actions().indexOf(s);
  slides->setSlide(i);
 }

void AstroWidget :: applyGeoSettings(AppSettings& s)
 {
  s.setValue("Scope/defaultLocation",     vectorToString(geoWdg->location()));
  s.setValue("Scope/defaultLocationName", geoWdg->locationName());
 }

QString AstroWidget :: vectorToString (const QVector3D& v)
 {
  return QString("%1 %2 %3").arg(v.x()).arg(v.y()).arg(v.z());
 }

QVector3D AstroWidget :: vectorFromString (const QString& str)
 {
  QVector3D ret;
  ret.setX(str.section(" ", 0, 0).toFloat());
  ret.setY(str.section(" ", 1, 1).toFloat());
  ret.setZ(str.section(" ", 2, 2).toFloat());
  return ret;
 }

void AstroWidget :: horoscopeControlChanged()
 {
  setupFile(file());
 }

AppSettings AstroWidget :: defaultSettings ()
 {  
  AppSettings s;

  foreach (AstroFileHandler* h, handlers)
    s << h->defaultSettings();

  s.setValue("Scope/defaultLocation",     "37.6184 55.7512 0");
  s.setValue("Scope/defaultLocationName", "Moscow, Russia");
  s.setValue("Scope/zodiac",              0);          // indexes of ComboBox items, not values itself
  s.setValue("Scope/houseSystem",         0);
  s.setValue("Scope/level",               0);
  s.setValue("slide", slides->currentSlideIndex());    // чтобы не возвращалась к первому слайду после сброса настроек
  return s;
 }

AppSettings AstroWidget :: currentSettings ()
 { 
  AppSettings s;

  foreach (AstroFileHandler* h, handlers)
    s << h->currentSettings();

  applyGeoSettings(s);

  s.setValue("Scope/zodiac",      zodiacSelector  -> currentIndex());
  s.setValue("Scope/houseSystem", hsystemSelector -> currentIndex());
  s.setValue("Scope/level",       levelSelector   -> currentIndex());
  s.setValue("slide",             slides          -> currentSlideIndex());
  return s;
 }

void AstroWidget :: applySettings      ( const AppSettings& s )
 {
  geoWdg -> setLocation     ( vectorFromString(s.value("Scope/defaultLocation").toString()) );
  geoWdg -> setLocationName ( s.value("Scope/defaultLocationName").toString() );

  zodiacSelector  -> setCurrentIndex (s.value("Scope/zodiac").toInt());
  hsystemSelector -> setCurrentIndex (s.value("Scope/houseSystem").toInt());
  levelSelector   -> setCurrentIndex (s.value("Scope/level").toUInt());
  slides          -> setSlide        (s.value("slide").toInt() );
  toolBar         -> actions()[slides->currentSlideIndex()]->setChecked(true);

  setupFile(file());

  foreach (AstroFileHandler* h, handlers)
    h->applySettings(s);
 }

void AstroWidget :: setupSettingsEditor ( AppSettingsEditor* ed )
 {
  //ed->addTab(tr("Data"));

  ed->addCustomWidget(geoWdg, tr("Default location:"), SIGNAL(locationChanged()));

  foreach (AstroFileHandler* h, handlers)
    h->setupSettingsEditor(ed);

  connect(ed, SIGNAL(apply(AppSettings&)), this, SLOT(applyGeoSettings(AppSettings&)));
 }


/* =========================== FILES BAR ============================================ */

FilesBar :: FilesBar(QWidget *parent) : QTabBar(parent)
 {
  setTabsClosable(true);
  setMovable(true);

  connect(this, SIGNAL(tabMoved(int,int)),      this, SLOT(swapFiles(int,int)));
  connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
 }

void FilesBar :: addEmptyFile()
 {
  AstroFile* file = new AstroFile;
  addFile(file);
 }

void FilesBar :: addFile (AstroFile* file)
 {
  if (!file)
   {
    qWarning() << "FilesBar::addFile: failed to add an empty file";
    return;
   }

  files << file;
  file->setParent(this);
  addTab("new");
  updateTab(count() - 1);
  setCurrentIndex(count() - 1);

  connect(file, SIGNAL(changed(AstroFile::Members)), this, SLOT(updateTab()));
  connect(file, SIGNAL(destroyed()),                 this, SLOT(removeTab()));
 }

AstroFile* FilesBar :: fileAt (int index)
 {
  if (index < 0 || index >= count()) return 0;
  return files[index];
 }

void FilesBar :: updateTab(int index)
 {
  if (files.count() <= index || count() <= index) return;

  AstroFile* file = files[index];
  QString text = file->getName();

  if (file->hasUnsavedChanges())
    text += '*';

  setTabText(index, text);
 }

void FilesBar :: updateTab()
 {
  AstroFile* file = (AstroFile*)sender();
  int index = files.indexOf(file);
  updateTab(index);
 }

void FilesBar :: removeTab()    // called when AstroFile going to be destroyed
 {
  AstroFile* file = (AstroFile*)sender();
  int index = files.indexOf(file);
  files.removeAt(index);
  ((QTabBar*)this)->removeTab(index);
  if (!count()) addEmptyFile();
 }

void FilesBar :: swapFiles(int f1,int f2)
 {
  AstroFile* temp = files[f1];
  files[f1] = files[f2];
  files[f2] = temp;
 }

bool FilesBar :: closeTab(int index)
 {
  AstroFile* file = files[index];

  if (askToSave && file->hasUnsavedChanges())
   {
    QMessageBox msgBox;
    msgBox.setText(tr("Save changes in '%1' before closing?").arg(file->getName()));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    switch (ret)
     {
      case QMessageBox::Yes: file->save(); break;
      case QMessageBox::Cancel: return false;
      default: break;
     }
   }
  else if (count() == 1)
   {
    return false;                               // avoid to close last tab (without unsaved changes)
   }

  files.removeAt(index);
  ((QTabBar*)this)->removeTab(index);
  file->destroy();                              // delete AstroFile, because we do not need it
  if (!count()) addEmptyFile();
  return true;
 }

void FilesBar :: deleteFile(QString name)
 {
  foreach (AstroFile* file, files)
   {
    if (file->getName() == name)
     {
      file->destroy();
      return;
     }
   }
 }

void FilesBar :: openFile(QString name)
 {
  for (int i = 0; i < count(); i++)
   {
    if (files[i]->getName() == name)
     {
      setCurrentIndex(i);            // focus if the file is currently opened
      return;
     }
   }

  if (currentFile()->hasUnsavedChanges())
    openFileInNewTab(name);
  else
    currentFile()->load(name);

 }

void FilesBar :: openFileInNewTab(QString name)
 {
  for (int i = 0; i < count(); i++)
   {
    if (files[i]->getName() == name)
     {
      setCurrentIndex(i);            // if the file is currently opened, focus it
      return;
     }
   }

  AstroFile* file = new AstroFile;
  file->load(name);
  addFile(file);
 }


/* =========================== ASTRO FILE DATABASE ================================== */

AstroDatabase :: AstroDatabase (QWidget *parent) : QFrame(parent)
 {
  QPushButton* refresh = new QPushButton;

  fileList   = new QListWidget;
  search = new QLineEdit;

  refresh->setIcon(QIcon("style/update.png"));
  refresh->setToolTip(tr("Refresh"));
  refresh->setCursor(Qt::PointingHandCursor);
  fileList->setSelectionMode(QAbstractItemView::ExtendedSelection);
  search->setPlaceholderText(tr("Search"));
  setMinimumWidth(200);
  setContextMenuPolicy(Qt::CustomContextMenu);
  setWindowTitle(tr("Database"));
  setWindowFlags(Qt::WindowStaysOnTopHint);

  QHBoxLayout* l = new QHBoxLayout;
   l->addWidget(search);
   l->addWidget(refresh);

  QVBoxLayout* layout = new QVBoxLayout(this);
   layout->addLayout(l);
   layout->addWidget(fileList);


  connect(refresh, SIGNAL(clicked()), this, SLOT(updateList()));
  connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
  connect(fileList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openSelected()));
  connect(search, SIGNAL(textChanged(QString)), this, SLOT(searchFilter(QString)));

  updateList();
 }

void AstroDatabase :: searchFilter(QString s)
 {
  for (int i = 0; i < fileList->count(); i++)
   {
    QListWidgetItem* item = fileList->item(i);
    item->setHidden(!item->text().contains(s, Qt::CaseInsensitive));
   }
 }

void AstroDatabase :: updateList()
 {
  QDir dir("user/");

  QStringList list = dir.entryList(QStringList("*.dat"), QDir::Files, QDir::Name|QDir::IgnoreCase);
  list.replaceInStrings(".dat", "");

  QStringList selectedItems;
  foreach(QListWidgetItem* item, fileList->selectedItems())
   {
    if (item->isSelected()) selectedItems << item->text();
   }

  fileList->clear();

  foreach(QString itemString, list)
   {
    QListWidgetItem* item = new QListWidgetItem(itemString, fileList);
    if (selectedItems.contains(itemString))
      item->setSelected(true);
   }


  searchFilter(search->text());
 }

void AstroDatabase :: deleteSelected()
 {
  int count = fileList->selectedItems().count();
  if (!count) return;

  QMessageBox msgBox;
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Save);

  if (count == 1)
    msgBox.setText(tr("Delete '%1' from list?").arg(fileList->selectedItems()[0]->text()));
  else
    msgBox.setText(tr("Delete %1 files from list?").arg(count));


  int ret = msgBox.exec();
  if (ret == QMessageBox::Cancel) return;

  foreach (QListWidgetItem* item, fileList->selectedItems())
   {
    QString file = "user/" + item->text() + ".dat";
    QFile::remove(file);
    emit fileRemoved(item->text());
    fileList->removeItemWidget(item);
    delete item;
   }
 }

void AstroDatabase :: openSelected()
 {
  int count = fileList->selectedItems().count();
  if (!count) return;

  if (count == 1)
   emit openFile(fileList->selectedItems()[0]->text());
  else
   foreach (QListWidgetItem* item, fileList->selectedItems())
     emit openFileInNewTab(item->text());
 }

void AstroDatabase :: openSelectedInNewTab()
 {
  foreach (QListWidgetItem* item, fileList->selectedItems())
    emit openFileInNewTab(item->text());
 }

void AstroDatabase :: showContextMenu(QPoint p)
 {
  QMenu* mnu = new QMenu(this);
  QPoint pos = ((QWidget*)sender())->mapToGlobal(p);

  mnu -> addAction(tr("Open"), this, SLOT(openSelected()));
  mnu -> addAction(tr("Open in new tab"), this, SLOT(openSelectedInNewTab()));
  mnu -> addSeparator();
  mnu -> addAction(QIcon("style/delete.png"), tr("Delete"),this, SLOT(deleteSelected()));

  mnu -> exec(pos);
  mnu -> deleteLater();
 }

void AstroDatabase :: keyPressEvent(QKeyEvent* e)
 {
  if (e->key() == Qt::Key_Delete)
    deleteSelected();
 }

void AstroDatabase :: mousePressEvent(QMouseEvent* e)
 {
  if (e->button() == Qt::MiddleButton)
    openSelectedInNewTab();
 }


/* =========================== MAIN WINDOW ========================================== */

MainWindow :: MainWindow(QWidget *parent) : QMainWindow(parent), Customizable()
 {
  filesBar           = new FilesBar(this);
  astroWidget        = new AstroWidget(this);
  databaseDockWidget = new QDockWidget(this);
  astroDatabase      = new AstroDatabase();
  help               = new HelpWidget(this);
  toolBar            = new QToolBar(tr("File"),   this);
  toolBar2           = new QToolBar(tr("Options"),  this);

  toolBar            -> setObjectName("toolBar");
  toolBar2           -> setObjectName("toolBar2");
  help               -> setAlignment(Qt::AlignCenter);
  help               -> hide();
  databaseDockWidget -> setObjectName("dbDockWidget");
  databaseDockWidget -> setWidget(astroDatabase);
  databaseDockWidget -> setWindowTitle(tr("Database"));
  databaseDockWidget -> hide();
  this               -> setIconSize(QSize(48,48));
  this               -> setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  this               -> setWindowTitle(QApplication::applicationName());
  this               -> setMinimumHeight(480);


  QWidget* wdg = new QWidget(this);
  wdg->setObjectName("centralWidget");
  QGridLayout* layout = new QGridLayout(wdg);
   layout->setSpacing(0);
   layout->setContentsMargins(0,0,0,0);
   layout->addWidget(filesBar,    0,0, 1,1, Qt::AlignLeft);
   layout->addWidget(astroWidget, 1,0, 1,1);
   layout->addWidget(help,        1,0, 1,1, Qt::AlignBottom);

  setCentralWidget(wdg);
  addToolBarActions();
  addToolBar(Qt::TopToolBarArea, toolBar);
  addToolBar(Qt::TopToolBarArea, astroWidget->getToolBar());
  addToolBar(Qt::TopToolBarArea, toolBar2);
  addDockWidget(Qt::LeftDockWidgetArea, databaseDockWidget);
  foreach(QWidget* w, astroWidget->getHoroscopeControls())
    statusBar()->addPermanentWidget(w);

  connect(filesBar,      SIGNAL(currentChanged(int)),          this,     SLOT(currentFileChanged()));
  connect(astroDatabase, SIGNAL(openFile(QString)),            filesBar, SLOT(openFile(QString)));
  connect(astroDatabase, SIGNAL(openFileInNewTab(QString)),    filesBar, SLOT(openFileInNewTab(QString)));
  connect(astroDatabase, SIGNAL(fileRemoved(QString)),         filesBar, SLOT(deleteFile(QString)));
  connect(astroWidget,   SIGNAL(helpRequested(QString)),       help,     SLOT(searchFor(QString)));
  connect(statusBar(),   SIGNAL(messageChanged(QString)),      help,     SLOT(searchFor(QString)));
  //connect(databaseDockWidget, SIGNAL(visibilityChanged(bool)), this,     SLOT(dockVisibilityChanged(bool)));

  loadSettings();
  filesBar->addEmptyFile();
 }

void MainWindow        :: addToolBarActions   ( )
 {
  toolBar     -> addAction(QIcon("style/file.png"),  tr("New"),      this, SLOT(addNewFile()));
  toolBar     -> addAction(QIcon("style/save.png"),  tr("Save"),  this, SLOT(saveFile()));
  toolBar     -> addAction(QIcon("style/database.png"), tr("Open"));
  //toolBar     -> addAction(QIcon("style/print.png"), tr("Экспорт"));
  toolBar     -> addAction(QIcon("style/edit.png"),  tr("Edit"), astroWidget, SLOT(editCurrentFile()));

  toolBar     -> actions()[0]->setShortcut(QKeySequence("CTRL+N"));
  toolBar     -> actions()[1]->setShortcut(QKeySequence("CTRL+S"));
  //toolBar     -> actions()[3]->setShortcut(QKeySequence("CTRL+P"));
  toolBar     -> actions()[3]->setShortcut(QKeySequence("F2"));

  toolBar     -> actions()[0]->setStatusTip(tr("New data") + "\n Ctrl+N");
  toolBar     -> actions()[1]->setStatusTip(tr("Save data") + "\n Ctrl+S");
  toolBar     -> actions()[2]->setStatusTip(tr("Open data") + "\n Ctrl+O");
  //toolBar     -> actions()[3]->setStatusTip(tr("Печать или экспорт \n Ctrl+P"));
  toolBar     -> actions()[3]->setStatusTip(tr("Edit data...") + "\n F2");

  toolBar2    -> addAction(QIcon("style/tools.png"),  tr("Options"),       this, SLOT(showSettingsEditor()));
  toolBar2    -> addAction(QIcon("style/help.png"),   tr("Info +/-"));
  //toolBar2    -> addAction(QIcon("style/coffee.png"), tr("Справка"));
  toolBar2    -> addAction(QIcon("style/info.png"),   tr("About"), this, SLOT(showAbout()));

  databaseToggle = toolBar->actions()[2];
  databaseToggle->setShortcut(QKeySequence("CTRL+O"));
  databaseToggle->setCheckable(true);
  connect(databaseDockWidget, SIGNAL(visibilityChanged(bool)), databaseToggle,     SLOT(setChecked(bool)));
  connect(databaseToggle,     SIGNAL(triggered(bool)),         databaseDockWidget, SLOT(setVisible(bool)));

  helpToggle = toolBar2->actions()[1];
  helpToggle->setCheckable(true);
  connect(helpToggle, SIGNAL(toggled(bool)), help, SLOT(setVisible(bool)));
 }

void MainWindow        :: addNewFile          ( )
 {
  filesBar->addEmptyFile();
  //astroWidget->editCurrentFile();
 }

/*void MainWindow        :: dockVisibilityChanged ( bool b )
 {
  QDockWidget* dock = (QDockWidget*)sender();

  if (b && dock->isFloating())
   {
    dock->move(geometry().left() +
               centralWidget()->geometry().left() +
               (centralWidget()->width() - dock->width()) / 2,

               geometry().top() +
               centralWidget()->geometry().top() +
               (centralWidget()->height() - dock->height()) / 2);
   }

 }*/

AppSettings MainWindow :: defaultSettings     ( )
 {
  AppSettings s;
  s << astroWidget->defaultSettings();
  s.setValue ( "Window/Geometry",         0 );
  s.setValue ( "Window/State",            0 );
  s.setValue ( "help",                false );
  s.setValue ( "askToSave",           false );
  return s;
 }

AppSettings MainWindow :: currentSettings     ( )
 {
  AppSettings s;
  s << astroWidget->currentSettings();
  s.setValue ( "Window/Geometry",      this->saveGeometry() );
  s.setValue ( "Window/State",         this->saveState() );
  s.setValue ( "help",                 helpToggle->isChecked() );
  s.setValue ( "askToSave",            askToSave );
  return s;
 }

void MainWindow        :: applySettings       ( const AppSettings& s )
 {
  astroWidget->applySettings(s);
  this -> restoreGeometry   ( s.value ( "Window/Geometry" ).toByteArray() );
  this -> restoreState      ( s.value ( "Window/State" ).toByteArray() );
  helpToggle->setChecked    ( s.value ( "help" ).toBool() );
  askToSave = s.value ( "askToSave" ).toBool();
 }

void MainWindow        :: setupSettingsEditor ( AppSettingsEditor* ed )
 {
  ed->addControl("askToSave", tr("Ask about unsaved files"));
  //ed->addTab(tr("Other"));
  astroWidget->setupSettingsEditor(ed);
 }

/*void MainWindow        :: mouseDoubleClickEvent ( QMouseEvent* )
 {
  openSettingsEditor();
 }*/

void MainWindow        :: closeEvent          ( QCloseEvent* ev )
 { 
  while (askToSave && filesBar->count() && filesBar->fileAt(0)->hasUnsavedChanges())   // close tabs
    if (!filesBar->closeTab(0))
      return ev->ignore();

  if (databaseDockWidget->isFloating()) databaseDockWidget->hide();
  saveSettings();
 }

void MainWindow        :: gotoUrl             ( QString url )
 {
  QDesktopServices::openUrl(QUrl(url));
 }

void MainWindow        :: showAbout           ( )
 {
  QDialog* d = new QDialog(this);
  QLabel* l = new QLabel;
  QLabel* l2 = new QLabel;
  SlideWidget* s = new SlideWidget;
  QPushButton* b = new QPushButton(tr("Credits"));

  b->setCheckable(true);
  d->setObjectName("about");
  l->setWordWrap(true);
  l->setTextInteractionFlags(Qt::LinksAccessibleByMouse|
                             Qt::TextSelectableByMouse);
  l2->setWordWrap(true);
  l2->setTextInteractionFlags(Qt::LinksAccessibleByMouse|
                             Qt::TextSelectableByMouse);
  s->addSlide(l);
  s->addSlide(l2);
  s->setTransitionEffect(SlideWidget::Transition_Overlay);

  QVBoxLayout* v = new QVBoxLayout(d);
   v->setMargin(0);
   v->addWidget(s);
   v->addWidget(b);

  l->setText("<center><b><big>" + QApplication::applicationVersion() + "</big></b>"
          "<p>" + tr("Astrological software for personal use.") + "</p>"
          "<p>Latest news will be available at:<br>"
          "<a style='color:white' href=\"http://www.syslog.pro/tag/zodiac\">syslog.pro/tag/zodiac</a></p>"
          "<p>Copyright (C) 2012-2014 Artem Vasilev<br>"
          "<a style='color:white' href=\"mailto:atten@syslog.pro\">atten@syslog.pro</a></p><br>" +
          tr("This application is provided AS IS and distributed in the hope that it will be useful,"
                     " but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY"
                     " or FITNESS FOR A PARTICULAR PURPOSE.") + "</center>");


  l2->setText("<p><b>Swiss Ephemerides library</b><br>"
              "Copyright (C) 1997 - 2008 Astrodienst AG, Switzerland.  All rights reserved.<br>"
              "<a style='color:white' href=\"ftp://www.astro.ch/pub/swisseph/LICENSE\">ftp://www.astro.ch/pub/swisseph/LICENSE</a></p>"
              "<p><b>Primo Icon Set</b> by Webdesigner Depot<br>"
              "<a style='color:white' href=\"https://www.iconfinder.com/iconsets/Primo_Icons#readme\">www.iconfinder.com/iconsets/Primo_Icons#readme</a></p>"
              "<p><b>Almagest True Type Font</b></p>"
              "<p>Additional thanks to authors of <b>\"SymSolon\"</b> project<br>"
              "<a style='color:white' href=\"http://sf.net/projects/symsolon\">sf.net/projects/symsolon</a></p>");

  connect(l,  SIGNAL(linkActivated(QString)), this, SLOT(gotoUrl(QString)));
  connect(l2, SIGNAL(linkActivated(QString)), this, SLOT(gotoUrl(QString)));
  connect(b,  SIGNAL(clicked()), s, SLOT(nextSlide()));
  d->exec();
 }
