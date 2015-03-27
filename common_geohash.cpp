/*
 * common_geohash.cpp
 *
 *  Created on: Mar 20, 2015
 *      Author: jimm
 */

#include "common_geohash.h"
#include "common_api.h"
#include <string.h>
#include <math.h>

const char *CGeoHash::BASE32 = "0123456789bcdefghjkmnpqrstuvwxyz";
char CGeoHash::bits[] = { 16, 8, 4, 2, 1 };
int CGeoHash::RIGHT = 0;
int CGeoHash::LEFT = 1;
int CGeoHash::TOP = 2;
int CGeoHash::BOTTOM = 3;
int CGeoHash::EVEN = 0;
int CGeoHash::ODD = 1;

static const char *NEIGHBORS[4][2] =
{
		"14365h7k9dcfesgujnmqp0r2twvyx8zb", "238967debc01fg45kmstqrwxuvhjyznp",
		"p0r21436x8zb9dcf5h7kjnmqesgutwvy", "bc01fg45238967deuvhjyznpkmstqrwx",
		"238967debc01fg45kmstqrwxuvhjyznp", "14365h7k9dcfesgujnmqp0r2twvyx8zb",
		"bc01fg45238967deuvhjyznpkmstqrwx", "p0r21436x8zb9dcf5h7kjnmqesgutwvy",
};

static const char *BORDERS[4][2] =
{
		"028b", "0145hjnp",
		"prxz", "bcfguvyz",
		"0145hjnp", "028b",
		"bcfguvyz", "prxz",
};

#define max(a, b) ((a) > (b) ? (a) : (b))

void CGeoHash::EncodeGeoHash(double latitude, double longitude, char *geohash)
{
	int32_t is_even = 1, i = 0;
	double lat[2], lon[2], mid;
	int32_t bit = 0, ch = 0;
	int32_t precision = 12;
	lat[0] = -90.0;
	lat[1] = 90.0;
	lon[0] = -180.0;
	lon[1] = 180.0;
	while (i < precision)
	{
		if (is_even)
		{
			mid = (lon[0] + lon[1]) / 2;
			if (longitude > mid)
			{
				ch |= bits[bit];
				lon[0] = mid;
			}
			else
			{
				lon[1] = mid;
			}
		}
		else
		{
			mid = (lat[0] + lat[1]) / 2;
			if (latitude > mid)
			{
				ch |= bits[bit];
				lat[0] = mid;
			}
			else
			{
				lat[1] = mid;
			}
		}

		is_even = !is_even;
		if (bit < 4)
		{
			bit++;
		}
		else
		{
			geohash[i++] = BASE32[ch];
			bit = 0;
			ch = 0;
		}
	}
	geohash[i] = 0;
}

void CGeoHash::DecodeGeoHash(char *geohash, double &latitude, double &longitude)
{
    bool is_even = true;
    double lat[2] = {-90.0, 90.0};
    double lon[2] = {-180.0, 180.0};
    double lat_err = 90.0;
    double lon_err = 180.0;
    int32_t geohash_length = strlen(geohash);

    latitude = 0;
    longitude = 0;

    for (int32_t i = 0; i < geohash_length; i++)
    {
            char c = geohash[i];
            int32_t cd = BASE32[strchr(BASE32, c) - BASE32];
            for (int32_t j = 0; j < (sizeof(bits) / sizeof(int32_t)); j++)
            {
                    int mask = bits[j];
                    if (is_even)
                    {
                            lon_err /= 2.0;
                            RefineInterval(lon, cd, mask);
                    }
                    else
                    {
                            lat_err /= 2.0;
                            RefineInterval(lat, cd, mask);
                    }
                    is_even = !is_even;
            }
    }
    latitude = (lat[0] + lat[1]) / 2.0;
    longitude = (lon[0] + lon[1]) / 2.0;

    double lat_precision = max(1, round(-log10(lat_err))) - 1;
    double lon_precision = max(1, round(-log10(lon_err))) - 1;

    latitude = GetPrecision(latitude, lat_precision);
    longitude = GetPrecision(longitude, lon_precision);
}

void CGeoHash::RefineInterval(double interval[], int32_t cd, int32_t mask)
{
    if ((cd & mask) > 0)
    {
            interval[0] = (interval[0] + interval[1]) / 2.0;
    }
    else
    {
            interval[1] = (interval[0] + interval[1]) / 2.0;
    }
}

double CGeoHash::GetPrecision(double x, double precision)
{
	double base = pow(10, -precision);
//	double diff = x % base;
	double diff = fmod(x, base);
	return x - diff;
}

string CGeoHash::CalculateAdjacent(string strSrcHash, int32_t dir)
{
//	strSrcHash = StringToLower(strSrcHash.c_str());
    char lastChr = strSrcHash[strSrcHash.size() - 1];
    int32_t type = (strSrcHash.size() % 2) == 1 ? ODD : EVEN;
    string strBase = strSrcHash.substr(0, strSrcHash.size() - 1);
    if(strchr(BORDERS[dir][type], lastChr) != NULL)
    {
    	CalculateAdjacent(strBase, dir);
    }

    int32_t index = strchr(NEIGHBORS[dir][type], lastChr) - NEIGHBORS[dir][type];
    return strBase + BASE32[index];
}

