#ifndef FILEEDITOR_H
#define FILEEDITOR_H

#include <Astroprocessor/Gui>

class QAction;
class QToolBar;
class QLineEdit;
class QComboBox;
class QDateTimeEdit;
class QDoubleSpinBox;
class GeoSearchWidget;
class QPlainTextEdit;


/* =========================== ASTRO FILE EDITOR ==================================== */

class AstroFileEditor : public AstroFileHandler
{
    Q_OBJECT

    private:
        int currentFile;

        QAction* editData1;
        QAction* editData2;
        QAction* addData2;
        QAction* delData2;
        QAction* swapFiles;

        QLineEdit* name;
        QComboBox* type;
        QDateTimeEdit* dateTime;
        QDoubleSpinBox* timeZone;
        GeoSearchWidget* geoSearch;
        QPlainTextEdit* comment;

        void update(AstroFile::Members);
        void set2ndFileEnabled(bool);

    protected:                            // AstroFileHandler implementations
        void filesUpdated(MembersList members);

        //virtual void showEvent(QShowEvent*);
        virtual void closeEvent(QCloseEvent*) { emit windowClosed(); }

    signals:
        void windowClosed();

    private slots:
        void remove2ndFile() { file(1)->destroy(); }
        void applyToFile();
        void timezoneChanged();

    public slots:
        void switchToFile1();
        void switchToFile2();

    public:
        AstroFileEditor(QWidget *parent = 0);

};


#endif // FILEEDITOR_H
