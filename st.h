#ifndef ST_H
#define ST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "fileReader.h"


// Structure of the Node
typedef struct node
{
	struct node* pCh;		// Pointer to child
	struct node* pSib;		// Pointer to sibling
	struct node* pPar;		// Pointer to parent
	struct node* sl;		// Suffix Link Pointer

	int id;					// ID of node

	int i;					// Start of edge Index
	int j;					// End of edge Index (including)
	int sd;					// String Depth
	int colour;				// Colour of the given node
							// i through k for single colour
							// k+1 for  mixed
} Node;

typedef struct lcsCoordinate
{
	int x1;					// Sequence 1 start index of lcs
	int y1;					// Sequence 1 end   index of lcs
	int x2;					// Sequence 2 start index of lcs
	int y2;					// Sequence 2 end   index of lcs
} LcsCoordinate;

typedef struct nodeListNode
{
	Node* myNode;
	struct nodeListNode* pNext;
}NodeListNode;

typedef struct fingerprint
{
	char* str;
	int size;
	NodeListNode* pHead;
	Sequence* sequence;
} Fingerprint;

// Global variable that is a pointer top the root node
Node* pRoot;
char* gSeq;
int mix_colour;
int cur_colour;
int tipping_point;
int seq_len;
int gJ;
int cur_j;
int cur_i;
int node_count;
int inter_node;
int leafs;

// Description:
//	The function Creates a new Node with ID of global node_count
//	and Initiates start and end indexes to i and j
// Parameters:
//	int i start index of edge
//	int j end index of edge (including)
// Returns:
//	Pointer to new node
Node* New_Node(int i, int j, int sd, int id);

// Description:
//	Dealocates a tree starting from Node u
// Parameters:
//	Root of tree to dealocate
// Returns:
//	None
void Destroy_Tree(Node* u);

// Description:
//	The function checks if the node is a leaf node
// Parameters:
//	Node to check
// Returns:
//	Boolean value of True if Leaf
//	False if not a Leaf
bool Is_Leaf(Node* u);

// Description:
//	The function checks if the node is the root node
// Parameters:
//	Node to check
// Returns:
//	Boolean value of True if Root
//	False if not a Root
bool Is_Root(Node* u);

// Description:
//	The function is in charge of initialising an driving
//  the tree building process. First creates tree and then
//	colourse it
// Parameters:
//	Concatenated Sequence, array of start indexes, count of sequences
// Returns:
//	Root node of tree
Node* Build_GSTree(char* l_seq, int start_ind[], int count);

// Description:
//	The function colours the tree in a depth first search manner
// Parameters:
//	Node that recursion goes from
// Returns:
//	integer
//	0 - no colour (no node)
//	i - k - no mixing, colour of node
//	k + 1 - mixed colour
int Colour_Tree(Node* u);

// Description:
//	Initialising function for searching of lcs, creates objects for storage
//	Handles output from recursion to capture LCS or return NULL
// Parameters:
//	Root node of tree to start from
// Returns:
//	NULL if there are is no LCS, lcs object with location otherwise
LcsCoordinate* Get_LCS(Node* node);

// Description:
//	Recursive function that looks for lcs, while making sure it is the longest
//	If multiple of the same length, randomly one is chosen
// Parameters:
//	Node that carries the recursion
// Returns:
//	Returns NULL if no LCS
//	othrwise returns Node with lcs
Node* Find_LCS_Node(Node* node);

// Description:
//	Function used for check, prints details about a node and colours it if 
//	the number of sequences is bellow 6
// Parameters:
//	Node to print details about
// Returns:
//	Nothing
void Print_Node(Node* node);

// Description:
//	This function is in charge of adding to the tree and
//	building it up
//	tree is built on the global root
// Parameters:
//	Sequence on which to build tree and alphabet used in the sequence
// Returns:
//	Root Node passed back if build succesfull
//	NUll pointer if build unsucessfull
Node* Insert_Sequence(char* seq, int start_ind[], int count);

// Description:
//	Step 3 of IA
// Parameters:
//	Node v, index i and alpha 
// Returns:
//	New Leaf node that was created
Node* Case_IA(Node* v, int i, int alpha);

// Description:
//	Step 3 of IB
// Parameters:
//	Node v, index i
// Returns:
//	New Leaf node that was created
Node* Case_IB(Node* root, int i);

// Description:
//	Steps 4-6 for Case IIA
// Parameters:
//	Node, , index and beta
// Returns:
//	New Leaf node that was created
Node* Case_IIA(Node* u, Node* v_prime, int i, int alpha_prime, int beta);

