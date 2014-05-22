#ifndef FILEEDITOR_H
#define FILEEDITOR_H

#include <Astroprocessor/Gui>

class QLineEdit;
class QComboBox;
class QDateTimeEdit;
class QDoubleSpinBox;
class GeoSearchWidget;
class QPlainTextEdit;
class QTabBar;


/* =========================== ASTRO FILE EDITOR ==================================== */

class AstroFileEditor : public AstroFileHandler
{
    Q_OBJECT

    private:
        int currentFile;

        QTabBar* tabs;
        QPushButton* addFileBtn;
        QLineEdit* name;
        QComboBox* type;
        QDateTimeEdit* dateTime;
        QDoubleSpinBox* timeZone;
        GeoSearchWidget* geoSearch;
        QPlainTextEdit* comment;

        void update(AstroFile::Members);
        void updateTabs();

    protected:
        void filesUpdated(MembersList members);  // AstroFileHandler implementations
        //virtual void showEvent(QShowEvent*);
        virtual void closeEvent(QCloseEvent*) { emit windowClosed(); }

    signals:
        void windowClosed();
        void appendFile();
        void swapFiles(int, int);

    private slots:
        void swapFilesSlot(int, int);
        void currentTabChanged(int);
        void removeTab(int);
        void applyToFile();
        void timezoneChanged();

    public:
        AstroFileEditor(QWidget *parent = 0);
        void setCurrentFile(int index);

};


#endif // FILEEDITOR_H
