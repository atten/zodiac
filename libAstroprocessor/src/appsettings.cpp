/****************************************************************************
**
** Copyright (C) 2010 Artem Vasilev.
** Contact: atten@coolline.ru
**
** This file is part of the FreeGate.
**
** FreeGate is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** FreeGate is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with FreeGate.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include <QObject>
#include <QStringList>
#include <QSettings>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QTabWidget>
#include <QFormLayout>
#include <QTextCodec>
#include <QDebug>
#include "appsettings.h"

/* =========================== APP SETTINGS ========================================= */

AppSettings :: AppSettings()
 {
 }

void AppSettings :: setValue (QString name, QVariant value)
 {
  vals[name] = value;
 }

void AppSettings :: setValues (const AppSettings& s)
 {
  QMapIterator<QString, QVariant> i(s.values());
  while (i.hasNext())
   {
    i.next();
    setValue(i.key(), i.value());
   }
 }

const QVariant AppSettings :: value (QString name, QVariant defaultValue) const
 {
  if (vals.contains(name))
   {
    return vals[name];
   }
  else
   {
    return defaultValue;
   }
 }

void AppSettings :: load (QString fileName)
 {
  QSettings file (fileName, QSettings::IniFormat);
  file.setIniCodec(QTextCodec::codecForName ("UTF-8"));

  foreach (QString key, vals.keys())
    setValue (key, file.value(key, value(key)));

  qDebug() << "AppSettings: loaded from" << fileName;
 }

void AppSettings :: save (QString fileName)
 {
  QSettings file (fileName, QSettings::IniFormat);
  file.setIniCodec(QTextCodec::codecForName ("UTF-8"));

  foreach (QString key, vals.keys())
    file.setValue (key, value(key));

  qDebug() << "AppSettings: saved to" << fileName;
 }


/* =========================== APP SETTINGS EDITOR ================================== */

AppSettingsEditor :: AppSettingsEditor() : QDialog()
 {
  changed = false;

  tabs       = new QTabWidget  (this);
  ok         = new QPushButton (tr("OK"), this);
  cancel     = new QPushButton (tr("Cancel"), this);
  bApply     = new QPushButton (tr("Apply"), this);
  setDefault = new QPushButton (tr("By default"), this);

  setWindowTitle (tr("Settings"));
  bApply->setEnabled(false);

  QHBoxLayout* buttLayout = new QHBoxLayout;
    buttLayout -> addWidget (setDefault);
    buttLayout -> addStretch(0);
    buttLayout -> addSpacing(10);
    buttLayout -> addWidget (ok);
    buttLayout -> addWidget (cancel);
    buttLayout -> addWidget (bApply);

  QVBoxLayout* layout = new QVBoxLayout(this);
    layout -> addWidget ( tabs );
    layout -> addLayout ( buttLayout );

  connect (bApply,     SIGNAL(clicked()), this, SLOT(applySettings()));
  connect (ok,         SIGNAL(clicked()), this, SLOT(applySettings()));
  connect (ok,         SIGNAL(clicked()), this, SLOT(accept()));
  connect (cancel,     SIGNAL(clicked()), this, SLOT(reject()));
  connect (setDefault, SIGNAL(clicked()), this, SLOT(restoreDefaults()));
 }

void AppSettingsEditor :: setObject(Customizable* obj)
 {
  customObj       = obj;
  settings        = obj->currentSettings();
  defaultSettings = obj->defaultSettings();
 }

void AppSettingsEditor :: change ()
 {
  changed = true;
  bApply->setEnabled(true);
 }

void AppSettingsEditor :: addTab    ( QString tabName )
 {
  QWidget* w = new QWidget;
  w->setLayout(new QFormLayout);
  tabs->addTab(w, tabName);
 }

/*void AppSettingsEditor :: beginGroup  ( QString groupName )
 {
  if (!tabs->count()) addTab(tr("General"));

  // ...
 }*/

QFormLayout* AppSettingsEditor :: lastLayout()
 {
  if (!tabs->count()) addTab(tr("General"));
  int last = tabs->count()-1;
  return (QFormLayout*)tabs->widget(last)->layout();
 }

void AppSettingsEditor :: addControl  ( QString valueName, QString label )
 {
  if (valueName.isEmpty()) return;

  QVariant ds = defaultSettings.value(valueName);

  if (ds.type() == QVariant::Int)
   {
    addLineEdit(valueName, label);
   }
  else if (ds.type() == QVariant::Bool)
   {
    addCheckBox(valueName, label);
   }
  else if (ds.type() == QVariant::String)
   {
    addLineEdit(valueName, label);
   }
  else
   {
    qDebug("AppSettingsEditor: can't add control: unsupported control type `%s`", ds.typeName());
   }

 }

void AppSettingsEditor :: addCustomWidget ( QWidget* wdg, QString label, const char* changeSignal )
 {                                     // add a user widget (manual reading and applying settings is required)
  customWidgets << wdg;
  lastLayout()->addRow(label, wdg);
  connect(wdg, changeSignal, this, SLOT(change()));
 }

