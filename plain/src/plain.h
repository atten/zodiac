#ifndef PLAIN_H
#define PLAIN_H

#include <Astroprocessor/Gui>

class QCheckBox;
class QTextBrowser;


/* ================================== WIDGET ======================================== */

class Plain : public AstroFileHandler
{
    Q_OBJECT

    private:
        QCheckBox* describeInput;
        QCheckBox* describePlanets;
        QCheckBox* describeHouses;
        QCheckBox* describeAspects;
        QCheckBox* describePower;
        QTextBrowser* view;

    private slots:
        void redraw();

    protected:                            // AstroFileHandler implementations
        void resetToDefault();
        void fileUpdated(AstroFile::Members);
        void fileDestroyed()  { }

    public:
        Plain  ( QWidget* parent = 0 );
};


#endif // PLAIN_H
