#include <cstdio>
#include <vector>
#include <cmath>
#include <tuple>
#include <fstream>
#include "star.hpp"
using namespace std;

bool load_stars( vector<Star> &stars, string starfilename ){
  ifstream ifs( starfilename, ios::in );
  string line;
  if( ifs.fail() ){
    return false;
  }
  while( getline( ifs, line ) ){
    int hip_n;
    double ra_deg, dec_deg, magnitude;
    if( line[0] == '#' ) continue;
    for( char & c : line ) if( c == '|') c = ' ';
    sscanf( line.c_str(), "%d %lf %lf %lf", &hip_n, &magnitude, &ra_deg, &dec_deg);
    stars.emplace_back( hip_n, ra_deg, dec_deg, magnitude );
  }
  return true;
}

tuple<double,double,double> calc_xyz( Star star, double latitude, double sidereal_h ){
  double h = (sidereal_h / 24.0) * 2 * PI;
  double l = (latitude / 360.0)  * 2 * PI;
  double x, y, z;
  x = cos(l) * sin(star.dec) - sin(l) * cos(star.dec) * cos(h-star.ra);
  y = -cos(star.dec) * sin(h-star.ra);
  z = sin(l) * sin(star.dec) + cos(l) * cos(star.dec) * cos(h-star.ra);
  return tuple<double,double,double>(x,y,z);
}
