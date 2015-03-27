/*
 * common_geohash.h
 *
 *  Created on: Mar 20, 2015
 *      Author: jimm
 */

#ifndef COMMON_GEOHASH_H_
#define COMMON_GEOHASH_H_

#include "common_typedef.h"
#include <string>
using namespace std;

class CGeoHash
{
public:
	static void EncodeGeoHash(double latitude, double longitude, char *geohash);
	static void DecodeGeoHash(char *geohash, double &latitude, double &longitude);

	static string CalculateAdjacent(string strSrcHash, int32_t dir);
protected:
	static void RefineInterval(double interval[], int32_t cd, int32_t mask);
	static double GetPrecision(double x, double precision);

	static const char *BASE32;
	static char bits[5];
	static int RIGHT;
	static int LEFT;
	static int TOP;
	static int BOTTOM;
	static int EVEN;
	static int ODD;
};

#endif /* COMMON_GEOHASH_H_ */

