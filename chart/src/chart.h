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
        static const int defaultChartRadius = 500;
        bool emptyScene;
        bool dualChart;
        QRectF viewport;
        float zoom;
        bool justUpdated;
        QGraphicsView* view;
        RotatingCircleItem* circle;

        CircleStart circleStart;
        bool clockwise;
        int l_zodiacWidth;
        int l_innerRadius;
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

        float zodiacWidth()  { return l_zodiacWidth * zoom; }
        float innerRadius()  { return l_innerRadius * zoom; }
        float middleRadius() { return (l_innerRadius + l_zodiacWidth) * zoom; }
        QRect chartRect();

        void fitInView();
        void createScene();
        void updateScene();
        void updateSecondChart();
        void clearScene();

    protected:                            // AstroFileHandler && other implementations
        void resetFile()                    { /*clearScene();*/ }
        void reset2ndFile()                 { clearScene(); createScene(); updateScene(); }
        void fileUpdated(AstroFile::Members);
        void secondFileUpdated(AstroFile::Members);

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
