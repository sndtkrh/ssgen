#include <bits/stdc++.h>
using namespace std;

double hms2h( double hour, double mini, double sec ){
  return hour + mini/60.0 + sec/3600.0;
}

double UT2sidereal( int year, int month, int day, double hour){
  int y = year + (month-3)/12;
  int m = (month-3) % 12;
  int d = day - 1;
  int n = d + (153 * m + 2) / 5 + 365 * y + y/4 - y/100 + y/400;
  double JD = n - 678881 + 2400001 - 0.5 + hour/24.0; // Julian day
  double TJD = JD - 2440000.5;
  double t = 0.671262 + 1.00273791 * TJD;
  double thetaG = 24.0 * ( t - floor(t) );
  return thetaG;
}

double ST2sidereal( int year, int month, int day, double hour, double dh, double longitude){
  double thetaG = UT2sidereal(year,month,day,hour-dh);
  double t = thetaG + longitude / 15.0;
  if( t < 0 ) t += 24.0;
  if( t > 24 ) t -= 24.0;
  return t;
}
