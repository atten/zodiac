#ifndef A_DATA_H
#define A_DATA_H

#include <QString>
#include <QDateTime>
#include <QPointF>
#include <QVector3D>
#include <QVector2D>
#include <QVariant>

namespace A {


typedef int ZodiacSignId;
typedef int ZodiacId;
typedef int AspectId;
typedef int HouseSystemId;
typedef int PlanetId;
typedef int AspectSetId;


const PlanetId      Planet_None          = -1;
const PlanetId      Planet_Sun           =  0;
const PlanetId      Planet_Moon          =  1;
const PlanetId      Planet_Mercury       =  2;
const PlanetId      Planet_Venus         =  3;
const PlanetId      Planet_Mars          =  4;
const PlanetId      Planet_Jupiter       =  5;
const PlanetId      Planet_Saturn        =  6;
const PlanetId      Planet_Uranus        =  7;
const PlanetId      Planet_Neptune       =  8;
const PlanetId      Planet_Pluto         =  9;
const PlanetId      Planet_NorthNode     = 10;

const AspectId      Aspect_None          = -1;
const AspectId      Aspect_Conjunction   =  0;
const AspectId      Aspect_Trine         =  1;
const AspectId      Aspect_Sextile       =  2;
const AspectId      Aspect_Opposition    =  3;
const AspectId      Aspect_Quadrature    =  4;

const HouseSystemId Housesystem_None     = -1;
const HouseSystemId Housesystem_Placidus =  0;

const ZodiacId      Zodiac_Tropical      =  0;
const ZodiacId      Zodiac_None          = -1;

const ZodiacSignId  Sign_None            = -1;

const AspectSetId   AspectSet_Default    =  0;


struct ZodiacSign {
  ZodiacSignId id;
  ZodiacId zodiacId;
  QString tag;
  QString name;
  float startAngle;
  float endAngle;
  QMap<QString, QVariant> userData;

  ZodiacSign() { id         = Zodiac_None;
                 zodiacId   = -1;
                 startAngle = 0;
                 endAngle   = 0; }
};

struct Zodiac {
  ZodiacId id;
  QString name;
  QList<ZodiacSign> signs;

  Zodiac() { id = Zodiac_None; }
};

struct HouseSystem {
  HouseSystemId id;
  QString  name;
  char     sweCode;
  QMap<QString, QVariant> userData;

  HouseSystem() { id = Housesystem_None; }
};

struct Houses
{
  float          cusp[12];            // angles of cuspides (0... 360)
  const HouseSystem* system;

  Houses() { for (int i = 0; i < 12; i++) cusp[i] = 0;
             system = 0; }
};

struct PlanetPower
{
  int            dignity;
  int            deficient;

  PlanetPower() { dignity = 0;
                  deficient = 0; }
};

enum PlanetPosition { Position_Normal,
                      Position_Exaltation,
                      Position_Dwelling,
                      Position_Downfall,
                      Position_Exile };

struct Planet
{
  PlanetId       id;
  QString        name;
  int            sweNum;
  int            sweFlags;
  bool           isReal;
  QVector2D      defaultEclipticSpeed;
  QList<ZodiacSignId> homeSigns,
                 exaltationSigns,
                 exileSigns,
                 downfallSigns;
  QMap<QString, QVariant> userData;

  QPointF        horizontalPos;       // x - azimuth (0... 360), y - height (0... 360)
  QPointF        eclipticPos;         // x - longitude (0... 360), y - latitude (0... 360)
  QVector2D      eclipticSpeed;       // x - longitude speed (degree/day)
  double         distance;            // A.U. (astronomical units)
  PlanetPosition position;
  PlanetPower    power;
  const ZodiacSign* sign;
  int            house;
  int            houseRuler;

  Planet() { id = Planet_None;
             sweNum = 0;
             sweFlags = 0;
             isReal = false;
             horizontalPos = QPoint(0,0);
             eclipticPos   = QPoint(0,0);
             eclipticSpeed = QVector2D(0,0);
             distance = 0;
             position = Position_Normal;
             sign = 0;
             house = 0;
             houseRuler = 0; }