void AppSettingsEditor :: addLineEdit ( QString valueName, QString label )
 {
  if (valueName.isEmpty()) return;
  QVariant s = settings.value(valueName);

  QLineEdit* edit = new QLineEdit ( s.toString() );
  lastLayout()->addRow(label, edit);

  bindedControls[valueName] = edit;
  connect (edit, SIGNAL(textChanged(QString)), this, SLOT(change()));
 }

void AppSettingsEditor :: addCheckBox ( QString valueName, QString label )
 {
  if (valueName.isEmpty()) return;
  QVariant s = settings.value(valueName);

  QCheckBox* edit = new QCheckBox;
  edit -> setChecked (s.toBool());
  lastLayout()->addRow(label, edit);

  bindedControls[valueName] = edit;
  connect (edit, SIGNAL(toggled(bool)), this, SLOT(change()));
 }

void AppSettingsEditor :: addSpinBox  ( QString valueName, QString label, int minValue, int MaxValue )
 {
  if (valueName.isEmpty()) return;
  QVariant s = settings.value(valueName);

  QSpinBox* edit = new QSpinBox;
  edit->setMinimum(minValue);
  edit->setMaximum(MaxValue);
  edit->setValue(s.toInt());
  edit->setMaximumWidth(70);

  lastLayout()->addRow(label, edit);

  bindedControls[valueName] = edit;
  connect (edit, SIGNAL(valueChanged(int)), this, SLOT(change()));
 }

void AppSettingsEditor :: addComboBox ( QString valueName, QString label, QMap<QString, QVariant> values)
 {
  if (valueName.isEmpty()) return;
  QVariant s = settings.value(valueName);

  QComboBox* edit = new QComboBox;

  edit->setEditable(false);
  edit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  int i = 0;
  foreach (QString str, values.keys())
   {
    edit->addItem(str, values[str]);
    if (values[str] == s) edit->setCurrentIndex(i);
    i++;
   }

  lastLayout()->addRow(label, edit);

  bindedControls[valueName] = edit;
  connect (edit, SIGNAL(currentIndexChanged(int)), this, SLOT(change()));
 }

void AppSettingsEditor :: addLabel    ( QString label )
 {
  if (label.isEmpty()) return;
  QLabel* l = new QLabel(label);
  lastLayout()->addWidget(l);
 }

void AppSettingsEditor :: addSpacing  ( int spacing )
 {
  lastLayout()->addItem(new QSpacerItem(1, spacing, QSizePolicy::Fixed, QSizePolicy::Fixed));
 }

void AppSettingsEditor :: applySettings ()
 {
  if (!changed) return;

  QMap<QString, QWidget*>::iterator i = bindedControls.begin();

  while (i != bindedControls.end())
   {
    QWidget* w = i.value();
    QString className = w->metaObject()->className();
    QVariant value;

    if (className == "QLineEdit")
      value = ((QLineEdit*)w)->text();

    else if (className == "QCheckBox")
      value = ((QCheckBox*)w)->isChecked();

    else if (className == "QSpinBox")
      value = ((QSpinBox*)w)->value();

    else if (className == "QComboBox")
      value = ((QComboBox*)w)->itemData(((QComboBox*)w)->currentIndex());

    settings.setValue(i.key(), value);
    i++;
   }

  changed = false;
  bApply->setEnabled(false);
  emit apply(settings);
  customObj->applySettings(settings);
 }

void AppSettingsEditor :: updateControls ()
 {
  QMap<QString, QWidget*>::iterator i = bindedControls.begin();

  while (i != bindedControls.end())
   {
    QWidget* w = i.value();
    QString className = w->metaObject()->className();
    QVariant value = settings.value(i.key());

    if (className == "QLineEdit")
      ((QLineEdit*)w)->setText(value.toString());

    else if (className == "QCheckBox")
      ((QCheckBox*)w)->setChecked(value.toBool());

    else if (className == "QSpinBox")
      ((QSpinBox*)w)->setValue(value.toInt());

    else if (className == "QComboBox")
      ((QComboBox*)w)->setCurrentIndex(((QComboBox*)w)->findData(value));

    i++;
   }
 }

void AppSettingsEditor :: restoreDefaults ()
 {
  settings = defaultSettings;
  changed = true;
  bApply->setEnabled(true);
  updateControls();
 }

AppSettingsEditor :: ~AppSettingsEditor()
 {
  foreach (QWidget* wdg, customWidgets)
   {
    wdg->setParent(this->parentWidget()); // deattach another widgets before deleting
   }
 }



/* =========================== CUSTOMIZABLE CLASS =================================== */

Customizable :: Customizable()
 {
 }

void Customizable :: openSettingsEditor()
 {
  AppSettingsEditor* ed = new AppSettingsEditor();
  ed->setObject(this);
  setupSettingsEditor(ed);

  ed->exec();
  ed->deleteLater();
 }

void Customizable :: saveSettings (QString iniFile)
 {
  currentSettings().save(iniFile);
 }

void Customizable :: loadSettings(QString iniFile)
 {
  AppSettings s = defaultSettings();
  s.load( iniFile );
  applySettings(s);
 }
