#include <swephexp.h>
#undef MSDOS     // undef macroses that made by SWE library
#undef UCHAR
#undef forward

#include <QDebug>
#include "csvreader.h"
#include "astro-data.h"

namespace A {


QMap<AspectSetId, AspectsSet> Data::aspectSets = QMap<AspectSetId, AspectsSet>();
QMap<PlanetId, Planet> Data::planets = QMap<PlanetId, Planet>();
QMap<HouseSystemId, HouseSystem> Data::houseSystems = QMap<HouseSystemId, HouseSystem>();
QMap<ZodiacId, Zodiac> Data::zodiacs = QMap<ZodiacId, Zodiac>();
AspectSetId Data::topAspSet = AspectSetId();
QString Data::usedLang = QString();

void Data :: load(QString language)
 {
  usedLang = language;
  swe_set_ephe_path( "swe/" );
  CsvFile f;

  f.setFileName("astroprocessor/aspect_sets.csv");
  if (!f.openForRead()) qDebug() << "A: Missing file" << f.fileName();
  topAspSet = 0;
  while (f.readRow())
   {
    AspectsSet s;
    s.id   = f.row(0).toInt();
    s.name = language == "ru" ? f.row(2) : f.row(1);

    //for (int i = 3; i < f.columnsCount(); i++)
    //  s.userData[f.header(i)] = f.row(i);

    aspectSets[s.id] = s;
    topAspSet = qMax(topAspSet, s.id);         // update top set
   }

  f.close();
  f.setFileName("astroprocessor/aspects.csv");
  if (!f.openForRead()) qDebug() << "A: Missing file" << f.fileName();
  while (f.readRow())
   {
    AspectType a;
    AspectSetId setId = f.row(0).toUInt();
    a.set   = &aspectSets[setId];
    a.id    = f.row(1).toInt();
    a.name  = language == "ru" ? f.row(3) : f.row(2);
    a.angle = f.row(4).toFloat();
    a.orb   = f.row(5).toFloat();

    for (int i = 6; i < f.columnsCount(); i++)
      a.userData[f.header(i)] = f.row(i);

    aspectSets[setId].aspects[a.id] = a;
   }

  f.close();
  f.setFileName("astroprocessor/hsystems.csv");
  if (!f.openForRead()) qDebug() << "A: Missing file" << f.fileName();
  while (f.readRow())
   {
    HouseSystem h;
    h.id      = f.row(0).toInt();
    h.name    = language == "ru" ? f.row(2) : f.row(1);
    h.sweCode = f.row(3)[0].toLatin1();

    houseSystems[h.id] = h;
   }

  f.close();
  f.setFileName("astroprocessor/zodiac.csv");
  if (!f.openForRead()) qDebug() << "A: Missing file" << f.fileName();
  while (f.readRow())
   {
    Zodiac z;
    z.id       = f.row(0).toInt();
    z.name     = language == "ru" ? f.row(2) : f.row(1);

    zodiacs[z.id] = z;
   }

  f.close();
  f.setFileName("astroprocessor/signs.csv");
  if (!f.openForRead()) qDebug() << "A: Missing file" << f.fileName();
  QHash<QString, ZodiacSignId> signs;            // collect and find signs by tag
  while (f.readRow())
   {
    ZodiacSign s;
    s.zodiacId  = f.row(0).toInt();
    s.id        = f.row(1).toInt();
    s.tag       = f.row(2);
    s.name      = language == "ru" ? f.row(4) : f.row(3);
    s.startAngle = f.row(5).toFloat();
    s.endAngle   = f.row(6).toFloat() + s.startAngle;
    if (s.endAngle > 360) s.endAngle -= 360;

    for (int i = 7; i < f.columnsCount(); i++)
      s.userData[f.header(i)] = f.row(i);

    zodiacs[s.zodiacId].signs << s;
    signs.insert(s.tag, s.id);
   }

  f.close();
  f.setFileName("astroprocessor/planets.csv");
  if (!f.openForRead()) qDebug() << "A: Missing file" << f.fileName();
  while (f.readRow())
   {
    Planet p;
    p.id       = f.row(0).toInt();
    p.name     = language == "ru" ? f.row(2) : f.row(1);
    p.sweNum   = f.row(3).toInt();
    p.sweFlags = f.row(4).toInt();
    p.defaultEclipticSpeed.setX(f.row(5).toFloat());
    p.isReal   = f.row(6).toInt();
    foreach (QString s, f.row(7).split(',')) p.homeSigns       << signs.values(s);
    foreach (QString s, f.row(8).split(',')) p.exaltationSigns << signs.values(s);
    foreach (QString s, f.row(9).split(',')) p.exileSigns      << signs.values(s);
    foreach (QString s, f.row(10).split(',')) p.downfallSigns   << signs.values(s);

    for (int i = 11; i < f.columnsCount(); i++)
      p.userData[f.header(i)] = f.row(i);

    planets[p.id] = p;
   }

  qDebug() << "Astroprocessor: initialized";
 }

const Planet& Data :: getPlanet(PlanetId id)
 {
  if (Data::planets.contains(id))
    return Data::planets[id];

  return planets[Planet_None];
 }

QList<PlanetId> Data :: getPlanets()
 {
  return planets.keys();
 }

const HouseSystem& Data :: getHouseSystem(HouseSystemId id)
 {
  if (Data::houseSystems.contains(id))
    return Data::houseSystems[id];

  return houseSystems[Housesystem_None];
 }

const Zodiac& Data :: getZodiac(ZodiacId id)
 {
  if (Data::zodiacs.contains(id))
    return Data::zodiacs[id];

  return zodiacs[Zodiac_Tropical];
 }

const QList<HouseSystem> Data :: getHouseSystems()
 {
  return Data::houseSystems.values();
 }

const QList<Zodiac> Data :: getZodiacs()
 {
  return zodiacs.values();
 }

/*const QList<AspectType> Data :: getAspects(AspectSetId set)
 {
  set = qBound(0, set, aspects.count());
  return aspects[set].values();
 }*/

const AspectType& Data :: getAspect(AspectId id, const AspectsSet& set)
 {
  if (set.aspects.contains(id))
    return aspectSets[set.id].aspects[id];

  return aspectSets[AspectSet_Default].aspects[Aspect_None];
 }

const AspectsSet& Data :: getAspectSet(AspectSetId set)
 {
  if (aspectSets.contains(set))
    return aspectSets[set];

  return aspectSets[AspectSet_Default];
 }

/*QList<AspectsSet>& Data :: getAspectSets()
 {
  return aspectSets.values();
 }*/


void load(QString language) { Data::load(language); }
QString usedLanguage()      { return Data::usedLanguage(); }
const Planet& getPlanet(PlanetId id) { return Data::getPlanet(id); }
QList<PlanetId> getPlanets() { return Data::getPlanets(); }
const HouseSystem& getHouseSystem(HouseSystemId id) { return Data::getHouseSystem(id); }
const Zodiac& getZodiac(ZodiacId id) { return Data::getZodiac(id); }
const QList<HouseSystem> getHouseSystems() { return Data::getHouseSystems(); }
const QList<Zodiac> getZodiacs() { return Data::getZodiacs(); }
//const QList<AspectType> getAspects(AspectSetId set) { return Data::getAspects(set); }
const AspectType& getAspect(AspectId id, const AspectsSet& set) { return Data::getAspect(id, set); }
QList<AspectsSet> getAspectSets() { return Data::getAspectSets(); }
const AspectsSet& getAspectSet(AspectSetId set) { return Data::getAspectSet(set); }
const AspectsSet& topAspectSet() { return Data::topAspectSet(); }
}