  bool operator==(const Planet & other) const { return this->id == other.id && this->eclipticPos == other.eclipticPos; }
  bool operator!=(const Planet & other) const { return this->id != other.id || this->eclipticPos != other.eclipticPos; }
};

//struct AspectsSet;

struct AspectType {
  AspectId id;
  const struct AspectsSet* set;
  QString  name;
  float    angle;
  float    orb;
  QMap<QString, QVariant> userData;

  AspectType() { id = Aspect_None;
                 set = 0;
                 angle = 0;
                 orb = 0; }
};

struct Aspect {
  const AspectType* d;
  const Planet* planet1;
  const Planet* planet2;
  float         angle;
  float         orb;
  bool          applying;

  Aspect()    { planet1 = 0;
                planet2 = 0;
                angle   = 0;
                orb     = 0;
                d       = 0;
                applying  = false; }
};

struct AspectsSet {
  AspectSetId id;
  QString name;
  QMap<AspectId, AspectType> aspects;

  AspectsSet() { id = AspectSet_Default; }
};


typedef QList<Aspect> AspectList;
typedef QList<Planet> PlanetList;
typedef QMap<PlanetId, Planet> PlanetMap;

class Data
{
    private:
        static QString usedLang;
        static QMap<AspectSetId, AspectsSet> aspectSets;
        static QMap<HouseSystemId, HouseSystem> houseSystems;
        static QMap<ZodiacId, Zodiac> zodiacs;
        static QMap<PlanetId, Planet> planets;
        static AspectSetId topAspSet;

    public:
        static void load(QString language);
        static const QString usedLanguage() { return usedLang; }

        static const Planet& getPlanet(PlanetId id);
        static QList<PlanetId> getPlanets();

        static const HouseSystem& getHouseSystem(HouseSystemId id);
        static const QList<HouseSystem> getHouseSystems();

        static const Zodiac& getZodiac(ZodiacId id);
        static const QList<Zodiac> getZodiacs();

        static const AspectType& getAspect(AspectId id, const AspectsSet& set);
        //static const QList<AspectType> getAspects(AspectSetId set);

        static QList<AspectsSet> getAspectSets() { return aspectSets.values(); }
        static const AspectsSet& getAspectSet(AspectSetId set);
        static const AspectsSet& topAspectSet() { return aspectSets[topAspSet]; }
};

void load(QString language);
QString usedLanguage();
const Planet& getPlanet(PlanetId id);
QList<PlanetId> getPlanets();
const HouseSystem& getHouseSystem(HouseSystemId id);
const Zodiac& getZodiac(ZodiacId id);
const QList<HouseSystem> getHouseSystems();
const QList<Zodiac> getZodiacs();
//const QList<AspectType> getAspects(AspectSetId set);
const AspectType& getAspect(AspectId id, const AspectsSet& set);
QList<AspectsSet> getAspectSets();
const AspectsSet& getAspectSet(AspectSetId set);
const AspectsSet&  topAspectSet();


struct InputData
{
  QDateTime      GMT;                 // greenwich time & date
  QVector3D      location;            // x - longitude (-180...180); y - latitude (-180...180), z - height
  HouseSystemId  houseSystem;
  ZodiacId       zodiac;
  AspectSetId    aspectSet;

  InputData() { GMT.setTimeSpec(Qt::UTC);
                GMT.setTime_t(0);
                location    = QVector3D(0,0,0);
                houseSystem = Housesystem_Placidus;
                zodiac      = Zodiac_Tropical;
                aspectSet   = AspectSet_Default; }
};

struct Horoscope
{
  InputData  inputData;
  Zodiac     zodiac;
  Houses     houses;
  AspectList aspects;
  PlanetMap  planets;
  Planet     sun,
             moon,
             mercury,
             venus,
             mars,
             jupiter,
             saturn,
             uranus,
             neptune,
             pluto,
             northNode;
};

}
#endif // A_DATA_H
