#include "tree.h"
#include <sstream>
#include <iostream>
#include <assert.h>
#include <QStack>

Tree::Tree(QList<Node*> nodes){
	m_Nodes = nodes;
}

Tree::~Tree(){
}

void Tree::RunOPTBST(){ //public accessor function for the optimal BST algorithm
	OPTBST(); 
}

void Tree::RunHuTuckerBST(){
	HuTuckerLCMPBST();
}

void Tree::RunDistBST(){
	DistBST(); 
}

void Tree::RunMidBST(){
	MidBST(); 
}

void Tree::OPTBST(){ //runs the optimal BST algorithm
	for(int i=0;i<m_Nodes.size();i++){ //initial for loop handles the arbitrary T_ii case. 
		scores subScores; 
		subScores.i = i;
		subScores.j = i;
		subScores.cost = m_Nodes.at(i)->GetFreq();
		subScores.weight = m_Nodes.at(i)->GetFreq();
		subScores.character = m_Nodes.at(i)->GetChar();//holds a pointer to the character associated with this node
		//stream in i and j to form unique entry into hash table
		std::ostringstream stm;
		stm<<i<<i;
		QString tempstr = QString::fromStdString(stm.str());
		//populate hash table with these entries.
		m_Scores.insert(tempstr,subScores);
	}
	//logic to handle the rest of the algorithm
	int span =1; //used to determine the ''span'' of a particular subproblem. the dist between i&j in T_ij
	for(;span<=m_Nodes.size();span++){ //check here for index errors
		scores subScores;
		for(int index = 0; index+span<m_Nodes.size();index++){ //loop gets solution for one optimal T_ij
			//initialize indices of subScore
			subScores.i = index;
			subScores.j = index + span;
			double optCost = 1000000; //some large arbitrary value
			for(int i=0;i<=span;i++){//loop calculates the individual subproblems of a T_ij
				double leftWeight(0);
				double rightWeight(0);
				double tempCost(0);
				std::ostringstream root;
				root<<index+i<<index+i;
				QHash<QString,scores>::iterator iter = m_Scores.find(QString::fromStdString(root.str()));
				tempCost+=iter.value().cost;
				if(i>0){ //if true, this is not the first subproblem, so calculate left problem
					std::ostringstream left;
					left<<index<<index +i-1;
					QString lft=QString::fromStdString(left.str());
					lft;
					iter = m_Scores.find(QString::fromStdString(left.str()));
					double tempcost = iter.value().cost;
					tempcost;
					if((index)==(index+i-1))//if T_ij is actually T_ii do 2*cost
						tempCost+=2*iter.value().cost;
					else //otherwise T_ij has a cost and weight that needs to be used instead
						tempCost+=iter.value().cost + iter.value().weight; 
					leftWeight = iter.value().weight;
				}
				if(i<span){ //if true, this is not the last subproblem, so calculate right problem
					std::ostringstream right;
					right<<index+i+1<<index + span;
					QString rght=QString::fromStdString(right.str());
					rght;
					iter = m_Scores.find(QString::fromStdString(right.str()));
					double tempcost = iter.value().cost;
					tempcost; 
					if((index+i+1)==(index+span))
						tempCost+=2*iter.value().cost;
					else
						tempCost+=iter.value().cost + iter.value().weight;
					rightWeight = iter.value().weight;
				}
				if(tempCost<optCost){//if temp is less than optimal, it is the new optimal and subScore needs to be updated. Constant time check
					optCost = tempCost;
					iter = m_Scores.find(QString::fromStdString(root.str())); //constant lookup of root node
					subScores.character = iter.value().character; //get char used in root of this subtree
					subScores.cost = optCost;
					subScores.weight = iter.value().weight + leftWeight + rightWeight; 
				}

			}
			//Add optimal T_ij to the QHash
			std::ostringstream stm;
			stm<<subScores.i<<subScores.j;
			m_Scores.insert(QString::fromStdString(stm.str()),subScores);
		}
		//span gets incremented here, and process begins for next T_ij
	}
}

