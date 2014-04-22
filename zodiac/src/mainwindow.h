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
        bool showAge;

        void setText(const QString& str);

    protected:                            // AstroFileHandler implementations
        void resetFile();
        void reset2ndFile() { }
        void fileUpdated(AstroFile::Members);
        void secondFileUpdated(AstroFile::Members) { }

    signals:
        void clicked();

    public:
        AstroFileInfo (QWidget *parent = 0);

        AppSettings defaultSettings ();
        AppSettings currentSettings ();
        void applySettings       ( const AppSettings& );
        void setupSettingsEditor ( AppSettingsEditor* );
};


/* =========================== ASTRO WIDGET ========================================= */

class AstroWidget : public QWidget
{
    Q_OBJECT

    private:
        AstroFile* f, *f2;

        AstroFileEditor*  editor;
        GeoSearchWidget*  geoWdg;
        QToolBar*         toolBar;
        QActionGroup*     actionGroup;
        SlideWidget*      slides;
        AstroFileInfo*    fileView, *fileView2nd;
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
        void currentSlideChanged();
        void horoscopeControlChanged();
        void destroyEditor();

    public slots:
        void openEditor();

    signals:
        void helpRequested(QString tag);

    public:
        AstroWidget(QWidget *parent = 0);
        QToolBar* getToolBar()      { return toolBar; }
        const QList<QComboBox*>& getHoroscopeControls() { return horoscopeControls; }

        void setFiles (AstroFile* file1, AstroFile* file2);
        //void set2ndFile (AstroFile* file);
        AstroFile* file()                     { return f; }
        AstroFile* secondFile()               { return f2; }

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
        QList<AstroFile*> secondFiles;

        void updateTab(int index);

    private slots:
        void swapFiles(int,int);
        void updateTab();
        void secondFileDestroyed();
        //void removeTab();

    public slots:
        void addNewFile() { addFile(new AstroFile); }
        void deleteFile(QString name);
        void openFile(QString name);
        void openFileInNewTab(QString name);
        void openFileAsSecond(QString name);
        void nextTab()            { setCurrentIndex((currentIndex() + 1) % count()); }
        bool closeTab(int);

    public:
        FilesBar(QWidget *parent = 0);

        void addFile(AstroFile* file);
        void setAskToSave(bool b) { askToSave = b; }
        AstroFile* fileAt(int index);
        AstroFile* currentFile()       { if (!count()) return 0; return files[currentIndex()]; }
        AstroFile* current2ndFile() { if (!count()) return 0; return secondFiles[currentIndex()]; }
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
        virtual bool eventFilter(QObject *, QEvent *);

    private slots:
        void showContextMenu(QPoint);
        void openSelected();
        void openSelectedInNewTab();
        void openSelectedAsSecond();
        void deleteSelected();
        void searchFilter(QString);

    public slots:
        void updateList();

    signals:
        void fileRemoved(QString);
        void openFile(QString);
        void openFileInNewTab(QString);
        void openFileAsSecond(QString);

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
        void saveFile()             { astroWidget->file()->save(); astroDatabase->updateList(); }
        void currentTabChanged();
        void showSettingsEditor()   { openSettingsEditor(); }
        void showAbout();
        void gotoUrl(QString url = "");

    protected:
        AppSettings defaultSettings ();      // 'Customizable' class implementations
        AppSettings currentSettings ();
        void applySettings        ( const AppSettings& );
        void setupSettingsEditor  ( AppSettingsEditor* );

        void closeEvent ( QCloseEvent* );

    public:
        MainWindow(QWidget *parent = 0);

};

#endif // MAINWINDOW_H
