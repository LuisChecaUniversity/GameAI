#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <limits>

//Set waypoints visible.
#define WAYPOINTS_VISIBLE

//Useful constants.
const int     MaxInt    = (std::numeric_limits<int>::max)();
const double  MaxDouble = (std::numeric_limits<double>::max)();
const double  MinDouble = (std::numeric_limits<double>::min)();
const float   MaxFloat  = (std::numeric_limits<float>::max)();
const float   MinFloat  = (std::numeric_limits<float>::min)();

//Angle manipulation.
const double   Pi					= 3.14159;
const double   TwoPi				= Pi * 2;
const double   HalfPi				= Pi / 2;
const double   QuarterPi			= Pi / 4;

inline double DegsToRads(double degrees) {return TwoPi * (degrees/360.0f);}
inline double RadsToDegs(double radians) {return radians * (180.0f/Pi);}

//Screen dimensions.
const int kScreenWidth					= 960;
const int kScreenHeight					= 640;

//Spritesheet dimensions.
const int   kNumberOfSpritesPerMan		= 3;
const float kBulletFireDelay			= 0.1f;
const float kCannonFireDelay			= 0.25f;
const int	kNumberOfSpritesForCannon	= 3;

//Weapon damage
const int		kBulletDamage			= 1;
const int		kRocketDamage			= 5;
const int		kMineDamage				= 10;

//Speeds
const float		kSpeedIncrement			= 1000.0f;
const float		kReboundSpeed			= -30.0f;
const float		kBulletSpeed			= 100.0f;
const float		kRocketSpeed			= 200.0f;

#endif //CONSTANTS_H