void Tree::MidBST(){
	//algorithm works as follows: Iterate through list getting total weight
	// then iterate through list until weight is approximately = to half the weight.
	// select the element that occurs when the above is true and add it to the tree.
	//repeat until the list is empty 

	QList<Node *> treeList; 
	while(!m_Nodes.isEmpty())
	{
		double listWeight(0);
		for(int i=0;i<m_Nodes.size();i++){ //get the list's total weight
			listWeight+=m_Nodes.at(i)->GetFreq(); 
		}
		double midWeight(0);
		for(int i=0;i<m_Nodes.size();i++){//get the weighted median of the element
			midWeight+=m_Nodes.at(i)->GetFreq();
			if(midWeight>=(listWeight/2)){ //compare i and i -1 to see which is closer to the true middle. Take this as the weighted median
				if(i!=0){
					double leftWeight = m_Nodes.at(i-1)->GetFreq();
					double rightWeight = m_Nodes.at(i)->GetFreq(); 
					double leftDiff = abs(midWeight - leftWeight); 
					double rightDiff = abs(midWeight - rightWeight);
					if(leftDiff<rightDiff){//leftWeight is closer to true median, take that element
						treeList.append(m_Nodes.takeAt(i-1)); 
						break;
					}
					else{//take the right element
						treeList.append(m_Nodes.takeAt(i));
						break;
					}

				}
				else{
					treeList.append(m_Nodes.takeFirst());
				}
			}
		}
		if(treeList.size()>=1){
			for(int i=0;i<treeList.size()-1;i++){//check chidren of all elements except the last added to set parent of the last added
				if(treeList.last()->GetParent()==NULL){
					if(treeList.at(i)->GetLeftChild()==NULL){
						treeList.at(i)->SetLeftChild(treeList.last());
						treeList.last()->SetParent(treeList.at(i));
						break;
					}
					else if(treeList.at(i)->GetRightChild()==NULL){
						treeList.at(i)->SetRightChild(treeList.last());
						treeList.last()->SetParent(treeList.at(i));
						break;
					}
				}
			}
		}
	}
	//for printing 
	bool hasChild = true;
	Node * pNode = treeList.first(); 
	Node * pRoot = treeList.first(); 
	std::cout<<pRoot->GetFreq();
	std::cout<<pRoot->GetChar().toAscii(); 
	std::cout<<std::endl;
	while(hasChild == true){
		if(pNode->GetLeftChild() != NULL && pNode->GetLeftChild()->GetOutput() == false)
		{
			//output left child
			pNode = pNode->GetLeftChild(); 
			std::cout<<pNode->GetFreq();
			std::cout<<pNode->GetChar().toAscii(); 
			std::cout<<' ';
			pNode->SetOutput(true); 
		}
		else if(pNode->GetRightChild()!=NULL && pNode->GetRightChild()->GetOutput() == false)
		{
			//output right child
			std::cout<<' '; 
			pNode = pNode->GetRightChild(); 
			std::cout<<pNode->GetFreq();
			std::cout<<pNode->GetChar().toAscii(); 
			std::cout<<' ';
			pNode->SetOutput(true); 
		}
		else{ //node has no children, go back up to its parent
			pNode = pNode->GetParent(); 
			std::cout<<std::endl; 
		}
		if(pNode == pRoot && pNode->GetLeftChild()->GetOutput()==true && pNode->GetRightChild()->GetOutput()==true){ //everything is output, loop is done
			hasChild = false; 
		}

	}
}

