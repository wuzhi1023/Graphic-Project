//
//  Ex11viewer Widget
//

#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "myProjectviewer.h"

//
//  Constructor
//
Ex11viewer::Ex11viewer()
{
   //  Set window title
   setWindowTitle(tr("Final Project:  Zhihao Wu"));

   //  Create new OpenGL widget
   ogl = new Ex11opengl;

   //  Select shader
   QComboBox* shader = new QComboBox();
   shader->addItem("All");
   shader->addItem("No Water");
   shader->addItem("Water Quad");
   shader->addItem("Refraction Buffer");
   shader->addItem("Reflection Buffer");
   shader->addItem("No Wave");
   shader->addItem("Simple Wave");
   shader->addItem("Sun Reflection");

   //  View angle and zoom
   QLabel* angles = new QLabel();

   QSpinBox* passes = new QSpinBox();
   passes->setRange(1,10);

//   //  Pause/resume button
//   light = new QPushButton("Pause");

   //  Reset
   QPushButton* rst = new QPushButton("Reset");
   //  Quit
   QPushButton* quit = new QPushButton("Quit");

   //  Set layout of child widgets
   QGridLayout* layout = new QGridLayout;
   layout->addWidget(ogl,0,0,9,1);
   layout->addWidget(new QLabel("Steps"),0,1);
   layout->addWidget(shader,0,2);
   layout->addWidget(new QLabel("Tree Movement"),1,1);
      layout->addWidget(passes,1,2);
   layout->addWidget(new QLabel("Angles"),2,1);
   layout->addWidget(angles,2,2);
   layout->addWidget(rst,8,1);
   layout->addWidget(quit,8,2);
   //  Manage resizing
   layout->setColumnStretch(0,100);
   layout->setColumnMinimumWidth(0,100);
   layout->setRowStretch(7,100);
   setLayout(layout);

   connect(shader,SIGNAL(currentIndexChanged(int))    , ogl,SLOT(setShader(int)));
   connect(passes,SIGNAL(valueChanged(int))           , ogl,SLOT(setPasses(int)));
   connect(ogl,SIGNAL(angles(QString)) , angles,SLOT(setText(QString)));
   connect(rst  ,SIGNAL(pressed()),ogl,SLOT(reset()));
   connect(quit,SIGNAL(pressed()) , qApp,SLOT(quit()));
}

//
//  Light pause/move
//
void Ex11viewer::lmove()
{
}
