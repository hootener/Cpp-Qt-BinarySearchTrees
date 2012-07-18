#include "node.h"
#include <iostream>

Node::Node(QChar symbol, int freq){
	m_Char = symbol;
	m_Freq = freq;
	m_isOriginal = true; 
	m_Depth = 0; 
	m_pParentNode = NULL;
	m_pLeftChild = NULL; 
	m_pRightChild = NULL; 
	m_isOutput = false;
}

Node::~Node(){
}

void Node::SetOriginal(bool orig){
	m_isOriginal = orig;
}


void Node::setDepth(int depth){
	m_Depth = depth;
}

void Node::SetLeftChild(Node *pNode){
	m_pLeftChild = pNode; 
}

void Node::SetRightChild(Node *pNode){
	m_pRightChild = pNode;
}

void Node::SetParent(Node *pNode){
	m_pParentNode = pNode;
}

//outputs the node's char and freq to the console.
void Node::OutputNode(){ 
	std::cout<<m_Char.toAscii()<<"\t"<<m_Freq<<std::endl;
}

void Node::UpdateDepth(){
	m_Depth++;
	Q_EMIT(Merged());
}

void Node::Merge(){
	Q_EMIT(Merged());
}

void Node::SetOutput(bool out){
	m_isOutput = out; 
}