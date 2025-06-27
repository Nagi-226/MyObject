#include "MathAddon.h"


const float MathAddon::PI = 3.14159265359f;




float MathAddon::angleRadToDeg(float angle) {
	return angle * 180.0f / PI;
}


float MathAddon::angleDegToRad(float angle) {
	return angle * PI / 180.0f;
}