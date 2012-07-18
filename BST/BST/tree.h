//System Includes
#include "node.h"
#include <QList>
#include <QHash>

class Tree : public QObject{
	Q_OBJECT
public:
	Tree(QList<Node *> nodes); //populates nodelist of this tree
	~Tree(); //destroys a tree
	//run functions...call the appropriate BST function
	void RunOPTBST();
	void RunMidBST();
	void RunDistBST();
	void RunHuTuckerBST(); 
	void OutputHash();
	void SetDistSpan(int span); 

private:
	void OPTBST();
	void MidBST();
	void DistBST(); 
	void HuTuckerBST(); 
	void HuTuckerLCMPBST(); //runs hutucker using the idea of local compatible minimum pairs
	QList<Node *> m_Nodes;
	struct scores{
		int i;
		int j;
		double cost;
		double weight;
		QChar character;
	};

	QHash<QString,scores> m_Scores;

	Node * HuMerge(Node * node1, Node * node2);
	Node * HuConstruct(Node * node1, Node * node2);
	QList<QList<Node *> > m_HuTree; //holds final hu tree arranged by level from left to right. 
	int m_distSpan; 
	
};

