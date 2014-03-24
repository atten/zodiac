#ifndef FILEEDITOR_H
#define FILEEDITOR_H

#include <Astroprocessor/Gui>

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
        QLineEdit* name;
        QComboBox* type;
        QDateTimeEdit* dateTime;
        QDoubleSpinBox* timeZone;
        GeoSearchWidget* geoSearch;
        QPlainTextEdit* comment;

    protected:                            // AstroFileHandler implementations
        void resetToDefault();
        void fileUpdated(AstroFile::Members);
        void fileDestroyed() { close(); }

        virtual void closeEvent(QCloseEvent*) { emit windowClosed(); }

    private slots:
        void applyToFile();
        void timezoneChanged();

    signals:
        void windowClosed();

    public:
        AstroFileEditor(QWidget *parent = 0);

};


#endif // FILEEDITOR_H