void Tree::SetDistSpan(int span){
	m_distSpan = span; 
}
void Tree::DistBST(){
	//algorithm works as follows: find weighted median of list, use m_distSpan as a divisor of the list size. Take this quotient (rounded to an int) and search on each side of the weighted median
	//by this quotient. if this quotient calls for an index that is out of range, just use the first or last element in the list.
	QList<Node *> treeList; 
	Node * weightedMedian; 
	while(!m_Nodes.isEmpty())
	{
		int medianLocation(-1); 
		double listWeight(0);
		for(int i=0;i<m_Nodes.size();i++){ //get the list's total weight
			listWeight+=m_Nodes.at(i)->GetFreq(); 
		}
		double midWeight(0);
		for(int i=0;i<m_Nodes.size();i++){//get the weighted median of the element
			midWeight+=m_Nodes.at(i)->GetFreq();
			if(midWeight>=(listWeight/2)){ //compare i and i -1 to see which is closer to the true middle. Take this as the weighted median
				if(i!=0){
					double leftWeight = m_Nodes.at(i-1)->GetFreq();
					double rightWeight = m_Nodes.at(i)->GetFreq(); 
					double leftDiff = abs(midWeight - leftWeight); 
					double rightDiff = abs(midWeight - rightWeight);
					if(leftDiff<rightDiff){//leftWeight is closer to true median, take that element
						weightedMedian = m_Nodes.at(i-1); 
						medianLocation = i-1; 
						break;
					}
					else{//take the right element
						weightedMedian = m_Nodes.at(i);
						medianLocation = i;
						break;
					}

				}
				else{
					weightedMedian = m_Nodes.first();
					medianLocation = 0; 
				}
			}
		}
			//weighted median determined. Now look on either side of it for largest
			int spandist = m_Nodes.size() / m_distSpan;
			int leftIndex = medianLocation - spandist;
			int rightIndex = medianLocation+spandist;
			if(leftIndex<0) leftIndex = 0; //take first node if index is out of range
			if(rightIndex>m_Nodes.size()) rightIndex = m_Nodes.size()-1; //take last node if index is out of range
			int maxWeight(0); 
			int removalIndex(-1); 
			if(leftIndex ==rightIndex)removalIndex = leftIndex; 
			for(int j = leftIndex; j<rightIndex;j++){
				if(m_Nodes.at(j)->GetFreq() > maxWeight)
				{
					maxWeight = m_Nodes.at(j)->GetFreq();
					removalIndex = j;
				}
			}
			//finally, take the node at the removalIndex from the list and add it to the tree list, set parentage. 
			treeList.append(m_Nodes.takeAt(removalIndex)); 
			if(treeList.size()>=1){
			for(int i=0;i<treeList.size()-1;i++){//check chidren of all elements except the last added to set parent of the last added
				if(treeList.last()->GetParent()==NULL){
					if(treeList.at(i)->GetLeftChild()==NULL){
						treeList.at(i)->SetLeftChild(treeList.last());
						treeList.last()->SetParent(treeList.at(i));
						break;
					}
					else if(treeList.at(i)->GetRightChild()==NULL){
						treeList.at(i)->SetRightChild(treeList.last());
						treeList.last()->SetParent(treeList.at(i));
						break;
					}
				}
			}
		}
	}
	//for printing 
	bool hasChild = true;
	Node * pNode = treeList.first(); 
	Node * pRoot = treeList.first(); 
	std::cout<<pRoot->GetFreq();
	std::cout<<pRoot->GetChar().toAscii(); 
	std::cout<<std::endl;
	while(hasChild == true){
		if(pNode->GetLeftChild() != NULL && pNode->GetLeftChild()->GetOutput() == false)
		{
			//output left child
			pNode = pNode->GetLeftChild(); 
			std::cout<<pNode->GetFreq();
			std::cout<<pNode->GetChar().toAscii(); 
			std::cout<<' ';
			pNode->SetOutput(true); 
		}
		else if(pNode->GetRightChild()!=NULL && pNode->GetRightChild()->GetOutput() == false)
		{
			//output right child
			std::cout<<' '; 
			pNode = pNode->GetRightChild(); 
			std::cout<<pNode->GetFreq();
			std::cout<<pNode->GetChar().toAscii(); 
			std::cout<<' ';
			pNode->SetOutput(true); 
		}
		else{ //node has no children, go back up to its parent
			pNode = pNode->GetParent(); 
			std::cout<<std::endl; 
		}
		if(pNode == pRoot && pNode->GetLeftChild()->GetOutput()==true && pNode->GetRightChild()->GetOutput()==true){ //everything is output, loop is done
			hasChild = false; 
		}

	}

}

