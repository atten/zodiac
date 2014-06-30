#ifndef PLANETS_H
#define PLANETS_H

#include <QDeclarativeView>
#include <Astroprocessor/Gui>
#include <Astroprocessor/Output>

class QRadioButton;
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

        AstroQmlView* view;
        QList<QDeclarativeItem*> cardItems, labelItems;

        QRadioButton* sortByPlanets;
        QRadioButton* sortByHouses;
        QRadioButton* sortByPower;
        QRadioButton* sortByElement;

        void drawPlanets();

    private slots:
        void orderChanged();
        void objectClicked(QString name);

    signals:
        void planetSelected(A::PlanetId, int fileIndex);

    protected:                            // AstroFileHandler && other implementations
        void filesUpdated(MembersList);

    public:
        Planets(QWidget *parent = 0);

};

#endif // PLANETS_H
