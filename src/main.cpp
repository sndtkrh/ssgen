#include <cstdio>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <tuple>
#include "star.hpp"
#include "argparser.hpp"
using namespace std;

extern double ST2sidereal( int year, int month, int day, double hour, double dh, double longitude );
extern double show( double hour );
extern double hms2h( double hour, double mini, double sec );
extern bool load_stars( vector<Star> &stars, string filename );
extern tuple<double,double,double> calc_xyz( Star star, double latitude, double sidereal_h );

void print_usage();
void get_time(int & year, int & month, int & day, int & hour, int & mini, int & sec, double & dh);

const string version = "0.1";
int main(int argc, char * argv[]){
  int year, month, day;
  int hour, mini, sec;
  double latitude = 35.026244, longitude = 135.780822;
  double dh = 9.0;
  string starfilename = "./data/hip.txt";
  string outputfilename = "";

  get_time( year, month, day, hour, mini, sec, dh );

  if( argc > 1 ){
    ArgParser argp( argc, argv, set<string>{"h", "t", "d", "c", "s", "o"});
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
    tie(f,o_arg) = argp.get_opt( "c" );
    if( f ) sscanf(o_arg.c_str(), "%lf,%lf", &longitude, &latitude );
    tie(f,o_arg) = argp.get_opt( "s" );
    if( f ) starfilename = o_arg;
    tie(f,o_arg) = argp.get_opt( "o" );
    if( f ) outputfilename = o_arg;
  }

  FILE *stream;
  if ( outputfilename.size() > 0 && (stream = freopen(outputfilename.c_str(), "w", stdout)) == NULL ) {
    fprintf(stderr, "Failed opening output file: %s\n", outputfilename.c_str() );
    return 1;
  }

  double sidereal_hour = ST2sidereal( year, month, day, hms2h(hour, mini, sec), dh, longitude );
  vector<Star> stars;
  if( ! load_stars( stars, starfilename ) ){
    fprintf(stderr, "Failed loading star data file: %s\n", starfilename.c_str() );
    return 0;
  }

  printf("# *** Starry Sky Generator *** #\n");
  printf("# version %s\n", version.c_str() );
  printf("# %04d/%02d/%02d %02d:%02d:%02d GMT%+.1f\n", year, month, day, hour, mini, sec, dh );
  printf("# longitude=%f\n", longitude );
  printf("# latitude=%f\n",  latitude );
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
  fprintf(stderr, "    -c\tlongitude,latitude\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "    -o\t<output file name>\n");
  fprintf(stderr, "    -s\t<star data file name>\n");
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
