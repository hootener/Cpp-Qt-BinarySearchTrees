TEMPLATE   = app
CONFIG	  += qt warn_on no_keywords embed_manifest_exe qt console
QT        +=
TARGET     = BST
SOURCES	  += main.cpp mainwindow.cpp node.cpp tree.cpp
HEADERS	  += mainwindow.h node.h tree.h
LIBS      +=


# Treat warnings as errors
win32:QMAKE_CXXFLAGS += /WX

CONFIG(debug, debug|release){
	# Debug build options
	# Enable a read-only console window (i.e. for printf etc.)
	# CONFIG   += console
}
else{
	# Release build options
	# Enable a read-only console window (i.e. for printf etc.)
	# CONFIG   += console
}