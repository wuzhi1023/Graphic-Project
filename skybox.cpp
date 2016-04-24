//
//  Skybox class
//
#include "skybox.h"
#include <iostream>
//
//  Constructor
//
Skybox::Skybox()
{
   sx = sy = sz = 1;
   glActiveTexture(GL_TEXTURE0);
   QString fileNames[6];
   fileNames[0] = ":/right.png";
   fileNames[1] = ":/left.png";
   fileNames[2] = ":/top.png";
   fileNames[3] = ":/bottom.png";
   fileNames[4] = ":/back.png";
   fileNames[5] = ":/front.png";
   glGenTextures(6,boxTex);
   for(int i=0;i<6;i++){
       glBindTexture(GL_TEXTURE_2D,boxTex[i]);
       QImage img(fileNames[i]);
       QImage rgba = QGLWidget::convertToGLFormat(img);
       glTexImage2D(GL_TEXTURE_2D,0,4,rgba.width(),rgba.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.bits());
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   }
}

//
//  Set scaling
//
void Skybox::scale(float dx,float dy,float dz)
{
   sx = dx;
   sy = dy;
   sz = dz;
}

//
//  Display the Skybox
//
void Skybox::display()
{
   glDisable(GL_LIGHTING);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   setTransform(sx,sy,sz);
   //  Texture on
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D,boxTex[4]);
   glEnable(GL_TEXTURE_2D);
   //  Front
   setColor(Color(1,1,1));
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   glBindTexture(GL_TEXTURE_2D,boxTex[5]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glBindTexture(GL_TEXTURE_2D,boxTex[0]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glBindTexture(GL_TEXTURE_2D,boxTex[1]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glBindTexture(GL_TEXTURE_2D,boxTex[2]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glBindTexture(GL_TEXTURE_2D,boxTex[3]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Texture off
   glDisable(GL_TEXTURE_2D);
   //  Undo transofrmations
   glPopMatrix();
   glEnable(GL_LIGHTING);
   //  Enable light 0
   glEnable(GL_LIGHT0);
}
