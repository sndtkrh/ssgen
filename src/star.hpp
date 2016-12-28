
constexpr double PI = 3.14159265358979323846;
struct Star{
  int HIPnumber;
  double ra, dec; // radian
  double magnitude;
  Star();
  Star( int n, double ra_deg, double dec_deg, double magnitude )
    : HIPnumber(n), magnitude(magnitude) {
    ra  = 2 * PI * (ra_deg / 360.0);
    dec = 2 * PI * (dec_deg / 360.0);
  }
};

