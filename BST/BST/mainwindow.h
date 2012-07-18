#ifndef __MAINWINDOW__
#define __MAINWINDOW__

//System Includes
#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QHash>
#include <QString>
#include <QChar>
#include <QSpinBox>

//Forward dec's
class QLabel;
class Tree;

//Application Includes
#include "node.h"

class MainWindow : public QMainWindow{
	Q_OBJECT

public:
	MainWindow(QWidget * parent = 0);
	void OutputFreqMap(); // outputs the freq map
protected:
	QTextEdit * m_pTextEdit; 
	QPushButton * m_pGoButton;
	QPushButton * m_pShowOutput; //output to file? Screen? Dialog?
	QComboBox * m_pBSTComboBox; 
	QHash<QChar, int> * m_pFreqMap; //map of character freqs
	Q_SLOT void SetBSTMethod(int method);
	Q_SLOT void SetSpinSpan(int spin); 
	QSpinBox * m_pDistSpan; 
private:
	void SetupTextEdit(); //inits the text editor 
	void SetupGui(); //inits the mainwindow gui
	Q_SLOT void MakeFreqMap(); //constructs a frequency map from the contents of m_pTextEdit;
	void MakeNodes(QHash<QChar,int> * map); //makes a set of nodes from a constructed map. 
	QList<Node *> m_Nodes; 
	int m_BstMethod;
	void RunBST(); //checks for the type of BST seclected then runs that algorithm 
	Tree * m_pBstTree; 
	int m_spinSpan; 
};

#endif
