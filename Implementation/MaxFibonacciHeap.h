#include <string>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include<map>
#include <sstream>

using namespace std;

/**
 * Definition of Node structure.
*/

struct Node{
	int data;
	int degree;
	string keyWord;
	Node* child;
	Node* leftSibling;
	Node* rightSibling;
	Node* parent;
	bool childCut;
};

/**
 * Max finonacci heap class which implements the following methods.
 * insert
 * remove max
 * cut
 * cascading cut
 * make parent and child
*/

class MaxFibonacciHeap{
        public:
                int numberOfNodes;
                Node *maxNode;
                MaxFibonacciHeap();
                Node* createNode(string,int);
                void insert(Node*);             
                void cut(Node*);
                void cascadingCut(Node*);
                void increaseKey(Node*,int);      
                Node* removeMax();                     
                void makeParentAndChild(Node*,Node*);      
};

/*
Constructor
Initializes maximum node to NULL
Initializes number of nodes to 0
*/
MaxFibonacciHeap::MaxFibonacciHeap(){
	maxNode=NULL; 
	numberOfNodes=0;
}

/**
 *  Returns the newly created node.
 *  @param k, The keyword.
 *  @param v, number of the occurences of the given keyword. 
 *  @return nn, newly created node.
*/
Node* MaxFibonacciHeap::createNode(string k,int v){
	Node* nn=new Node();
    nn->degree=0;
	nn->keyWord=k;
    nn->data=v;
    nn->child=NULL;
	nn->leftSibling=nn;				
	nn->rightSibling=nn;	
    nn->parent=NULL;
    nn->childCut=false;
	return nn;
}

/**
 * Function responsible for node insertion into fibonacci heap.
 * @param nn, The node to be inserted.
 * @return nothing
*/
void MaxFibonacciHeap::insert(Node* nn){
	if(maxNode==NULL){
		maxNode=nn;
	}else{
		Node *t=maxNode->rightSibling;
		maxNode->rightSibling=nn;
		nn->leftSibling=maxNode;
		nn->rightSibling=t;
		t->leftSibling=nn;
	}
	if(nn->data > maxNode->data){			
		maxNode=nn;
	}
	numberOfNodes++;			
}

/**
 * Function responsible for increasing the number of occurences for a given node.
 * @param node, pointer to the node whose value has to be increased.
 * @param val, amount by which the value should be increased.
 * @return nothing.
*/
void MaxFibonacciHeap::increaseKey(Node* node,int val){
	node->data+=val;
	if(node->parent!=NULL && node->data > node->parent->data){
		Node* p=node->parent; 
        //Removes the node from its parent and adds it to the root list
		cut(node);
        //recursively check till you find a parent who has childcut as false
		cascadingCut(p);
	}
	if(node->data > maxNode->data){
		maxNode=node;
	}
}

/**
 * Function responsible for removing the node from its parent and adding it to the root list.
 * @param *node, pointer to the node which has to be removed from its parent's child list and be put in root list.
 * @return nothing.
*/
void MaxFibonacciHeap::cut(Node* node){
	if(node->parent->degree==1){
        //check if this is the only child of its parent
		node->parent->child=NULL;
		node->parent->degree--;
	}else{
        //This is not the only child
		Node* left=node->leftSibling;
		Node* right=node->rightSibling;
		left->rightSibling=right;
		right->leftSibling=left;
		if(node->parent->child == node){ 		
			node->parent->child=left;
        }
		node->parent->degree--;
	}
    //adding the node to the right of root in rootlist
	Node* next=maxNode->rightSibling;
	maxNode->rightSibling=node;
	node->leftSibling=maxNode;
	node->rightSibling=next;
	next->leftSibling=node;
	node->childCut=false; 		
	node->parent=NULL;
}	

/**
 * Function which recursively iterate over parents till you find childcut value as false,
 * Sets childcut to true when encounters a false
 * @param *node, pointer to the node from which cascade cut have to be performed.
 * @return nothing.
*/
void MaxFibonacciHeap::cascadingCut(Node* node){
	if(node->parent!=NULL){	
		if(node->childCut==false){
			node->childCut=true;
        }else{
			Node *parent=node->parent; 
			cut(node);
			cascadingCut(parent);
		}
	}
}


