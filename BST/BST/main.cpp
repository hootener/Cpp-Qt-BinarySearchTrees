#include <QApplication>

#include "mainwindow.h"


int main(int argc, char ** argv){

	//create the application
	QApplication app(argc, argv);
	
	//create the main window
	MainWindow mainWindow;

	//show mainWindow and start the applicatoin
	mainWindow.show();
	return(app.exec());
}