void Tree::HuTuckerBST(){
	//compatibility rule: you can only merge two blocks if there are no original blocks left between them
	//merge rule: merge x and y if x is the lowest frequency block compatible to y and y is the lowest frequency block compatible to x
	//copy m_Nodes to separate list where merging will occur
	QList<Node *> nodeList = m_Nodes;
	int listIndex = 0; //stores index of smallest freq node	
	for(int i=0;i<nodeList.size();i++) //loop will do merge until only all artifical nodes remain
	{
		double leastWeight = 1000000; //used for finding minimum freq in list 
		for(int iter=i+1;iter<nodeList.size();iter++){ //for loop to find index of least weight member
			if(nodeList.at(iter)->GetFreq()<leastWeight){
				leastWeight = nodeList.at(iter)->GetFreq();
				listIndex = iter; 		
			}
		}
		//node has been found that satisfies merge rule, now check for compatibility. No original node can exist between i and listIndex
		for(int jiter = i+1;jiter<=listIndex;jiter++){
			int origCount(0); //used to keep a count of all the ''original'' nodes left. If this is>1, merging does not reduce to the Huffman case. 
			//check here to see if any original nodes remain (this adds time to check the list equal to O(n), unfortunately.	
			for(int kiter = 0; kiter<nodeList.size();kiter++){
				if(nodeList.at(kiter)->GetOriginal() == true)
					origCount++;
				if(origCount >1) //prevents having to go through the entire list everytime
					break;
			}
			if(origCount > 1){
				if(nodeList.at(jiter)->GetOriginal() ==true && jiter!=listIndex){ //no compatible node was found, so no merge occurs
					break; //leave this loop
				}
				else if(nodeList.at(jiter)->GetOriginal() ==true &&jiter==listIndex){
					nodeList.replace(i,HuMerge(nodeList.at(i),nodeList.at(listIndex))); //a merge should occur and the loop should break (this should technically only occur on the last iteration of the loop anyways. Replace the left child with the new parent.
					//remove the right child from the list being considered.
					nodeList.removeAt(listIndex); 
					i=-1;
					break;
				}
			}
			else{ //only all artifical nodes +- 1 original node are left
				nodeList.replace(i,HuMerge(nodeList.at(i),nodeList.at(listIndex))); //a merge should occur and the loop should break (this should technically only occur on the last iteration of the loop anyways. Replace the left child with the new parent.
				//remove the right child from the list being considered.
				nodeList.removeAt(listIndex);
				i=-1;
				break;
			}
		}
	}
}

