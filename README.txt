Provides some geometrical algorithm in WGS84

Documentation:

Compute the intersection P between lines A (A1, A2) and B (B1, B2)
void intersection_line_line(double lata1, double lona1, double lata2, double lona2,
                            double latb1, double lonb1, double latb2, double lonb2,
                            double *latp, double *lonp);

Compute the projection P of point B on lines A (A1, A2)
void projection_point_on_line(double lata1, double lona1, double lata2, double lona2,
                              double latb, double lonb,
                              double *latp, double *lonp);

Compute the projection P of the point B over the circle (C, radius)
void projection_point_on_circle(double latc, double lonc, double radius,
                                double latb, double lonb,
                                double *latp, double *lonp);


Compute the point P following a circular trajectory starting from B for an angle (angle) along the circle (C,radius)
void turn_advance_angle(double latc, double lonc, double radius,
                        double latb, double lonb, double angle,
                        double *latp, double *lonp);

Compute the center C of a circular trajectory given the different raidus parameters
void turn_center(double latb, double lonb, double azib,
                 double turn_radius, int turn_dir,
                 double *latc, double *lonc);

Dependencies
 - GeographicLib https://geographiclib.sourceforge.io/

Installation
 - Python wrapper: pyWrapper/README.txt