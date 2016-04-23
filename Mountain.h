#ifndef MOUNTAIN_H
#define MOUNTAIN_H

#include "Object.h"
#include "terrain.h"

class Mountain: public Object
{
private:
   Terrain* _terrain;
   float sx,sy,sz;  //  Dimensions in X/Y/Z directions
public:
   Mountain();                                  //  Constructor
   void scale(float dx,float dy,float dz);  //  Set scale
   void display();                          //  Render the cube
};

#endif // MOUNTAIN_H
