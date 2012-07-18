#ifndef __NODE__
#define __NODE__

//system includes
#include<QChar>
#include<QObject>

//forward dec's

//application includes

class Node : public QObject{
	Q_OBJECT
public:
	Node(QChar,int); //construct a new node
	~Node(); //delete this node
	
	//API
	void setDepth(int depth); 
	int GetDepth(){return (m_Depth);}
	Node * GetLeftChild(){return(m_pLeftChild);}
	Node * GetRightChild(){return(m_pRightChild);}
	void SetLeftChild(Node * pNode);
	void SetRightChild(Node * pNode); 
	Node * GetParent(){return(m_pParentNode);} 
	void SetParent(Node * pNode); 
	QChar GetChar(){return (m_Char);} 
	int GetFreq(){return (m_Freq);}
	void OutputNode(); //outputs the node to the console. 
	void SetOriginal(bool orig); 
	bool GetOriginal(){return(m_isOriginal);}
	Q_SLOT void UpdateDepth(); //connects to merged() of father nodes, this will ensure that depth of all nodes updates at every appropriate merging. Also emits merged() to ensure that depth updates keep passing all the way down.
	Q_SIGNAL void Merged();
	void SetOutput(bool);
	bool GetOutput(){return(m_isOutput);}
	void Merge();
	

private:
	Node * m_pParentNode; //this node's parent
	int m_Depth; //this node's depth in the BST
	QChar m_Char;
	Node *m_pLeftChild; //node's left child
	Node *m_pRightChild; //node's right child
	int m_Freq; //this node's frequency
	bool m_isOriginal; //bool value used to determine if node is original or artificial, used in Hu Tucker only	
	bool m_isOutput; //tracks if a node has been output. 
	
};

#endif
