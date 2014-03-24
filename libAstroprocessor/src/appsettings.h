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

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QMap>
#include <QString>
#include <QVariant>
#include <QDialog>

class QTabWidget;
class Customizable;
class QFormLayout;


/* =========================== APP SETTINGS ========================================= */

class AppSettings
{
    private:
        QMap<QString, QVariant> vals;

    public:
        AppSettings ();

        void setValue (QString name, QVariant value);
        const QVariant value (QString name, QVariant defaultValue = QVariant()) const;

        const QMap<QString, QVariant>& values() const { return vals; }
        void setValues (const AppSettings&);

        void load (QString fileName);
        void save (QString fileName);

        AppSettings& operator<< (const AppSettings& s) { this->setValues(s); return *this; }
};


/* =========================== APP SETTINGS EDITOR ================================== */

class AppSettingsEditor : public QDialog
{
    Q_OBJECT

    private:
        Customizable* customObj;
        AppSettings settings;
        AppSettings defaultSettings;  // нужен для восстановления значений, а также проверки типов
        QMap <QString, QWidget*> bindedControls;
        QList<QWidget*> customWidgets;  // widgets added by addCustomWidget() method
        bool changed;

        QTabWidget* tabs;
        QPushButton* ok;
        QPushButton* cancel;
        QPushButton* bApply;
        QPushButton* setDefault;

        void updateControls ();
        QFormLayout* lastLayout();

    private slots:
        void change ();         // помечает текущие настройки как несохранённые
        void applySettings ();
        void restoreDefaults ();

    signals:
        void apply (AppSettings&);

    public:
        AppSettingsEditor();
        ~AppSettingsEditor();

        void addTab    ( QString tabName );
        //void beginGroup  ( QString groupName );
        void endGroup    ( );
        void addSpacing  ( int spacing );
        void addControl  ( QString valueName, QString label );
        void addCustomWidget ( QWidget* wdg, QString label, const char* changeSignal );
        void addLineEdit ( QString valueName, QString label );
        void addCheckBox ( QString valueName, QString label );
        void addSpinBox  ( QString valueName, QString label, int minValue, int MaxValue );
        void addComboBox ( QString valueName, QString label, QMap<QString, QVariant> values);
        void addLabel    ( QString label );

        void setObject(Customizable* obj);

};


/* =========================== CUSTOMIZABLE CLASS =================================== */

class Customizable
{
    public:
        Customizable ();

        virtual AppSettings defaultSettings ()                    { return AppSettings(); }
        virtual AppSettings currentSettings ()                    { return AppSettings(); }
        virtual void applySettings ( const AppSettings& )         { }
        virtual void setupSettingsEditor  ( AppSettingsEditor* )  { }

        void loadSettings (QString iniFile = "settings.ini");
        void saveSettings (QString iniFile = "settings.ini");
        void openSettingsEditor();
};

#endif // APPSETTINGS_H
