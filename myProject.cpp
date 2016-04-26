//
//  Final Project
//  Zhihao Wu
//  CSCI 5239 Spring 2016
//

#include <QApplication>
#include "myProjectviewer.h"

//
//  Main function
//
int main(int argc, char *argv[])
{
   //  Create the application
   QApplication app(argc,argv);
   //  Create and show view widget
   Ex11viewer view;
   view.show();
   //  Main loop for application
   return app.exec();
}
