#ifndef SKYBOX_H
#define SKYBOX_H

#include "Object.h"

class Skybox: public Object
{
private:
   float sx,sy,sz;  //  Dimensions in X/Y/Z directions
public:
   Skybox();                                  //  Constructor
   unsigned int boxTex[6];
   void scale(float dx,float dy,float dz);  //  Set scale
   void display();                          //  Render the cube
};

#endif // SKYBOX_H
