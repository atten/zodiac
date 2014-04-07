#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabBar>
#include <QDockWidget>
#include <Astroprocessor/Gui>
#include "help.h"
#include "slidewidget.h"

class QListWidget;
class QLineEdit;
class QActionGroup;
class AstroFileEditor;
class GeoSearchWidget;
class QComboBox;

/* =========================== ASTRO FILE INFO ====================================== */

class AstroFileInfo : public AstroFileHandler
{
    Q_OBJECT

    private:
        QPushButton* edit;
        QLabel* shadow;

        void setText(const QString& str);

    protected:                            // AstroFileHandler implementations
        void resetToDefault();
        void fileUpdated(AstroFile::Members);
        void fileDestroyed()  { }

    signals:
        void clicked();

    public:
        AstroFileInfo (QWidget *parent = 0);
};


/* =========================== ASTRO WIDGET ========================================= */

class AstroWidget : public QWidget
{
    Q_OBJECT

    private:
        AstroFile* f;

        AstroFileEditor*  editor;
        GeoSearchWidget*  geoWdg;
        QToolBar*         toolBar;
        QActionGroup*     actionGroup;
        SlideWidget*      slides;
        AstroFileInfo*    fileView;
        QList<AstroFileHandler*> handlers;

        QComboBox* zodiacSelector;
        QComboBox* hsystemSelector;
        QComboBox* levelSelector;
        QList<QComboBox*> horoscopeControls;

        void setupFile (AstroFile* file);
        QString vectorToString (const QVector3D& v);
        QVector3D vectorFromString (const QString& str);

        void addHoroscopeControls();
        void addSlide(AstroFileHandler* w, const QIcon& icon, QString title);

    private slots:
        void applyGeoSettings(AppSettings&);
        void toolBarActionClicked();
        void horoscopeControlChanged();
        void destroyEditor();

    public slots:
        void editCurrentFile();

    signals:
        void helpRequested(QString tag);

    public:
        AstroWidget(QWidget *parent = 0);
        QToolBar* getToolBar()      { return toolBar; }
        const QList<QComboBox*>& getHoroscopeControls() { return horoscopeControls; }

        void setFile (AstroFile* file);
        AstroFile* file()                     { return f; }

        AppSettings defaultSettings ();
        AppSettings currentSettings ();
        void applySettings       ( const AppSettings& );
        void setupSettingsEditor ( AppSettingsEditor* );
};


/* =========================== FILES BAR ============================================ */

class FilesBar : public QTabBar
{
    Q_OBJECT

    private:
        bool askToSave;
        QList<AstroFile*> files;

        void updateTab(int index);

    private slots:
        void swapFiles(int,int);
        void updateTab();
        void removeTab();

    public slots:
        void deleteFile(QString name);
        void openFile(QString name);
        void openFileInNewTab(QString name);
        void nextTab()            { setCurrentIndex((currentIndex() + 1) % count()); }
        bool closeTab(int);

    public:
        FilesBar(QWidget *parent = 0);

        void addFile(AstroFile* file);
        void addEmptyFile();
        void setAskToSave(bool b) { askToSave = b; }
        AstroFile* fileAt(int index);
        AstroFile* currentFile()  { if (!count()) return 0; return files[currentIndex()]; }
        int count()               { return files.count(); }
};


/* =========================== ASTRO FILE DATABASE ================================== */

class AstroDatabase : public QFrame
{
    Q_OBJECT

    private:
        QListWidget* fileList;
        QLineEdit* search;

    protected:
        virtual void keyPressEvent(QKeyEvent*);
        virtual void mousePressEvent(QMouseEvent*);

    private slots:
        void showContextMenu(QPoint);
        void openSelected();
        void openSelectedInNewTab();
        void deleteSelected();
        void searchFilter(QString);

    public slots:
        void updateList();

    signals:
        void fileRemoved(QString);
        void openFile(QString);
        void openFileInNewTab(QString);

    public:
        AstroDatabase(QWidget *parent = 0);
};


/* =========================== MAIN WINDOW ========================================== */

class MainWindow : public QMainWindow, public Customizable
{
    Q_OBJECT

    private:
        bool askToSave;

        FilesBar*      filesBar;
        AstroWidget*   astroWidget;
        AstroDatabase* astroDatabase;
        HelpWidget*    help;
        QDockWidget*   databaseDockWidget;
        QToolBar*      toolBar;
        QToolBar*      toolBar2;
        QAction*       databaseToggle, *helpToggle;

        void addToolBarActions();

    private slots:
        void addNewFile();
        void saveFile()             { astroWidget->file()->save(); astroDatabase->updateList(); }
        void currentFileChanged()   { astroWidget->setFile(filesBar->currentFile()); }
        //void dockVisibilityChanged(bool);
        void showSettingsEditor()   { openSettingsEditor(); }
        void showAbout();
        void gotoUrl(QString url);

    protected:
        AppSettings defaultSettings ();      // 'Customizable' class implementations
        AppSettings currentSettings ();
        void applySettings        ( const AppSettings& );
        void setupSettingsEditor  ( AppSettingsEditor* );

        //void mouseDoubleClickEvent ( QMouseEvent* );
        void closeEvent ( QCloseEvent* );

    public:
        MainWindow(QWidget *parent = 0);

};

#endif // MAINWINDOW_H
