#ifndef PLANETS_H
#define PLANETS_H

#include <QDeclarativeView>
#include <QModelIndex>
#include <Astroprocessor/Gui>
#include <Astroprocessor/Output>

class ExpandWidget;
class QLabel;
class QListWidget;
class QSplitter;
class QRadioButton;
class QComboBox;
class QDeclarativeItem;


/* =========================== ASTRO QML VIEW ======================================= */

class AstroQmlView : public QDeclarativeView
{
    Q_OBJECT

    private:
        void fitSceneInView();

    protected:
        virtual void resizeEvent(QResizeEvent*) { fitSceneInView(); }

    public slots:

    signals:                        // bindings to QML functions
        void objectClicked (QString name);
        void requestHelp(QString tag);

    public:
        AstroQmlView(QWidget *parent = 0);
        void setSource(const QUrl& url);

};


/* =========================== ASTRO PLANETS SHOW =================================== */

class Planets : public AstroFileHandler
{
    Q_OBJECT

    private:
        A::PlanetsOrder order;
        A::PlanetId      selectedPlanet;
        bool expandedAspects;

        ExpandWidget* position;
        ExpandWidget* aspects;
        ExpandWidget* power;
        QLabel*       positionLabel;
        QListWidget*  aspectsList;
        QLabel*       powerLabel;
        QSplitter*    splitter;

        AstroQmlView* view;
        QList<QDeclarativeItem*> cardItems, labelItems;

        QRadioButton* sortByPlanets;
        QRadioButton* sortByHouses;
        QRadioButton* sortByPower;
        QRadioButton* sortByElement;
        QComboBox*    planetSelector;

        void drawPlanets();
        void describePlanet();
        void expandAspects();
        void collapseAspects();
        void updateListHeight(QListWidget* w);
        void setCurrentPlanet(A::PlanetId);

    private slots:
        void orderChanged();
        void planetClicked(QString name);
        void aspectClicked(QModelIndex);
        void planetSelected(int index);

    protected:                            // AstroFileHandler && other implementations
        void resetFile();
        void reset2ndFile() { }
        void fileUpdated(AstroFile::Members);
        void secondFileUpdated(AstroFile::Members) { }

        AppSettings defaultSettings ();
        AppSettings currentSettings ();
        void applySettings ( const AppSettings& );

    public:
        Planets(QWidget *parent = 0);

};

#endif // PLANETS_H