void Tree::HuTuckerLCMPBST(){
//uses LCMP to run hu tucker. Works as follows:
//find first compatible pair, get combined weight, store indices
//find next compatible pair, if combined weight is greater than previous, merge the previous pair. If not, store this pair's weight and indices, overwrite previous
//find next pair and repeat.
//caveats: if the size of the list ==2, merge the pairs regardless. If you reach end of list, compare weight of last pair to previous pair, take smallest as the LMCP
//caveats: only check nodes without parents. This means you can replace lowest index node in pair with merged father node and keep the other node. This node just won't be considered for grouping and is ''invisible'' to the algorithm
// this invisibility means the node stays in the list but isn't considered, this maintains accuracy of algorithm without altering list indices and causing a crash. 

	QList<Node *> nodeList = m_Nodes; //copy of original list, will have merged nodes inserted. 
	std::cout<<"Input List"<<std::endl;
	for(int i=0;i<m_Nodes.size();i++){
		std::cout<<m_Nodes.at(i)->GetChar().toAscii();
		std::cout<<m_Nodes.at(i)->GetFreq();
		std::cout<<std::endl; 
	}
	std::cout<<std::endl;
	std::cout<<"OABST"<<std::endl; 
	double mergedWeight(100000);
		int rightIndex(0);
		int leftIndex(0);
		for(int i=0;i<nodeList.size();i++){ //loop to select left node
			if(nodeList.size()<=2)
				break; //a simple merge is all that is required
			for(int j=i+1;j<nodeList.size();j++){ //loop to select compatible right node
				
				bool onOriginal = false; //this needs to be set to true when an original node is encountered, so that node is the LAST node considered for a lcmp
				if(nodeList.at(j)->GetOriginal() == true)
					onOriginal = true; 
				if(nodeList.at(j)->GetParent() == NULL){//grab this node,store it if the merged weight is a minimum
					if(((nodeList.at(i)->GetFreq()+nodeList.at(j)->GetFreq()) <= mergedWeight) && nodeList.at(i)!=nodeList.at(nodeList.size()-2)){ //this will do all compares except the very last
						mergedWeight = nodeList.at(i)->GetFreq()+nodeList.at(j)->GetFreq();
						rightIndex =j;
						leftIndex = i;
					}
					else if(nodeList.at(i)==nodeList.at(nodeList.size()-2)){ //condition that you are at the end of the list and no other comparisons are available. Need to compare the last element with the one previous.   
						double endWeight = nodeList.at(i)->GetFreq()+nodeList.at(j)->GetFreq();
						if(endWeight <= mergedWeight){//the last element in the list is the right index of the smallest pair and needs to be merged.
							rightIndex =j;
							leftIndex = i;
						}
						//now merge. This will either merge the last element in the list or the previous element, whichever made the smallest pairing with i. 
						nodeList.replace(leftIndex,HuMerge(nodeList.at(leftIndex),nodeList.at(rightIndex))); 
						nodeList.removeAt(rightIndex); //remove the right node from consideration.
						i = -1;
						mergedWeight = 100000;
						break;

					}
					else{ //the merged weight is greater than the previous, so the previous was a local minimum and we need to merge it 
						nodeList.replace(leftIndex,HuMerge(nodeList.at(leftIndex),nodeList.at(rightIndex))); 
						nodeList.removeAt(rightIndex); //remove the right node from consideration. 
						i= -1; //an appropriate lcmp was found, so we start the loop over without looking at the rest of the list 
						mergedWeight = 1000000; //mergedWeight is reset since a merge was made. 
						break; //leave this internal loop, you have merged the ith node. 
					}
				}
			
				else //no compatible node was found for this node.
					break;
				//need to break if you're on an original node. Since anything after an original node will NOT be compatible with this node 
				if(onOriginal ==true)
					break;
			}		
		}
	//redundant: check again to make sure nodeList.size() ==2, if so, merge the last two nodes regardless.
	if(nodeList.size() == 2){
		nodeList.replace(0,HuMerge(nodeList.at(0),nodeList.at(1)));
		nodeList.removeLast();
	}
	else{//assert, something's wrong
		assert(0);
	}
	//At this point, depths of the original node list have been determined and are correctly stored in m_Nodes, now tree must be reconstructed to remove any potential cross edges in the tree. 
	//reconstruction is as follows: 1. stack is empty, elements are in list with order preserved. 2. if stack.size()<2 go to 3, otherwise, check to see if top two elements of stack are equal. Go to 3 if they are different and 4 if they are equal.
	//3. remove element at beginning of list and put it on top of the stack, go to step 2.
	//4. let I_j be the top element of the stack and I_j-1 be the element just below I_j. Replace these two elements with (I_j)-1. Stop if (I_j)-1 =0. Otherwise, go to step 2.  This replacement in the stack results in a new node that is a father node to the combined elements. 
	QStack<Node *> nodeStack; //used to reconstruct the tree
	
	//Stack Algorithm: Hu-Tucker
	while(!m_Nodes.isEmpty() || nodeStack.size()>1){
		if(nodeStack.size()<2){
			nodeStack.push(m_Nodes.takeFirst()); 
		}
		else{
			if(nodeStack.top()->GetDepth() == nodeStack.at(nodeStack.size()-2)->GetDepth()){
				int newDepth = nodeStack.top()->GetDepth()-1; 
				nodeStack.push(HuConstruct(nodeStack.pop(),nodeStack.pop())); 
				nodeStack.top()->setDepth(newDepth); 
				if(newDepth <= 0) break;
			}
			else{
				nodeStack.push(m_Nodes.takeFirst());
			}
		}
	}

	//disconnect depth connections between nodes
	for(int i=0;i<m_Nodes.size();i++)
	{
		m_Nodes.at(i)->disconnect(); 
	}
	//at this point, tree is complete and can be shown by following the output from the single node left in the nodeStack.
	bool hasChild = true;
	Node * pNode = nodeStack.first(); 
	Node * pRoot = nodeStack.first(); 
	std::cout<<pRoot->GetFreq();
	std::cout<<pRoot->GetChar().toAscii(); 
	std::cout<<std::endl;
	while(hasChild == true){
		if(pNode->GetLeftChild() != NULL && pNode->GetLeftChild()->GetOutput() == false)
		{
			//output left child
			pNode = pNode->GetLeftChild(); 
			std::cout<<pNode->GetFreq();
			std::cout<<pNode->GetChar().toAscii(); 
			std::cout<<' ';
			pNode->SetOutput(true); 
		}
		else if(pNode->GetRightChild()!=NULL && pNode->GetRightChild()->GetOutput() == false)
		{
			//output right child
			std::cout<<' '; 
			pNode = pNode->GetRightChild(); 
			std::cout<<pNode->GetFreq();
			std::cout<<pNode->GetChar().toAscii(); 
			std::cout<<' ';
			pNode->SetOutput(true); 
		}
		else{ //node has no children, go back up to its parent
			pNode = pNode->GetParent(); 
			std::cout<<std::endl; 
		}
		if(pNode == pRoot && pNode->GetLeftChild()->GetOutput()==true && pNode->GetRightChild()->GetOutput()==true){ //everything is output, loop is done
			hasChild = false; 
		}

	}
}