// Description:
//	Steps 4-6 for Case IIB
// Parameters:
//	Node, root, index and beta
// Returns:
//	New Leaf node that was created
Node* Case_IIB(Node* u, Node* root, int i, int beta_prime);

// Description:
//	Finds path by matching each value, till mismatch
// Parameters:
//	Node to start matching from
// Returns:
//	New Leaf node that was created
Node* FindPath(Node* u, int i);

// Description:
//	Hops for nodes till beta amount is hoped
// Parameters:
//	U to start hops from
//	Index to hop from
//	Beta value
// Returns:
//	v node that a suffix link will point to
Node* NodeHops(Node* u, int i, int beta);

// Description:
//	This function looks for branch that starts with a specific char
//  If none is found returns false and found pointer is pointting at
//	Node to insert after or NULL (insert as the child of the parent)
// Parameters:
//	First Child node from node in whihc we are searching for char c
//	Node into which a node will be put if needed
// Returns:
//	bool that says if branch was found
//	found node pointer that point to the node if found
//	and if not found, the location of where to insert
bool Find_Branch(Node* u, char c, Node** found);

// Description:
//	Prints all children of Node u
// Parameters:
//	Node u to print children of
// Returns:
//	None
void Get_Children(Node* u, FILE* fp, char* seq);

// Description:
//	Enumerate nodes using DFS traversal (i.e., visit all
//	children from left to right AFTER visiting the parent)
// Parameters:
//	Node u to do DFS traversal from and count for line length
// Returns:
//	returns counts which helps with printing 10 chars a line
int DFS(Node* u, int count, FILE* fp);


// Description:
//	Enumerate nodes using post order traversal (i.e., visit all
//	children from left to right Before the parent)
// Parameters:
//	Node u to do DFS traversal from and count for line length
// Returns:
//	returns counts which helps with printing 10 chars a line
int Post_Order(Node* u, int count, FILE* fp);

// Description:
//	Enumerating ONLY the leaf node id's from left to right
//  (lexicographically smallest to largest) and prints them to desired output
// Parameters:
//	Node u to do BWT traversal from and count for line length
// Returns:
//	NONE
void BWT(Node* u, FILE* fp, char* seq);

// Description:
//	Enumerating ONLY the leaf node id's from left to right
//  (lexicographically smallest to largest) and prints them to desired output
//  with colours
// Parameters:
//	Node u to do BWT traversal from and count for line length
// Returns:
//	NONE
void BWT_Colour(Node* u, FILE* fp, char* seq);

// Description:
//	prints an enumerated leaf to desired output
// Parameters:
//	enumerated position, location to print and seq to print from
// Returns:
//	NONE
void print_BWT(int i, FILE* fp, char* seq);

// Description:
//	Function that can be used to count internal nodes from any "root" node
// Parameters:
//	Node u to do count from
// Returns:
//	Count of Internal Nodes (without root)
int Count_Internal_Nodes(Node* u);

// Description:
//	Function that can be used to count leaf nodes from any "root" node
// Parameters:
//	Node u to do count from
// Returns:
//	Count of Leaf Nodes (without root)
int Count_Leaf_Nodes(Node* u);

// Description:
//	Function that can be used to count total nodes from any "root" node
// Parameters:
//	Node u to do count from
// Returns:
//	Count of Total Nodes (without root)
int Count_Total_Nodes(Node* u);

// Description:
//	Function that can be used to count total string depth for all internal
// nodes from any "root" node
// Parameters:
//	Node u to do count from
// Returns:
//	Count of Total String Depth (without root)
int Total_SD(Node* u);

// Description:
//	Function that can be used to find deepest internal node from any "root" node
// Parameters:
//	Node u to do count from
// Returns:
//	Deepest internal node or NULL (without root)
Node* Deepest_Internal_Node(Node* u);

void Exact_Match_Repeat(Node* u, FILE* fp);

void GetFingerPrints(Node* curNode, Fingerprint* fingerprints, int mixedColor);

// Description:
//	Prints the tree in detail for accuracy check
// Parameters:
//	Node to print and depth of node to space out values in a tree manner
// Returns:
//	Nothing
void Tree_Detailed_Print(Node* node, int depth);

// Description:
//	Prints 2 * depth spaces for easier reading of tree
// Parameters:
//	depth to be used for amount of space prints
// Returns:
//	Nothing
void Print_Spaces(int depth);

#endif