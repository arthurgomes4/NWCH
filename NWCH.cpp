#include<NWCH.h>

NWCH::NWCH()                                //empty constructor, duh
{
  return;
}
NWCH::NWCH(int w, int* a)                    //set wheels and angles
{
  this->setSio("base",&Serial);
  this->setWheels(w);
  this->setAngles(a);
}
NWCH::NWCH(String Name,Stream* bridge)        //set sio parameters only
{
  this->setSio(Name,bridge);
}
NWCH::NWCH(String Name,Stream* bridge,int w,int* a)//set sio and wheels n angles
{
  this->setSio(Name,bridge);
  this->setWheels(w);
  this->setAngles(a);
}
//-------------------------------deg2rad----------------------------------------

float NWCH::deg2rad( float angle )       //converter function
{
  return( angle*0.017453293 );
}

//--------------------------------pins------------------------------------------

void NWCH::attachDirPins(int* dir)                   //attach dir pins array
{
  int i;
  for(i=0;i<this->wheels;i++)
  {
    dirPins[i] = dir[i];
  }
  for(i=0;i<this->wheels;i++)
  {
    pinMode(dirPins[i],OUTPUT);
  }
  this->serialMonitor("dir pins set");
}
void NWCH::attachPwmPins(int* pwm)                   //attach pwm pins array
{
  int i;
  for(i=0;i<this->wheels;i++)
  {
    pwmPins[i] = pwm[i];
  }
  for(i=0;i<this->wheels;i++)
  {
    pinMode(pwmPins[i],OUTPUT);
  }
  this->serialMonitor("pwm pins set");
}
void NWCH::attachPins(int* pwm,int* dir)  //attach both pwm and dir pins  arrays
{
  this->attachDirPins(dir);
  this->attachPwmPins(pwm);
}

//-------------------------setting functions-------------------------------------

void NWCH::setSio( String Name, Stream *bridge)       //set sio parameters
{
  this->serialName = Name;
  this->serialMonitorStream = bridge;
  this->serialMonitorStatus = true;
  this->serialMonitor("beginning transmission");
}
void NWCH::setRadii()               //create and attach radii array
{
  int i;
  for(i=0;i<this->wheels;i++)
  {
    this->radii[i] = 1.0;
  }

}
void NWCH::setRadii( float* r)       //modify radii array
{
  int i;
  for(i=0;i<this->wheels;i++)
  {
    this->radii[i] = r[i];
  }
  this->serialMonitor("radii updated");
}
void NWCH::setOffset(float off)       //modify offset, default offset = 0
{
  this->Offset = off;
  String msg="Offset set to ";
  msg.concat(off);
  this->serialMonitor(msg);
}
void NWCH::setWheels( int w )        //set number of wheels
{
  this->wheels = w;
  this->setRadii();
  this->setVelocities();
  this->serialMonitor("wheels set");
}
void NWCH::setAngles(int* a)          //attach angles array
{
  int i;
  for(i=0;i<this->wheels;i++)
  {
    this->angles[i] = a[i];
  }
  this->serialMonitor("angles set");
}
void NWCH::setDirections()            //create and attach motor directions array
{

  int i;
  for(i=0;i<this->wheels;i++)
  {
    this->directions[i] = true;
  }
}
void NWCH::setDirections( bool* dir)   //modify the motor directions array
{
  int i;
  for(i=0;i<this->wheels;i++)
  {
    this->directions[i] = dir[i];
  }
  this->serialMonitor("motor directions updated");
}
void NWCH::setVelocities()           //create and attach velocities array
{
  int i;
  for(i=0;i<this->wheels;i++)
  {
    this->velocities[i] = 0;
  }
}


//--------------------------------execute---------------------------------------

//calculates and writes to pins

void NWCH::execute( int botVelocity, float botAngle, int w )
{
  botAngle += this->Offset;
  float Vx = botVelocity*cos(deg2rad(botAngle)), Vy = botVelocity*sin(deg2rad(botAngle));
  int i;
  for(i=0;i<this->wheels;i++)
  {
    float vec = -1*Vx*sin(deg2rad(angles[i])) + Vy*cos(deg2rad(angles[i])) + radii[i]*w;
    this->velocities[i] = (int)vec;
    if(directions[i] == false)
    this->velocities[i] *= -1;
  }
  for(i=0;i<this->wheels;i++)
  {
    if(this->velocities[i]>=0)
    {
      digitalWrite(this->dirPins[i],HIGH);
      analogWrite(this->pwmPins[i],this->velocities[i]);
    }
    else
    {
      digitalWrite(this->dirPins[i],LOW);
      int v = -1*this->velocities[i];
      analogWrite(pwmPins[i],v);
    }
      if(this->serialMonitorStatus == false)
      continue;
      String msg="executing ";
      msg.concat(botVelocity);
      msg.concat(" ");
      msg.concat(botAngle);
      msg.concat(" ");
      msg.concat(w);
      this->serialMonitor(msg);
  }

}
