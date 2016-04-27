//
//  OpenGL widget
//
#include "myProjectopengl.h"
#include <QtOpenGL>
#include <QMessageBox>
#include "skybox.h"
#include "Mountain.h"
#include "WaveOBJ.h"
#include <math.h>
#include <iostream>
#define Cos(th) cos(M_PI/180*(th))
#define Sin(th) sin(M_PI/180*(th))
using namespace std;

const float wLevel = -2.6;
const float moveSpeed = 0.03;
float movement;

//
//  Constructor
//
Ex11opengl::Ex11opengl(QWidget* parent)
    : QGLWidget(parent)
{
   mode = 0;
   N = 1;
   init  = false;
   mouse = false;
   asp = 1;
   dim = 3;
   fov = 55;
   th = ph = 0;
   x0 = y0 = 0;
   z0 = 1;
   zh = 0;
   framebuf[0] = 0;
   framebuf[1] = 0;
   framebuf[2] = 0;
   framebuf[3] = 0;
}

//
//  Reset view
//
void Ex11opengl::reset()
{
   th = ph = 0;
   dim = 3;
   //  Request redisplay
   updateGL();
}

//
//  Light animation
//
void Ex11opengl::setLightMove(bool on)
{
   move = on;
   updateGL();
}

//
//  Set passes
//
void Ex11opengl::setPasses(int n)
{
   N = n;
   //  Request redisplay
   updateGL();
}

//
//  Set shader
//
void Ex11opengl::setShader(int sel)
{
   mode = sel;
   //  Request redisplay
   updateGL();
}

//
//  Set light position
//
void Ex11opengl::setPos(int Zh)
{
   zh = Zh;
   //  Request redisplay
   updateGL();
}

//
//  Set light elevation
//
void Ex11opengl::setElev(int Z)
{
   z0 = 0.02*Z;
   //  Request redisplay
   updateGL();
}

//
//  Set projection
//
void Ex11opengl::setPerspective(int on)
{
   fov = on ? 55 : 0;
   //  Request redisplay
   updateGL();
}

//
//  Initialize
//
void Ex11opengl::initializeGL()
{
   if (init) return;
   init = true;

   //  Load shaders
   Shader(shader[1],"",":/ex11a.frag");
   Shader(shader[2],"",":/ex11b.frag");
   Shader(shader[3],":/ex11c.vert",":/ex11c.frag");
   Shader(shader[4],":/ex11d.vert","");
   Shader(shader[5],":/ex11c.vert",":/ex11e.frag");

   glGenTextures(1,&waterDUDV);
   glBindTexture(GL_TEXTURE_2D,waterDUDV);
   QImage waterDD(":/waterDUDV.png");
   QImage rgba = QGLWidget::convertToGLFormat(waterDD);
   glTexImage2D(GL_TEXTURE_2D,0,4,rgba.width(),rgba.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,rgba.bits());
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

   glGenTextures(1,&waterNormal);
   glBindTexture(GL_TEXTURE_2D,waterNormal);
   QImage waterNM(":/normal.png");
   QImage NMrgba = QGLWidget::convertToGLFormat(waterNM);
   glTexImage2D(GL_TEXTURE_2D,0,4,NMrgba.width(),NMrgba.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,NMrgba.bits());
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);


   sky = new Skybox();
   sky->scale(40,40,40);
   sky->translate(0,-2,0);
   objects.push_back(sky);


   Mountain* moun = new Mountain();
   moun->texture(":/mountain.jpg");
   moun->translate(-5,-2,-5);
   objects.push_back(moun);


   // Cruiser
   WaveOBJ* tree=0;
   try
   {
      tree = new WaveOBJ("FR01a.obj",":/");
   }
   catch (QString err)
   {
      Fatal("Error loading object\n"+err);
   }
   if (tree)
   {
      tree->color(1,1,1);
      tree->translate(0.2,-2.6,-1.9);
      tree->rotate(-90,1,0,0);
      tree->scale(0.24);
      trees.push_back(tree);
   }

   tree=0;
    try
    {
     tree = new WaveOBJ("FR01a.obj",":/");
    }
    catch (QString err)
    {
     Fatal("Error loading object\n"+err);
    }
    if (tree)
    {
     tree->color(1,1,1);
     tree->translate(0.6,-2.9,-2.0);
     tree->rotate(-90,1,0,0);
     tree->scale(0.2);
     trees.push_back(tree);
    }

   tree=0;
   try
   {
     tree = new WaveOBJ("EA02y.obj",":/");
   }
   catch (QString err)
   {
     Fatal("Error loading object\n"+err);
   }
   if (tree)
   {
     tree->color(1,1,1);
     tree->translate(4,-2.6,1);
     tree->rotate(-90,1,0,0);
     tree->scale(0.06);
     trees.push_back(tree);
   }

   tree=0;
   try
   {
    tree = new WaveOBJ("EA02a.obj",":/");
   }
   catch (QString err)
   {
    Fatal("Error loading object\n"+err);
   }
   if (tree)
   {
    tree->color(1,1,1);
    tree->translate(4,-2.6,2);
    tree->rotate(-90,1,0,0);
    tree->scale(0.02);
    trees.push_back(tree);
   }

   tree=0;
   try
   {
    tree = new WaveOBJ("EA02a.obj",":/");
   }
   catch (QString err)
   {
    Fatal("Error loading object\n"+err);
   }
   if (tree)
   {
    tree->color(1,1,1);
    tree->translate(-2,-2.6,5.4);
    tree->rotate(-90,1,0,0);
    tree->scale(0.02);
    trees.push_back(tree);
   }

   //  Start 100 fps timer connected to updateGL
   move = false;
   timer.setInterval(17);
   connect(&timer,SIGNAL(timeout()),this,SLOT(updateGL()));
   timer.start();
   time.start();
}

