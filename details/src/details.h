#ifndef DETAILS_H
#define DETAILS_H

#include <QModelIndex>
#include <Astroprocessor/Gui>

class ExpandWidget;
class QComboBox;
class QLabel;
class QListWidget;


class Details : public AstroFileHandler
{
    Q_OBJECT

    private:
        A::PlanetId planet;
        int fileIndex;
        bool expandedAspects;

        QComboBox*    planetSelector;
        ExpandWidget* position;
        ExpandWidget* aspects;
        ExpandWidget* power;
        QLabel*       positionLabel;
        QListWidget*  aspectsList;
        QLabel*       powerLabel;

        void describePlanet();
        void expandAspects();
        void collapseAspects();
        void updateListHeight(QListWidget* w);
        void clear();

    protected:                            // AstroFileHandler implementation
        void filesUpdated(MembersList);

        AppSettings defaultSettings ();
        AppSettings currentSettings ();
        void applySettings ( const AppSettings& );

    private slots:
        void aspectClicked(QModelIndex);
        void planetSelected(int index);

    public slots:
         void setCurrentPlanet(A::PlanetId, int);

    public:
        Details(QWidget* parent = 0);
};

#endif // DETAILS_H
