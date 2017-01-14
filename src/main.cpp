#include <cstdio>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <tuple>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "star.hpp"
#include "argparser.hpp"
using namespace std;

extern double ST2sidereal( int year, int month, int day, double hour, double dh, double longitude );
extern double show( double hour );
extern double hms2h( double hour, double mini, double sec );
extern bool load_stars( vector<Star> &stars, string filename );
extern tuple<double,double,double> calc_xyz( Star star, double latitude, double sidereal_h );

int output(vector<Star> & stars, string outputfilename,
	   int year, int month, int day, int hour, int mini, int sec, double dh,
	   double latitude, double longitude, bool printinfo);
void print_usage();
void get_time(int & year, int & month, int & day, int & hour, int & mini, int & sec, double & dh);
vector<string> split( string str, char c );

const string version = "0.1";
int main(int argc, char * argv[]){
  int year, month, day;
  int hour, mini, sec;
  double latitude = 35.026244, longitude = 135.780822;
  double dh = 9.0;
  string starfilename = "./data/hip.txt";
  string outputfilename = "";
  string citiesdatafilename = "./data/cities_data.txt";
  string city_name = "";

  int n = 1; // number of output
  int interval = 3600;

  get_time( year, month, day, hour, mini, sec, dh );

  if( argc > 1 ){
    ArgParser argp( argc, argv, set<string>{"h", "t", "d", "p", "s", "o", "c", "f", "n", "i"});
    if( ! argp.success ){
      fprintf(stderr, "%s\n", argp.err_msg.c_str() );
      return 1;
    }
    bool f;
    string o_arg;
    tie(f,o_arg) = argp.get_opt( "h" );
    if( f ){ print_usage(); return 0; }
    tie(f,o_arg) = argp.get_opt( "t" );
    if( f ) sscanf(o_arg.c_str(), "%d/%d/%d,%d:%d:%d", &year, &month, &day, &hour, &mini, &sec);
    tie(f,o_arg) = argp.get_opt( "d" );
    if( f ) sscanf(o_arg.c_str(), "%lf", &dh);
    tie(f,o_arg) = argp.get_opt( "p" );
    if( f ) sscanf(o_arg.c_str(), "%lf,%lf", &longitude, &latitude );
    tie(f,o_arg) = argp.get_opt( "s" );
    if( f ) starfilename = o_arg;
    tie(f,o_arg) = argp.get_opt( "o" );
    if( f ) outputfilename = o_arg;
    tie(f,o_arg) = argp.get_opt( "f" );
    if( f ) citiesdatafilename = o_arg;
    tie(f,o_arg) = argp.get_opt( "c" );
    if( f ){
      city_name = o_arg;
      ifstream ifs( citiesdatafilename, ios::in );
      string line;
      bool success = false;
      if( ifs.fail() ){
	fprintf(stderr, "Failed opening cities data file: %s\n", citiesdatafilename.c_str() );
	return 1;
      }
      while( getline( ifs, line ) ){
	if( line[0] == '#' ) continue;
	vector<string> data = split( line, ',' );
	if( data[0] == city_name ){
	  sscanf( data[1].c_str(), "%lf", &longitude );
	  sscanf( data[2].c_str(), "%lf", &latitude );
	  sscanf( data[3].c_str(), "%lf", &dh );
	  success = true;
	  break;
	}
      }
      if( ! success ){
	fprintf(stderr, "No such a city: %s\n", city_name.c_str() );
	return 1;
      }
    }
    tie(f,o_arg) = argp.get_opt( "n" );
    if( f ) sscanf(o_arg.c_str(), "%d", &n );
    tie(f,o_arg) = argp.get_opt( "i" );
    if( f ) sscanf(o_arg.c_str(), "%d", &interval );
  }

  vector<Star> stars;
  if( ! load_stars( stars, starfilename ) ){
    fprintf(stderr, "Failed loading star data file: %s\n", starfilename.c_str() );
    return 1;
  }

  int ret;
  if( n == 1 ){
    ret = output( stars, outputfilename, year, month, day, hour, mini, sec, dh, latitude, longitude, true );
  }else{
    for(int i = 0; i < n; i++){
      ostringstream sout;
      sout << std::setfill('0') << std::setw(6) << i;
      string number = sout.str();
      string ofname = outputfilename + number;
      ret = output( stars, ofname, year, month, day, hour, mini, sec + interval * i, dh, latitude, longitude, false );
      if( ret ) break;
    }
  }

  return ret;
}

int output(vector<Star> & stars, string outputfilename,
	   int year, int month, int day,
	   int hour, int mini, int sec, double dh,
	   double latitude, double longitude, bool printinfo){
  FILE *stream;
  if ( outputfilename.size() > 0 && (stream = freopen(outputfilename.c_str(), "w", stdout)) == NULL ) {
    fprintf(stderr, "Failed opening output file: %s\n", outputfilename.c_str() );
    return 1;
  }
  double sidereal_hour = ST2sidereal( year, month, day, hms2h(hour, mini, sec), dh, longitude );
  if( printinfo ){
    printf("# *** ssgen ***\n");
    printf("# version %s\n", version.c_str() );
    printf("# %04d/%02d/%02d %02d:%02d:%02d UTC%+.1f\n", year, month, day, hour, mini, sec, dh );
    printf("# longitude=%f\n", longitude );
    printf("# latitude=%f\n",  latitude );
  }
  for( auto star : stars ){
    double x, y, z;
    tie(x,y,z) = calc_xyz( star, latitude, sidereal_hour );
    printf("%d %f %f %f %f\n", star.HIPnumber, x, y, z, star.magnitude );
  }
  return 0;
}
void print_usage(){
  fprintf(stderr, "Usage: ssgen [options]\n");
  fprintf(stderr, "    -t\tyyyy/mm/dd,hour:min:sec\n");
  fprintf(stderr, "    -d\t<time differencial>\n");
  fprintf(stderr, "    -p\tlongitude,latitude\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "    -o\t<output file name>\n");
  fprintf(stderr, "    -s\t<star data file name>\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "    -f\t<cities data file name>\n");
  fprintf(stderr, "    -c\t<city name>\n" );
  fprintf(stderr, "\n");
  fprintf(stderr, "    -h\t(help)\n");
  fprintf(stderr, "\n");
  return;
}
void get_time(int & year, int & month, int & day, int & hour, int & mini, int & sec, double & dh){
    time_t now = time(NULL);
    struct tm *pnow = localtime(&now);
    struct tm *gmnow = gmtime(&now);
    year =  pnow->tm_year+1900;
    month = pnow->tm_mon + 1;
    day = pnow->tm_mday;
    hour = pnow->tm_hour;
    mini = pnow->tm_min;
    sec = pnow->tm_sec;
    int gm_hour = gmnow->tm_hour;
    int gm_min = gmnow->tm_min;
    if( gmnow->tm_mday < day ){
      dh = 24.0 + hour - gm_hour + (mini - gm_min) / 60.0;
    }else if( gmnow->tm_mday > day ){
      dh = hour - (gm_hour + 24.0) + (mini - gm_min) / 60.0;
    }else{
      dh = hour - gm_hour + (mini - gm_min) / 60.0;
    }
    return;
}
vector<string> split( string str, char c ){
  vector<string> res;
  int p = 0;
  if( str[ str.size()-1 ] != c ) str += c;
  for(unsigned int i = 0; i < str.size(); i++){
    if( str[i] == c ){
      res.emplace_back( str.substr( p, i ) );
      p = i + 1;
    }
  }
  return res;
}