/**
 * Function which removes the max element pointer from root list and returns the pointer to the caller.
 * After removing the max elemnt, it also performs pair wise combine of nodes which have 2 same degrees.
 * @param, no parameters.
 * @return, Pointer to the maxmium/removed node.
*/
Node* MaxFibonacciHeap::removeMax(){
    //Keep the max pointer node
	Node *ptr=maxNode; 			

	if(maxNode!=NULL){
        //Heap is not empty
		Node *leftSibling=maxNode->leftSibling;
		Node *rightSibling=maxNode->rightSibling;
		if(leftSibling!=maxNode){
            //Heap has more than one node in root list
			Node *curChild=maxNode->child;
			if(curChild!=NULL){
				Node *firstChild=maxNode->child; 
				Node *lastChild=firstChild->leftSibling;
				leftSibling->rightSibling=firstChild;
				firstChild->leftSibling=leftSibling;
				rightSibling->leftSibling=lastChild;
				lastChild->rightSibling=rightSibling;
                //Update children's values-> make parent as NULL and childcut as false
				for(int i=maxNode->degree;i>0;i--){
					curChild->childCut=false; 	
					curChild->parent=NULL;
					curChild=curChild->rightSibling;
				}
			}else{
                //MaxNode has no children
				leftSibling->rightSibling=rightSibling;
				rightSibling->leftSibling=leftSibling;
			}
			maxNode=rightSibling;	//Actual max node will be obtained in pairwise function	
		}else{
            //Heap has only one root
			Node* curChild=maxNode->child;
			Node* firstChild=maxNode->child;
            //Update children's values-> make parent as NULL and childcut as false
			if(curChild!=NULL){
				do{	
					curChild->parent=NULL;
					curChild->childCut=false;
					curChild=curChild->rightSibling;
				}while(curChild!=firstChild);
			}
			maxNode=curChild;
		}
		//Taking care that No two nodes have same degree.
		int len=numberOfNodes; // Degree table can never have more elements than the number of nodes.
        Node **arr=new Node*[len]; //Degreetable
	    for(int i=0;i<len;i++){		//Initialize to null
		    arr[i]=NULL;
        }
	    if(maxNode!=NULL){		
			//Create a vector and put all the nodes of root list in it
		    vector<Node*> allNodes;
            Node* curNode=maxNode;
		    do{
			    allNodes.push_back(curNode);
			    curNode=curNode->rightSibling;
		    }while(curNode!=maxNode);
			//Traverse through the vector 
            for(vector<Node*>::iterator it = allNodes.begin() ; it != allNodes.end(); ++it){
			    curNode=*it;
			    Node* temp=curNode;
			    int degree=curNode->degree;
			    while(arr[degree]!=NULL){
				    Node *otherNode = arr[degree]; //There is already a node with same degree
				    //Make them parent and children
					if(otherNode->data > curNode->data){
					    temp=otherNode;
					    makeParentAndChild(otherNode,curNode);	
				    }else{
					    temp=curNode;
					    makeParentAndChild(curNode,otherNode);	
				    }
				    arr[degree]=NULL; 	
				    degree++;
				    curNode=temp;
			    }
				//above loop comes out when there are no two nodes with two false
			    arr[degree]=temp;
		    }
	    }
		//Reset the max node
	    maxNode=NULL; 
	    for(int i=0;i<len;i++){
		    if(arr[i]!=NULL){
			    if(maxNode==NULL){
				    maxNode=arr[i];
                }else if(arr[i]->data > maxNode->data){
				    maxNode=arr[i];
                }
		    }
	    }
    }else{
        //maxNode is empty which means the heap is empty
		cout<<"Empty heap"<<endl;
        return NULL;	
	}
    if(numberOfNodes>0){
        numberOfNodes--;
    }
	//Reinitializing the max's values as we dont want it to mess up when it is re inserted
	ptr->degree=0;
	ptr->child=NULL;
	ptr->leftSibling=ptr;
	ptr->rightSibling=ptr;
	ptr->parent=NULL;
	ptr->childCut=false;
	return ptr;
}


/**
 * Function which joins the two passed nodes by making second argument as child of first argument.
 * @param Node*, pointer to the node which has to become parent.
 * @param Node*, pointer to the node which has to become child.
 * @return nothing.
*/
void MaxFibonacciHeap::makeParentAndChild(Node *parentHere, Node *childHere){
	// childHere will be the child of parentHere
	// remove childHere from rootlist
	childHere->leftSibling->rightSibling=childHere->rightSibling;	
	childHere->rightSibling->leftSibling=childHere->leftSibling;
	// set childHere left and right siblings to itself
	childHere->leftSibling=childHere;
	childHere->rightSibling=childHere;
	childHere->childCut=false; 
    //If this parent has no child
	if(parentHere->child==NULL){
		parentHere->child=childHere;
		childHere->parent=parentHere;
	}else{
		Node* firstChild=parentHere->child;
		Node* secondChild=parentHere->child->rightSibling;
		// insert new node on the right of first child
		firstChild->rightSibling=childHere;
		childHere->leftSibling=firstChild;
		childHere->rightSibling=secondChild;
		secondChild->leftSibling=childHere;
		childHere->parent=parentHere;
	}
	parentHere->degree++;
}
