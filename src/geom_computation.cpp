// Find intersection of two geodesics
// Compile with, e.g.,
// g++ -o intersect -I/usr/local intersect.cpp \
//    -lGeographic -Wl,-rpath=/usr/local/lib

#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/Gnomonic.hpp>

#include <cfloat>
#include <cmath>
#include "geom_computation.hpp"
class vector3 {
public:
  double _x, _y, _z;
  vector3(double x, double y, double z = 1) throw() : _x(x), _y(y), _z(z) {}
  vector3 cross(const vector3 &b) const throw() {
    return vector3(_y * b._z - _z * b._y, _z * b._x - _x * b._z,
                   _x * b._y - _y * b._x);
  }
  void norm() throw() {
    _x /= _z;
    _y /= _z;
    _z = 1;
  }
};

#define MAX_ITER 100
void intersection_line_line(double lata1, double lona1, double lata2,
                                  double lona2, double latb1, double lonb1,
                                  double latb2, double lonb2, double *latp,
                                  double *lonp) {

  const GeographicLib::Geodesic geod(GeographicLib::Constants::WGS84_a(),
                                     GeographicLib::Constants::WGS84_f());
  const GeographicLib::Gnomonic gn(geod);
  double lat0 = (lata1 + lata2 + latb1 + latb2) / 4,
         // Possibly need to deal with longitudes wrapping around
      lon0 = (lona1 + lona2 + lonb1 + lonb2) / 4;
  double err = DBL_MAX;
  int iter = 0;
  while (err > 1e-12 && iter<MAX_ITER) {
    iter++;
    double xa1, ya1, xa2, ya2;
    double xb1, yb1, xb2, yb2;
    gn.Forward(lat0, lon0, lata1, lona1, xa1, ya1);
    gn.Forward(lat0, lon0, lata2, lona2, xa2, ya2);
    gn.Forward(lat0, lon0, latb1, lonb1, xb1, yb1);
    gn.Forward(lat0, lon0, latb2, lonb2, xb2, yb2);
    // See Hartley and Zisserman, Multiple View Geometry, Sec. 2.2.1
    vector3 va1(xa1, ya1);
    vector3 va2(xa2, ya2);
    vector3 vb1(xb1, yb1);
    vector3 vb2(xb2, yb2);
    // la is homogeneous representation of line A1,A2
    // lb is homogeneous representation of line B1,B2
    vector3 la = va1.cross(va2);
    vector3 lb = vb1.cross(vb2);
    // p0 is homogeneous representation of intersection of la and lb
    vector3 p0 = la.cross(lb);
    p0.norm();
    double lat1, lon1;
    gn.Reverse(lat0, lon0, p0._x, p0._y, lat1, lon1);
    err = fabs(lat0 - lat1) + fabs(lon0 - lon1);
    lat0 = lat1;
    lon0 = lon1;
  }
  *latp = lat0;
  *lonp = lon0;
}


#define MAX_ITER 100

void projection_point_on_line(double lata1, double lona1, double lata2,
                                 double lona2, double latb, double lonb,
                                 double *latp, double *lonp) {
  const GeographicLib::Geodesic geod(GeographicLib::Constants::WGS84_a(),
                                     GeographicLib::Constants::WGS84_f());
  const GeographicLib::Gnomonic gn(geod);
  double lat0 = (lata1 + lata2 + latb) / 3,
         // Possibly need to deal with longitudes wrapping around
         lon0 = (lona1 + lona2 + lonb) / 3;

  double err = DBL_MAX;
  // printf("\n------\n", err);
  int iter = 0;
  while (err > 1e-12 && iter<MAX_ITER) {
    iter++;
    double xa1, ya1, xa2, ya2;
    double xb, yb;
    
    gn.Forward(lat0, lon0, lata1, lona1, xa1, ya1);
    gn.Forward(lat0, lon0, lata2, lona2, xa2, ya2);
    gn.Forward(lat0, lon0, latb, lonb, xb, yb);
    // See Hartley and Zisserman, Multiple View Geometry, Sec. 2.2.1
    vector3 va1(xa1, ya1);
    vector3 va2(xa2, ya2);
    vector3 vb(xb, yb);
    // la is homogeneous representation of line A1,A2
    // lb is homogeneous representation of line B1,B2
    vector3 la = va1.cross(va2);

    vector3 Fla(la);
    Fla._z = 0;

    vector3 p0 = (Fla.cross(vb)).cross(la);
    vector3 p0_u = p0;
    p0_u.norm();

    double lat1, lon1;
    gn.Reverse(lat0, lon0, p0_u._x, p0_u._y, lat1, lon1);
    err = fabs(lat0 - lat1) + fabs(lon0 - lon1);
    lat0 = lat1;
    lon0 = lon1;
  }

  *latp = lat0;
  *lonp = lon0;
}


void projection_point_on_circle(double latc, double lonc, double radius,
                                double latb, double lonb,
                                double *latp, double *lonp){
  const GeographicLib::Geodesic geod(GeographicLib::Constants::WGS84_a(),
                                     GeographicLib::Constants::WGS84_f());
  double azic, azib;
  geod.Inverse(latc, lonc, latb, lonb, azic, azib);
  geod.Direct(latc, lonc, latb, lonb, azic, radius, *latp, *lonp);
}

void turn_center(double latb, double lonb, double azib,
                 double turn_radius, int turn_dir,
                 double *latc, double *lonc){
  const GeographicLib::Geodesic geod(GeographicLib::Constants::WGS84_a(),
                                     GeographicLib::Constants::WGS84_f());

  geod.Direct(latb, lonb, azib + turn_dir*90.0, turn_radius, *latc, *lonc);
}

void turn_advance_angle(double latc, double lonc, double radius,
                        double latb, double lonb, double angle,
                        double *latp, double *lonp){
  const GeographicLib::Geodesic geod(GeographicLib::Constants::WGS84_a(),
                                     GeographicLib::Constants::WGS84_f());
  double azic, azib;
  geod.Inverse(latc, lonc, latb, lonb, azic, azib);
  geod.Direct(latc, lonc, azic + angle , radius, *latp, *lonp);
}

