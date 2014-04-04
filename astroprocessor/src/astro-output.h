#ifndef A_OUTPUT_H
#define A_OUTPUT_H

#include "astro-data.h"

namespace A {

enum PlanetsOrder   { Order_NoOrder,
                      Order_Power,
                      Order_House,
                      Order_Element
};

enum Article        { Article_All     = 0xFF,
                      Article_Input   = 0x1,
                      Article_Houses  = 0x2,
                      Article_Aspects = 0x4,
                      Article_Planet  = 0x8,
                      Article_Power   = 0x10 };

enum AnglePrecision { NormalPrecision,
                      HighPrecision };

Q_DECLARE_FLAGS(Articles, Article)



QString romanNum             ( int num);
QString houseNum             ( const Planet& planet );
QString getPositionName      ( PlanetPosition p );
QString degreeToString       ( float deg, AnglePrecision precision = NormalPrecision );
QString zodiacPosition       ( float deg, const Zodiac& zodiac, AnglePrecision precision = NormalPrecision );
QString zodiacPosition       ( const Planet& planet, const Zodiac& zodiac, AnglePrecision precision = NormalPrecision );
void    sortPlanets          ( PlanetList &planets, PlanetsOrder order );



QString     describeInput       ( const InputData& data );
QString     describeHouses      ( const Houses& houses, const Zodiac& zodiac );
QString     describeAspect      (const Aspect &aspect, bool monospace = false);
QString     describePlanet      ( const Planet& planet, const Zodiac& zodiac );
QString     describePlanetCoord  ( const Planet& planet );
QString     describePlanetCoordInHtml ( const Planet& planet );
QString     describePower       ( const Planet& planet, const Horoscope& scope );
QString     describePowerInHtml ( const Planet& planet, const Horoscope& scope );
QString     describe            ( const Horoscope& scope, Articles article = Article_All );

}

#endif // A_OUTPUT_H
