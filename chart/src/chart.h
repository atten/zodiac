#ifndef CHART_H
#define CHART_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <Astroprocessor/Gui>

enum CircleStart { Start_ZeroDegree = 0, Start_Ascendent = 1 };

class Chart;


class RotatingCircleItem : public QAbstractGraphicsShapeItem
{
    private:
        QRectF rect;
        float dragAngle;
        QDateTime dragDT;
        AstroFile* file;

        float angle(const QPointF& pos);                   // converts coordinate into angle
        Chart* chart();

    protected:
        void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
        bool sceneEventFilter(QGraphicsItem*, QEvent*);    // handles events of items
        bool sceneEvent(QEvent *event);

    public:
        RotatingCircleItem(QRect rect, const QPen& pen);
        QPainterPath shape() const;
        QRectF boundingRect() const { return rect; }

        void setFile(AstroFile* f) { file = f; }
        void setHelpTag(QGraphicsItem* item, QString tag);
};


/* =========================== ASTRO MAP SHOW ======================================= */

class Chart : public AstroFileHandler
{
    Q_OBJECT

    private:
        bool emptyScene;
        QRect mapRect;
        float zoom, defaultZoom;
        QPointF pan;
        QGraphicsView* view;
        RotatingCircleItem* circle;

        CircleStart circleStart;
        bool clockwise;
        int zodiacWidth;
        int innerRadius;
        int cuspideLength;
        bool coloredZodiac;
        bool zodiacDropShadow;

        QList<QGraphicsLineItem*>         cuspides;
        QList<QGraphicsSimpleTextItem*>   cuspideLabels;
        QMap<A::PlanetId, QGraphicsItem*> planetMarkers;
        QMap<A::PlanetId, QGraphicsItem*> planets;
        //QList<QGraphicsSimpleTextItem*> aspectMarkers;
        QList<QGraphicsLineItem*>         aspects;
        QList<QGraphicsItem*>             signIcons;

        int normalPlanetPosX(QGraphicsItem* planet, QGraphicsItem* marker);
        const QPen& aspectPen(const A::Aspect& asp);

        void fitInView();
        void createScene();
        void updateScene();
        void clearScene();

    protected:                            // AstroFileHandler && other implementations
        void resetToDefault()                    { clearScene(); }
        void fileUpdated(AstroFile::Members);
        void fileDestroyed()  { }

        AppSettings defaultSettings ();
        AppSettings currentSettings ();
        void applySettings       ( const AppSettings& );
        void setupSettingsEditor ( AppSettingsEditor* );

        bool eventFilter(QObject *, QEvent *);
        void resizeEvent (QResizeEvent *ev);

    public:
        Chart(QWidget *parent = 0);

        void help(QString tag)    { requestHelp(tag); }    // called by circle item (because requestHelp() is protected)
        bool isClockwise()        { return clockwise; }
        CircleStart startPoint()  { return circleStart; }
};

#endif // CHART_H
