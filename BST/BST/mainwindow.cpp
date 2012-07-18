//System Includes
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextCursor>
#include <QList>
#include <iostream>
#include <QTime>

//Application Includes
#include "mainwindow.h"
#include "tree.h"

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent){
	SetupTextEdit();
	SetupGui();
	setWindowTitle("BST-erator");
	m_BstMethod = 0; //initialized to 0 for the OPTBST case. 
}

void MainWindow::SetupTextEdit(){
	//init the text editor
	m_pTextEdit = new QTextEdit;
	QTextCursor * pCursor = new QTextCursor(m_pTextEdit->document());
	pCursor->insertText("aaaaaaaaaaaabbbbbbbbcccccdddeeeeeeeee");
	//pCursor->insertText("Enter text or press Go to begin. Different BST algorithms can be selected from the combo box below. Use the Show Output button to see statistics from the BST's construction.");
}

void MainWindow::SetupGui(){
	//create components
	m_pGoButton = new QPushButton(tr("Go"));
	m_pGoButton->connect(m_pGoButton,SIGNAL(released()),this,SLOT(MakeFreqMap()));
	m_pShowOutput = new QPushButton(tr("Show Output"));
	
	//create and connect spinBox 
	m_pDistSpan = new QSpinBox(); 
	connect(m_pDistSpan,SIGNAL(valueChanged(int)),this,SLOT(SetSpinSpan(int))); 
	//create combo box
	m_pBSTComboBox = new QComboBox();
	m_pBSTComboBox->addItem("Optimal BST");
	m_pBSTComboBox->addItem("Middleweight BST");
	m_pBSTComboBox->addItem("Distance BST");
	m_pBSTComboBox->addItem("Hu-Tucker BST");
	connect(m_pBSTComboBox,SIGNAL(activated(int)),this,SLOT(SetBSTMethod(int))); 
	
	//layout components
	QHBoxLayout * pHBox = new QHBoxLayout; 
	pHBox->addWidget(m_pBSTComboBox);
	pHBox->addWidget(m_pGoButton);
	QVBoxLayout *pVBox = new QVBoxLayout;
	pVBox->addWidget(m_pTextEdit);
	pVBox->addLayout(pHBox);
	pVBox->addWidget(m_pDistSpan); 
	pVBox->addWidget(m_pShowOutput);
	QWidget * pWindowWidget = new QWidget(this);
	pWindowWidget->setLayout(pVBox);
	setCentralWidget(pWindowWidget); 
}

void MainWindow::SetSpinSpan(int spin){
	m_spinSpan = spin; 
}

void MainWindow::MakeFreqMap(){
	m_pFreqMap = new QHash<QChar,int>(); 
	QString plainText = m_pTextEdit->toPlainText() ; 
	if(!plainText.isEmpty())
	{
		QChar * data = plainText.data();
		while(!data->isNull())
		{
			//iterate through string, adding and incrementing frequencies.
			QHash<QChar, int>::iterator iter = m_pFreqMap->find(*data); 
			if(iter.key() == m_pFreqMap->end().key()){
				//key not found, insert new key and set its value to 1
				m_pFreqMap->insert(*data,1);
			}
			else{
				//the value was found, increment its frequency
				//this is an inefficient way to do this, consider redoing it.
				int newFreq = m_pFreqMap->take(*data) + 1; 
				m_pFreqMap->insert(*data,newFreq);		
			}
			//lastly, move one position through the string. 
			data++;
		}
	}
//Make Nodes from the freq map
	MakeNodes(m_pFreqMap);
	RunBST(); 
}

void MainWindow::MakeNodes(QHash<QChar,int> * map){
	QHash<QChar,int>::iterator iter = map->begin();
	while(iter.key() != map->end().key())
	{
		m_Nodes.append(new Node(iter.key(),iter.value()));
		iter++;
	}
}

//sets the method the program will use to generate the BST. Can be 1.OPTBST, 2.MidBST, 3.DistBST, or 4.HuBST
void MainWindow::SetBSTMethod(int method){
	m_BstMethod = method; 
}

void MainWindow::RunBST(){
	QTime aTimer; 
	int elapsed;
	int inputSize = m_Nodes.size();
	m_pBstTree = new Tree(m_Nodes);
	m_pBstTree->SetDistSpan(m_spinSpan); //sets span for DistBST
	switch(m_BstMethod){
		case 0:
			aTimer.start(); 
			m_pBstTree->RunOPTBST();
			elapsed = aTimer.elapsed();
			m_pBstTree->OutputHash();
			break;
		case 1:
			aTimer.start(); 
			m_pBstTree->RunMidBST();
			elapsed = aTimer.elapsed();
			m_pBstTree->OutputHash();
			break;
		case 2:
			aTimer.start();
			m_pBstTree->RunDistBST();
			elapsed = aTimer.elapsed(); 
			m_pBstTree->OutputHash();
			std::cout<<"K size: "<<((int)(inputSize/m_spinSpan))<<std::endl;
			break;
		case 3:
			aTimer.start(); 
			m_pBstTree->RunHuTuckerBST();
			elapsed = aTimer.elapsed();
		default:
			break;
	}
	std::cout<<"Input Size: "<<inputSize<<std::endl;
	std::cout<<"Elapsed Time: ";
	std::cout<<elapsed<<std::endl;
	//delete the tree
	delete m_pBstTree; 
	m_Nodes.clear();
}