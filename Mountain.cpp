//
//  Mountain class
//
#include "Mountain.h"
#include <QImage>
#include <iostream>
using namespace std;

//Loads a terrain from a heightmap.  The heights of the terrain range from
//-height / 2 to height / 2.
Terrain* loadTerrain(const char* filename, float height) {
   QImage image(filename);
   int Iwidth = image.width();
   int Iheight = image.height();
   Terrain* t = new Terrain(Iwidth, Iheight);
   for(int y = 0; y < Iheight; y++) {
      for(int x = 0; x < Iwidth; x++) {
         QRgb color = image.pixel(x,y);
//         cout<<"g"<<qGreen(color)<<",r"<<qRed(color)<<"b"<<qBlue(color)<<endl;
         float h = height * ((qBlue(color) / 255.0) - 0.5);
         t->setHeight(x, y, h);
      }
   }
   t->computeNormals();
   return t;
}

//
//  Constructor
//
Mountain::Mountain()
{
   sx = sy = sz = 0.05;
   _terrain = loadTerrain(":/mountainsBW.bmp", 30);
}

//
//  Set scaling
//
void Mountain::scale(float dx,float dy,float dz)
{
   sx = dx;
   sy = dy;
   sz = dz;
}

//
//  Display the cube
//
void Mountain::display()
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   setTransform(sx,sy,sz);
   setColor();
   //  Texture on
   EnableTex();

   for(int z = 0; z < _terrain->length() - 1; z++) {
    glBegin(GL_QUAD_STRIP);
    for(int x = 0; x < _terrain->width(); x++) {
       Vec3f normal = _terrain->getNormal(x, z);
       float fX = x;
       float fZ = z;
       glNormal3f(normal[0], normal[1], normal[2]);
       glTexCoord2f(fX/(_terrain->width()),fZ/(_terrain->length()));
       glVertex3f(fX, _terrain->getHeight(x, z), fZ);
       normal = _terrain->getNormal(x, z + 1);
       glNormal3f(normal[0], normal[1], normal[2]);
       glTexCoord2f((fX)/(_terrain->width()),(fZ+1.0)/(_terrain->length()));
       glVertex3f(fX, _terrain->getHeight(x, z + 1), fZ + 1.0);
    }
    glEnd();
   }



//   for(int z = 0; z < _terrain->length(); z++) {
//    for(int x = 0; x < _terrain->width(); x++) {
//       Vec3f normal = _terrain->getNormal(x, z);
//       cout<<"x"<<x<<'z'<<z<<endl;
//       cout<<"  normal"<<normal[0]<<","<<normal[1]<<","<<normal[2]<<endl;
//       cout<<"  text"<<x/(_terrain->width())<<","<<z/(_terrain->length())<<endl;
//       cout<<"  vert"<<x<<','<<_terrain->getHeight(x, z)<<','<<z<<endl;
//    }
//   }

   //  Texture off
   DisableTex();
   //  Undo transofrmations
   glPopMatrix();
}