//
//  Set projection when window is resized
//
void Ex11opengl::resizeGL(int width, int height)
{
   //  Window aspect ration
   asp = height ? width / (float)height : 1;
   //  Viewport is whole screen
   glViewport(0,0,width,height);
   //  Allocate frame buffer objects
   for (int k=0;k<4;k++)
   {
      //  Allocate frame buffer objects
      if (framebuf[k]) delete framebuf[k];
      framebuf[k] = new QGLFramebufferObject(width,height,QGLFramebufferObject::Depth);
   }
   dX = 1.0/width;
   dY = 1.0/height;
}

//
//  Draw the window
//
void Ex11opengl::paintGL()
{
   //  Wall time (seconds)
   float t = 0.001*time.elapsed();
   if (move) zh = fmod(90*t,360);
   float skyZh = fmod(t*0.3,360);
   float movement = fmod(t*moveSpeed,1);
   float moveCycle = fmod(t*2, 6.2831852);
   sky->rotate(skyZh,0,1,0);

   //  Set projection
   Projection();
   //  Set view
   glLoadIdentity();
   if (fov) glTranslated(0,0,-2*dim);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);

   //  Translate intensity to color vectors
   float Ambient[]  = {0.3,0.3,0.3,1.0};
   float Diffuse[]  = {0.8,0.8,0.8,1.0};
   float Specular[] = {1.0,1.0,1.0,1.0};
   float Position[] = {(float)(3*Cos(zh)),z0,(float)(3*Sin(zh)),1.0};
   //  Send output to framebuf[0]