void Tree::OutputHash(){ //outputs contents of hash table to console
	QList<QString> keys = m_Scores.keys();
	QList<scores> values = m_Scores.values();
	for(int i=0;i<keys.size();i++){
		std::cout<<"T_";
		std::cout<<keys.at(i).toStdString();
		std::cout<<"= ";
		std::cout<<values.at(i).cost<<",";
		std::cout<<values.at(i).weight<<",";
		std::cout<<values.at(i).character.toAscii()<<"  ";
	}
	std::cout<<std::endl; 
}

Node * Tree::HuMerge(Node * node1, Node * node2){
	//must make new node and set freq to sum of node1 and node2 freqs and set its depth to 0.
	//child depths will be set at node level based on an emitted ''parent merged'' signal.
	//so connect parent merged() signal to childupdatedepth() slot, this slot should also emit a merged() so its children get updated and so on.... 
	//have both nodes emit merged()
	//set new node as parent to these nodes. Set smaller weight node as left child to parent
	//and larger weight node as right child to parent
	//what to do about ties? Always build to the leftChild

	Node * parentNode = new Node(' ',node1->GetFreq()+node2->GetFreq()); //constructs new father node
	connect(parentNode,SIGNAL(Merged()),node1,SLOT(UpdateDepth()));
	connect(parentNode,SIGNAL(Merged()),node2,SLOT(UpdateDepth()));
	node1->SetParent(parentNode);
	node2->SetParent(parentNode);
	parentNode->SetLeftChild(node1);
	parentNode->SetRightChild(node2);
	parentNode->SetOriginal(false); //this parentNode is artificial. 
	parentNode->Merged(); 
	
	return(parentNode);

}

Node * Tree::HuConstruct(Node * node1, Node * node2){
	//used during reconstruction phase, since reconstruction algorithm handles depths. 
	Node * parentNode = new Node(' ',node1->GetFreq()+node2->GetFreq()); //constructs new father node
	node1->SetParent(parentNode);
	node2->SetParent(parentNode);
	parentNode->SetLeftChild(node1);
	parentNode->SetRightChild(node2);
	parentNode->SetOriginal(false); //this parentNode is artificial.
	return(parentNode);

}
