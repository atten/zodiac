#include <QObject>
#include <QStringList>
#include <stdio.h>
#include <math.h>
#include "astro-calc.h"
#include "astro-output.h"
//#include <QDebug>

namespace A {

QString romanNum             ( int num )
 {
  static const char* h[] = { "I", "II", "III", "IV",
                             "V", "VI", "VII", "VIII",
                             "IX", "X", "XI",  "XII" };

  if (num >= 1 && num <= 12) return h[--num];
  return "0";
 }

QString houseTag             ( int num )
 {
  static const char* h[] = { "Asc", "II", "III", "IV",
                             "V", "VI", "VII", "VIII",
                             "IX", "X", "XI",  "XII" };

  if (num >= 1 && num <= 12) return h[--num];
  return "0";
 }

QString houseNum             ( const Planet& planet )
 {
  return romanNum(planet.house);
 }

QString getPositionName      ( PlanetPosition p )
 {
  switch (p)
   {
    case Position_Normal     : return QObject::tr("");
    case Position_Exaltation : return QObject::tr("Exaltation");
    case Position_Dwelling   : return QObject::tr("House");
    case Position_Downfall   : return QObject::tr("Fall");
    case Position_Exile      : return QObject::tr("Detriment");
   }

  return "";
 }

QString degreeToString       ( float deg, AnglePrecision precision )
 {
  int d=0, m=0, s=0, polarity=0;

  if (deg<0)
   {
    polarity=-1;
    deg = -deg;
   }

  d = (int)(deg);
  m = (int)(60.0*(deg - d));

  char* str = new char[20];
  if (precision == HighPrecision)
   {
    s = (int)((deg - (d + m/60.0)) * 3600.0);
    sprintf( str, "%s%i°%02i\'%02i\'\'", (polarity<0)?"-":"", d, m, s );
   }
  else
   {
    if (m) sprintf( str, "%s%i°%02i\'", (polarity<0)?"-":"", d, m );
    else   sprintf( str, "%s%i°", (polarity<0)?"-":"", d );
   }

  return QString(str);
 }

QString zodiacPosition       (float deg, const Zodiac& zodiac, AnglePrecision precision )
 {
  const ZodiacSign& sign  = getSign(deg, zodiac);
  int ang = floor(deg) - sign.startAngle;
  if (ang < 0) ang += 360;

  if (precision)
   {
    QString str = degreeToString(deg, precision);
    str.remove(0, str.indexOf("°"));
    return QString("%1%2 %3").arg(ang).arg(str).arg(sign.tag);
   }
  else
   {
    int m = (int)(60.0*(deg - (int)deg));
    return QString("%1 %2 %3%4").arg(ang).arg(sign.tag).arg(m > 10 ? "" : "0").arg(m);
   }
 }

QString zodiacPosition       (const Planet& planet, const Zodiac& zodiac, AnglePrecision precision )
 {
  return zodiacPosition(planet.eclipticPos.x(), zodiac, precision);
 }

void sortPlanets             ( PlanetList &planets, PlanetsOrder order )
 {
  if (!planets.count()) return;
  if (order == Order_NoOrder) return;

  for (int i = 0; i < planets.count(); i++)
   {
    for (int j = i + 1; j < planets.count(); j++)
     {
      if (order == Order_House)
       {
        if (planets[i].house > planets[j].house || (planets[i].house == planets[j].house &&
                                                    isEarlier(planets[j], planets[i])))
         {
          Planet t = planets[i];
          planets[i] = planets[j];
          planets[j] = t;
         }
       }
      else if (order == Order_Power)
       {
        if ((planets[i].power.dignity > 0 && planets[j].power.dignity > 0 &&
            planets[i].power.deficient + planets[i].power.dignity <
            planets[j].power.deficient + planets[j].power.dignity)
            ||
            (planets[i].power.dignity == 0 && planets[i].power.dignity > 0))
         {
          Planet t = planets[i];
          planets[i] = planets[j];
          planets[j] = t;
         }
       }
      else if (order == Order_Element)
       {
        int sign1 = planets[i].sign->id;
        int sign2 = planets[j].sign->id;
        if ((sign1 - 1) % 4 > (sign2 - 1) % 4)
         {
          Planet t = planets[i];
          planets[i] = planets[j];
          planets[j] = t;
         }
       }
     }
   }
 }






QString     describeInput     ( const InputData& data )
 {
  QString ret;

  QString date      = data.GMT.date().toString(Qt::DefaultLocaleLongDate);
  QString time      = data.GMT.time().toString(Qt::DefaultLocaleLongDate);
  QString dayOfWeek = data.GMT.date().toString("ddd");

  ret += QObject::tr("Date: %1, %2 %3 GMT\n").arg(dayOfWeek).arg(date).arg(time);
  ret += QObject::tr("Location: %1N %2E").arg(degreeToString(data.location.y(), HighPrecision))
                                         .arg(degreeToString(data.location.x(), HighPrecision));
  return ret;
 }

QString     describeHouses    ( const Houses& houses, const Zodiac& zodiac )
 {
  QString ret;
  ret += QObject::tr("Houses (%1)\n").arg(houses.system->name);

  for (int i = 0; i < 12; i++)
   {
    ret += houseTag(i + 1).rightJustified(4, ' ') + " -" +
           zodiacPosition(houses.cusp[i], zodiac).rightJustified(10,' ');

    if (i < 11) ret += '\n';
   }

  return ret;
 }

QString     describeAspect    (const Aspect &aspect, bool monospace)
 {
  QString ret = aspect.d->name;
  if (monospace) ret = ret.leftJustified(14, ' ', true);
  if (!ret.isEmpty()) ret += " ";

  ret += aspect.planet1->name + "-" + aspect.planet2->name;
  if (monospace) ret = ret.leftJustified(35, ' ', true);

  QString angleStr = degreeToString(aspect.angle);

  if (aspect.applying) ret += " >" + angleStr + "<";
  else               ret += " <" + angleStr + ">";

  return ret;
 }

QString     describeAspectFull  (const Aspect &asp, QString tag1, QString tag2)
 {
  //if (!tag1.isEmpty()) tag1 = " (" + tag1 + ")";
  //if (!tag2.isEmpty()) tag1 = " (" + tag2 + ")";

  return QString("%1 (%2) %3%4-%5%6 [%7]\n").arg(asp.d->name)
                                   .arg(degreeToString(asp.angle))
                                   .arg(asp.planet1->name)
                                   .arg(tag1)
                                   .arg(asp.planet2->name)
                                   .arg(tag2)
                                   .arg(degreeToString(asp.d->angle)) +
         QObject::tr("Orb: %1 (max: %2)\n").arg(degreeToString(asp.orb))
                                           .arg(degreeToString(asp.d->orb)) +
         (asp.applying ? QObject::tr("Applying") : QObject::tr("Separating"));
 }

QString     describePlanet    ( const Planet& planet, const Zodiac& zodiac )
 {
  QString ret;

  QString name      = planet.name.leftJustified(10,' ',true);
  QString longitude = zodiacPosition(planet, zodiac).rightJustified(10,' ',true);

  ret = name + " " + longitude + " " + houseNum(planet).leftJustified(4, ' ');

  if (planet.defaultEclipticSpeed.x() != 0)
   {
    float speed = planet.eclipticSpeed.x() / planet.defaultEclipticSpeed.x();
    ret += QString(" (%1%)").arg((int)(speed * 100)).leftJustified(8,' ',true);
   }
  else
   ret += QString().leftJustified(8,' ');

  if (planet.power.dignity != 0 || planet.power.deficient != 0)
   {
    QString plus      = planet.power.dignity != 0 ? "+" : "";
    QString dignity   = QString(plus + "%1").arg(planet.power.dignity).rightJustified(3, ' ');
    QString deficient = QString("%1").arg(planet.power.deficient).leftJustified(3, ' ');

    ret += " " + dignity + "|" + deficient;
   }
  else
   ret += QString().leftJustified(8,' ');

  if (planet.houseRuler > 0)
    ret += QObject::tr(" ruler of %1").arg(romanNum(planet.houseRuler)).leftJustified(15, ' ');

  if (planet.position != Position_Normal)
   ret += getPositionName(planet.position);

  return ret;
 }

QString     describePlanetCoord ( const Planet& planet )
 {
  QString ret;

  ret += QObject::tr("Longitude: %1\n").arg(degreeToString(planet.eclipticPos.x(), HighPrecision));
  ret += QObject::tr("Latitude: %1\n").arg(degreeToString(planet.eclipticPos.y(), HighPrecision));
  ret += QObject::tr("Distance: %1a.u.\n").arg(planet.distance);
  ret += QObject::tr("Azimuth: %1\n").arg(degreeToString(planet.horizontalPos.x(), HighPrecision));
  ret += QObject::tr("Height: %1\n").arg(degreeToString(planet.horizontalPos.y(), HighPrecision));

  if (planet.isReal)
    ret += QObject::tr("Speed: %1% (%2 per day)\n")
                    .arg((int)(planet.eclipticSpeed.x() / planet.defaultEclipticSpeed.x() * 100))
                    .arg(degreeToString(planet.eclipticSpeed.x(), HighPrecision));

  return ret;
 }

QString     describePlanetCoordInHtml ( const Planet& planet )
 {
  QString ret = describePlanetCoord(planet);
  ret.replace(QRegExp("(: [!-° ]+)"), "<font color='#e9e9e4'>\\1</font>");  // replaces values
  ret.replace("\n", "<br>");
  return ret;
 }

QString     describePower     ( const Planet& planet, const Horoscope& scope)
 {
  if (!planet.isReal) return "";

  QStringList ret;

  // TODO: finally work out

  bool peregrine = false;
  switch (planet.position)
   {
    case Position_Dwelling:   ret << QObject::tr("+5: Planet is in its own sign"); break;
    case Position_Exaltation: ret << QObject::tr("+5: Planet is in exaltation"); break;
    case Position_Exile:      ret << QObject::tr("-5: Planet is in detriment"); break;
    case Position_Downfall:   ret << QObject::tr("-4: Planet is in its fall"); break;
    case Position_Normal:     peregrine = true;
    default: break;
   }

  PlanetId pl = receptionWith(planet, scope);
  if (pl != Planet_None)
    ret << QObject::tr("+5: Planet is in mutual reception with %1").arg(getPlanet(pl).name);
  else if (peregrine)
    ret << QObject::tr("-5: Planet is peregrine (doesn't have an essential dignity)");


  int h = planet.house;
  QString p;
  switch (h)
   {
    case 1: case 10:         p = "+5"; break;
    case 4: case 7: case 11: p = "+4"; break;
    case 2: case 5:          p = "+3"; break;
    case 9:                  p = "+2"; break;
    case 3:                  p = "+1"; break;
    case 12:                 p = "-5"; break;
    case 8: case 6:          p = "-2"; break;
    default: break;
   }
  if (!p.isEmpty()) ret << QObject::tr("%1: Planet is placed in %2 house").arg(p).arg(romanNum(h));


  if (planet.eclipticSpeed.x() > 0 &&
      planet.id != Planet_Sun &&
      planet.id != Planet_Moon)
    ret << QObject::tr("+4: Planet is direct");

  if (planet.eclipticSpeed.x() > planet.defaultEclipticSpeed.x())
    ret << QObject::tr("+2: Planet is fast");
  else if (planet.eclipticSpeed.x() > 0)
    ret << QObject::tr("-2: Planet is slow");
  else
    ret << QObject::tr("-5: Planet is retrograde");

  switch (planet.id)
   {
    case Planet_Mars:
    case Planet_Jupiter:
    case Planet_Saturn:

       if (isEarlier(planet, scope.sun))
         ret << QObject::tr("+2: %1 rises earlier than the Sun (oriental)").arg(planet.name);
       else
         ret << QObject::tr("-2: %1 rises later than the Sun (occidental)").arg(planet.name);
       break;

    case Planet_Mercury:
    case Planet_Venus:

       if (!isEarlier(planet, scope.sun))
         ret << QObject::tr("+2: %1 rises later than the Sun (occidental)").arg(planet.name);
       else
         ret << QObject::tr("-2: %1 rises earlier than the Sun (occidental)").arg(planet.name);
       break;

    case Planet_Moon:

       if (!isEarlier(planet, scope.sun))
         ret << QObject::tr("+2: Moon is waxing");
       else
         ret << QObject::tr("-2: Moon is waning");
       break;

    default: break;
   }


  if (planet.id != Planet_Sun)
   {
    if      ( angle(planet, scope.sun) > 9 )
      ret << QObject::tr("+5: Planet is neither combust nor under the beams");
    else if ( angle(planet, scope.sun) < 0.4 )
      ret << QObject::tr("+5: Planet is cazimi");
    else
      ret << QObject::tr("-4: Planet is either combust or under the beams");
   }


  switch (aspect(planet, scope.jupiter, topAspectSet()))
   {
    case Aspect_Conjunction: ret << QObject::tr("+5: Planet is in partile conjunction with Jupiter"); break;
    case Aspect_Trine:       ret << QObject::tr("+4: Planet is in partile trine with Jupiter"); break;
    case Aspect_Sextile:     ret << QObject::tr("+3: Planet is in partile sextile with Jupiter"); break;
    default: break;
   }

  switch (aspect(planet, scope.venus, topAspectSet()))
   {
    case Aspect_Conjunction: ret << QObject::tr("+5: Planet is in partile conjunction with Venus"); break;
    case Aspect_Trine:       ret << QObject::tr("+4: Planet is in partile trine with Venus"); break;
    case Aspect_Sextile:     ret << QObject::tr("+3: Planet is in partile sextile with Venus"); break;
    default: break;
   }

  switch (aspect(planet, scope.northNode, topAspectSet()))
   {
    case Aspect_Conjunction: ret << QObject::tr("+4: Planet is in partile conjunction with North Node"); break;
    /*case Aspect_Trine:       ret << QObject::tr("+4: Planet is in partile trine with North Node"); break;
    case Aspect_Sextile:     ret << QObject::tr("+4: Planet is in partile sextile with North Node"); break;
    case Aspect_Opposition:  ret << QObject::tr("-4: Planet is in partile opposition with North Node"); break;*/
    default: break;
   }

  switch (aspect(planet, scope.mars, topAspectSet()))
   {
    case Aspect_Conjunction: ret << QObject::tr("-5: Planet is in partile conjunction with Mars"); break;
    case Aspect_Opposition:  ret << QObject::tr("-4: Planet is in partile opposition with Mars"); break;
    case Aspect_Quadrature:  ret << QObject::tr("-3: Planet is in partile quadrature with Mars"); break;
    default: break;
   }

  switch (aspect(planet, scope.saturn, topAspectSet()))
   {
    case Aspect_Conjunction: ret << QObject::tr("-5: Planet is in partile conjunction with Saturn"); break;
    case Aspect_Opposition:  ret << QObject::tr("-4: Planet is in partile opposition with Saturn"); break;
    case Aspect_Quadrature:  ret << QObject::tr("-3: Planet is in partile quadrature with Saturn"); break;
    default: break;
   }


  if (aspect(planet, QPointF(149.833, 0.45), topAspectSet()) == Aspect_Conjunction)
    ret << QObject::tr("+6: Planet is in conjunction with Regulus");

  if (aspect(planet, QPointF(203.833, -2.05), topAspectSet()) == Aspect_Conjunction)
    ret << QObject::tr("+5: Planet is in conjunction with Spica");

  if (aspect(planet, QPointF(56.166, 22.416), topAspectSet()) == Aspect_Conjunction)
    ret << QObject::tr("-5: Planet is in conjunction with Algol");


  // sort values from biggest to smallest

  for (int i = 0; i < ret.count(); i++)
   {
    for (int j = i + 1; j < ret.count(); j++)
     {
      int val1 = ret[i].at(1).digitValue();
      int val2 = ret[j].at(1).digitValue();

      if (ret[i][0] == '-') val1 = -val1;
      if (ret[j][0] == '-') val2 = -val2;

      if (val1 < val2)
       {
        QString t = ret[i];
        ret[i] = ret[j];
        ret[j] = t;
       }
     }
   }


  return ret.join(";\n") + '.';
 }

QString     describePowerInHtml ( const Planet& planet, const Horoscope& scope )
 {
  QString ret = describePower(planet, scope);
  if (ret.isEmpty()) return ret;

  ret.replace("\n", "</p><p>");
  ret.replace(QRegExp("(-\\d:)"),   "<font color='#dfb096'><b>\\1</b></font>");  // replaces negative values
  ret.replace(QRegExp("(\\+\\d:)"), "<font color='#71aeec'><b>\\1</b></font>");  // replaces positive values

  return "<p>" + ret + "</p>";
 }

QString     describe          ( const Horoscope& scope, Articles article )
 {
  QString ret;

  ret += QObject::tr("%1 sign").arg(scope.zodiac.name) + "\n";

  if (article & Article_Input)
    ret += describeInput(scope.inputData) + "\n\n";

  if ((article & Article_Planet) && scope.planets.count())
   {
    foreach (const Planet& p, scope.planets)
      ret += describePlanet(p, scope.zodiac) + "\n";

    ret += "\n";
    ret += QObject::tr("Auriga:     %1").arg(auriga(scope)->name)     + "\n";
    ret += QObject::tr("Almuten:    %1").arg(almuten(scope)->name)   + "\n";
    ret += QObject::tr("Doryphoros: %1").arg(doryphoros(scope)->name) + "\n\n";
   }


  if ((article & Article_Houses) && scope.houses.system)
    ret += describeHouses(scope.houses, scope.zodiac) + "\n\n";


  if ((article & Article_Aspects) && scope.aspects.count())
   {
    foreach (const Aspect& asp, scope.aspects)
      ret += describeAspect(asp, true) + "\n";
    ret += "\n";
   }


  if ((article & Article_Power) && scope.planets.count())
    foreach (const Planet& p, scope.planets)
      if (p.isReal)
        ret += p.name + "\n" + describePower(p, scope) + "\n\n";

  return ret;
 }


}