//   if (mode) framebuf[0]->bind();

   {
       glDisable(GL_CLIP_PLANE0);
       //  Z-buffer
       glEnable(GL_DEPTH_TEST);
       //  Clear screen and depth buffer
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       //  Draw light position (no lighting yet)
       glColor3f(1,1,1);
//       ball(Position[0],Position[1],Position[2] , 0.1);
       //  OpenGL should normalize normal vectors
       glEnable(GL_NORMALIZE);
       //  Enable lighting
       glEnable(GL_LIGHTING);
       //  Enable light 0
       glEnable(GL_LIGHT0);
       //  Set ambient, diffuse, specular components and position of light 0
       glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
       glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
       glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
       glLightfv(GL_LIGHT0,GL_POSITION,Position);

       //  Draw scene

       glPushMatrix();
       glScaled(2,1,2);
       for (int k=0;k<objects.size();k++)
          objects[k]->display();

       shader[4].bind();
       shader[4].setUniformValue("movement" ,moveCycle);
       shader[4].setUniformValue("moveRange" ,(float)N);
       for (int k=0;k<trees.size();k++)
          trees[k]->display();
       shader[4].release();

    if(mode==2){
        glTranslatef(0.7,0,1.3);
        glBegin(GL_QUADS);
        glVertex3f(-3,wLevel,-3);
        glVertex3f(3,wLevel,-3);
        glVertex3f(3,wLevel,3);
        glVertex3f(-3,wLevel,3);
        glEnd();
    }
    if(mode!=1 && mode!=2){
       if(mode==5) shader[2].bind();
       else if(mode==6) shader[5].bind();
       else shader[3].bind();
       //  Texture
       glEnable(GL_TEXTURE_2D);
       glActiveTexture(GL_TEXTURE0);
       glBindTexture(GL_TEXTURE_2D,framebuf[2]->texture());
       glActiveTexture(GL_TEXTURE1);
       glBindTexture(GL_TEXTURE_2D,framebuf[3]->texture());
       glActiveTexture(GL_TEXTURE2);
       glBindTexture(GL_TEXTURE_2D,waterDUDV);
       glActiveTexture(GL_TEXTURE3);
       glBindTexture(GL_TEXTURE_2D,waterNormal);
       QVector2D dim(width(),height());
       if(mode==5) {
           shader[2].setUniformValue("Tex0" ,0);
           shader[2].setUniformValue("Tex1" ,1);
           shader[2].setUniformValue("dim",dim);
       }
       else if(mode==6) {
           shader[5].setUniformValue("Tex0" ,0);
           shader[5].setUniformValue("Tex1" ,1);
           shader[5].setUniformValue("waterDUDV" ,2);
           shader[5].setUniformValue("waterNormal" ,3);
           shader[5].setUniformValue("movement" ,movement);
           shader[5].setUniformValue("reflection",(float)(0.1+(float)abs(ph-90)/140.0));
           shader[5].setUniformValue("dim",dim);
       }
       else {
           shader[3].setUniformValue("Tex0" ,0);
           shader[3].setUniformValue("Tex1" ,1);
           shader[3].setUniformValue("waterDUDV" ,2);
           shader[3].setUniformValue("waterNormal" ,3);
           shader[3].setUniformValue("movement" ,movement);
           shader[3].setUniformValue("reflection",(float)(0.1+(float)abs(ph-90)/140.0));
           shader[3].setUniformValue("dim",dim);
       }
       glTranslatef(0.7,0,1.3);
       glBegin(GL_QUADS);
       glTexCoord2f(0,0);glVertex3f(-3,wLevel,-3);
       glTexCoord2f(2,0);glVertex3f(3,wLevel,-3);
       glTexCoord2f(2,2);glVertex3f(3,wLevel,3);
       glTexCoord2f(0,2);glVertex3f(-3,wLevel,3);
       glEnd();
       if(mode==5) shader[2].release();
       else if(mode==6) shader[5].release();
       else shader[3].release();
    }


       glPopMatrix();

       //  Disable lighting and depth
       glDisable(GL_LIGHTING);
       glDisable(GL_DEPTH_TEST);
   }

   
   //doing two more pass
   {
	//  Set projection
	Projection();
    //  Set view
	glLoadIdentity();
	if (fov) glTranslated(0,0,-2*dim);
	glRotated(ph,1,0,0);
	glRotated(th,0,1,0);
    framebuf[2]->bind();
    //Clipping Point
    glEnable(GL_CLIP_PLANE0);
    static const GLdouble equation[] = { 0.0, -1.0, 0.0, wLevel + 0.04 };
    glClipPlane(GL_CLIP_PLANE0, equation);
	//  Z-buffer
	glEnable(GL_DEPTH_TEST);
	//  Clear screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//  Draw light position (no lighting yet)
	glColor3f(1,1,1);
//	ball(Position[0],Position[1],Position[2] , 0.1);
	//  OpenGL should normalize normal vectors
	glEnable(GL_NORMALIZE);
	//  Enable lighting
	glEnable(GL_LIGHTING);
	//  Enable light 0
	glEnable(GL_LIGHT0);
	//  Set ambient, diffuse, specular components and position of light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
	glLightfv(GL_LIGHT0,GL_POSITION,Position);

	//  Draw scene
	glPushMatrix();
    glScaled(2,1,2);
	for (int k=0;k<objects.size();k++)
	objects[k]->display();
	glPopMatrix();

	//  Disable lighting and depth
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_CLIP_PLANE0);

    framebuf[2]->release();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(mode==3){
        shader[1].bind();

        //try to show the two new buffer
        //  Get the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,framebuf[2]->texture());
        //  Exercise shader
        glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(-1,-1);
        glTexCoord2f(1,0); glVertex2f(1,-1);
        glTexCoord2f(1,1); glVertex2f(1,1);
        glTexCoord2f(0,1); glVertex2f(-1,1);
        glEnd();
        shader[1].release();
    }
   }
   //one more
   {
	//  Set projection
	Projection();
	//  Set view
	glLoadIdentity();
	if (fov) glTranslated(0,0,-2*dim);
    glRotated(ph,1,0,0);
	glRotated(th,0,1,0);
    framebuf[3]->bind();
    //clipping plane
    glEnable(GL_CLIP_PLANE0);
    static const GLdouble equation[] = { 0.0, 1.0, 0.0, -wLevel + 0.04};
    glTranslated(0,2*wLevel,0);
    glScaled(1,-1,1);
    glClipPlane(GL_CLIP_PLANE0, equation);
	//  Z-buffer
	glEnable(GL_DEPTH_TEST);
	//  Clear screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//  Draw light position (no lighting yet)
	glColor3f(1,1,1);
//	ball(Position[0],Position[1],Position[2] , 0.1);
	//  OpenGL should normalize normal vectors
	glEnable(GL_NORMALIZE);
	//  Enable lighting
	glEnable(GL_LIGHTING);
	//  Enable light 0
	glEnable(GL_LIGHT0);
	//  Set ambient, diffuse, specular components and position of light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
	glLightfv(GL_LIGHT0,GL_POSITION,Position);

	//  Draw scene
	glPushMatrix();
    glScaled(2,1,2);
	for (int k=0;k<objects.size();k++)
	objects[k]->display();

    shader[4].bind();
    shader[4].setUniformValue("movement" ,moveCycle);
    shader[4].setUniformValue("moveRange" ,(float)N);
    for (int k=0;k<trees.size();k++)
       trees[k]->display();
    shader[4].release();

	glPopMatrix();

	//  Disable lighting and depth
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_CLIP_PLANE0);

    framebuf[3]->release();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    if(mode==4){
        //try to show the two new buffer
        //  Get the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,framebuf[3]->texture());
        shader[1].bind();
        //  Exercise shader
        glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(-1,-1);
        glTexCoord2f(1,0); glVertex2f(1,-1);
        glTexCoord2f(1,1); glVertex2f(1,1);
        glTexCoord2f(0,1); glVertex2f(-1,1);
        glEnd();
        shader[1].release();
    }

   }








   //  Emit angles to display
   emit angles(QString::number(th)+","+QString::number(ph));
   //  Emit light angle
   emit light((int)zh);
}

