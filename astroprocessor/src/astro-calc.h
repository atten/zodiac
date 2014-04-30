#ifndef A_CALC_H
#define A_CALC_H

#include "astro-data.h"


namespace A {  // Astrology, sort of :)

double  getJulianDate            ( QDateTime GMT );
float   roundDegree              ( float deg );         // returns 0...360
const ZodiacSign& getSign        ( float deg, const Zodiac& zodiac );
int     getHouse                 ( const Houses& houses, float deg ); // returns 1...12
int     getHouse                 ( ZodiacSignId sign, const Houses& houses, const Zodiac& zodiac );

float   angle                    ( const Planet& planet1, const Planet& planet2 );
float   angle                    ( const Planet& planet, float deg );
float   angle                    ( const Planet& planet, QPointF coordinate );
float   angle                    ( float deg1, float deg2 );
AspectId aspect                  ( const Planet& planet1, const Planet& planet2, AspectLevel aspectLevel );
AspectId aspect                  ( const Planet& planet, QPointF coordinate, AspectLevel aspectLevel );
AspectId aspect                  ( const Planet& planet1, float degree, AspectLevel aspectLevel );
AspectId aspect                  ( float angle, AspectLevel aspectLevel );
bool    towardsMovement          ( const Planet& planet1, const Planet& planet2 );
PlanetPosition getPosition       ( const Planet& planet, ZodiacSignId sign );
const Planet* doryphoros         ( const Horoscope& scope );
const Planet* auriga             ( const Horoscope& scope );
const Planet* almuten            ( const Horoscope& scope );
bool    rulerDisposition         ( int house, int houseAuthority, const Horoscope& scope );
bool    isEarlier                ( const Planet& planet, const Planet& sun );
//const Planet& ruler          ( int house, const Horoscope& scope );
PlanetId receptionWith           ( const Planet& planet, const Horoscope& scope );


Planet      calculatePlanet      ( PlanetId planet, const InputData& input, const Houses& houses, const Zodiac& zodiac );
PlanetPower calculatePlanetPower ( const Planet& planet, const Horoscope& scope );
Houses      calculateHouses      ( const InputData& input );
Aspect      calculateAspect      ( AspectLevel aspectLevel, const Planet& planet1, const Planet& planet2 );
AspectList  calculateAspects     ( AspectLevel aspectLevel, const PlanetMap& planets );
AspectList  calculateAspects     ( AspectLevel aspectLevel, const PlanetMap& planets1, const PlanetMap& planets2 );   // synastry
Horoscope   calculateAll         ( const InputData& input );

}
#endif // A_CALC_H
