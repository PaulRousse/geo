#ifndef H_GEODESIC_COMPUTATION
#define H_GEODESIC_COMPUTATION

#ifdef __cplusplus
extern "C" {
#endif

void intersection_line_line(double lata1, double lona1, double lata2,
                                  double lona2, double latb1, double lonb1,
                                  double latb2, double lonb2,
                                  double *latp, double *lonp);

void projection_point_on_line(double lata1, double lona1, double lata2,
                                 double lona2, double latb, double lonb,
                                 double *latp, double *lonp);

void projection_point_on_circle(double latc, double lonc, double radius,
                                double latb, double lonb,
                                double *latp, double *lonp);


void turn_advance_angle(double latc, double lonc, double radius,
                        double latb, double lonb, double angle,
                        double *latp, double *lonp);

void turn_center(double latb, double lonb, double azib,
                 double turn_radius, int turn_dir,
                 double *latc, double *lonc);


#ifdef __cplusplus
}
#endif

#endif