//
//  Throw a fatal error and die
//
void Ex11opengl::Fatal(QString message)
{
   QMessageBox::critical(this,"Ex11opengl",message);
   QApplication::quit();
}

//
//  Set OpenGL projection
//
void Ex11opengl::Projection()
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (fov)
   {
      float zmin = 0.1;
      float zmax = 1000;
      float ydim = zmin*tan(fov*M_PI/360);
      float xdim = ydim*asp;
      glFrustum(-xdim,+xdim,-ydim,+ydim,zmin,zmax);
   }
   else
      glOrtho(-dim*asp, +dim*asp, -dim, +dim, -dim, 4*dim);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

/******************************************************************/
/*************************  Mouse Events  *************************/
/******************************************************************/
//
//  Mouse pressed
//
void Ex11opengl::mousePressEvent(QMouseEvent* e)
{
   mouse = true;
   pos = e->pos();  //  Remember mouse location
}

//
//  Mouse released
//
void Ex11opengl::mouseReleaseEvent(QMouseEvent*)
{
    mouse = false;
}

//
//  Mouse moved
//
void Ex11opengl::mouseMoveEvent(QMouseEvent* e)
{
   if (mouse)
   {
      QPoint d = e->pos()-pos;  //  Change in mouse location
      th = (th+d.x())%360;      //  Translate x movement to azimuth
      ph = (ph+d.y())%360;      //  Translate y movement to elevation
      pos = e->pos();           //  Remember new location
      updateGL();               //  Request redisplay
   }
}

//
//  Mouse wheel
//
void Ex11opengl::wheelEvent(QWheelEvent* e)
{
   //  Zoom out
   if (e->delta()<0)
      dim += 0.1;
   //  Zoom in
   else if (dim>1)
      dim -= 0.1;
   //  Request redisplay
   updateGL();
}

//
//  Load shader
//
void Ex11opengl::Shader(QGLShaderProgram& shader,QString vert,QString frag)
{
   //  Vertex shader
   if (vert.length() && !shader.addShaderFromSourceFile(QGLShader::Vertex,vert))
      Fatal("Error compiling "+vert+"\n"+shader.log());
   //  Fragment shader
   if (frag.length() && !shader.addShaderFromSourceFile(QGLShader::Fragment,frag))
      Fatal("Error compiling "+frag+"\n"+shader.log());
   //  Link
   if (!shader.link())
      Fatal("Error linking shader\n"+shader.log());
}
