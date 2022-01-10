#ifndef NWCH_H
#define NWCH_H
#include<Arduino.h>
#include<sio.h>

const int wheel_limit = 6;

class NWCH : public sio
{
  private:

  int wheels = wheel_limit;
  int angles[wheel_limit], pwmPins[wheel_limit], dirPins[wheel_limit], velocities[wheel_limit];
  bool directions[wheel_limit];
  float radii[wheel_limit];
  float Offset = 0;
  float deg2rad( float );

  public:

  NWCH();
  NWCH(int, int*);
  NWCH(String, Stream*);
  NWCH(String, Stream*, int, int*);

  void attachPins(int*,int*);
  void attachDirPins(int*);
  void attachPwmPins(int*);

  void setSio( String, Stream* );
  void setRadii();
  void setRadii( float* );
  void setOffset( float );
  void setWheels( int );
  void setAngles( int* );
  void setVelocities();
  void setDirections();
  void setDirections( bool* );

  void execute(int,float,int);
};

#endif
