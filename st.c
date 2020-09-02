#include "st.h"

Node* New_Node(int i, int j, int sd, int id)
{
	Node* new_node = (Node*)malloc(sizeof(Node));

	if (new_node != NULL)
	{
		new_node->id = id;
		node_count++;

		new_node->i = i;
		new_node->j = j;
		new_node->sd = sd;

		new_node->pCh = NULL;
		new_node->pPar = NULL;
		new_node->pSib = NULL;
		new_node->sl = NULL;
	}

	return new_node;
}

void Destroy_Tree(Node* u)
{
	if (u->pCh != NULL)
	{
		Destroy_Tree(u->pCh);
	}
	if (u->pSib != NULL)
	{
		Destroy_Tree(u->pSib);
	}

	free(u);

	return;
}

bool Is_Leaf(Node* u)
{
	if (u != NULL)
	{
		if (u->pCh == NULL)
		{
			return true;
		}
	}

	return false;
}

bool Is_Root(Node* u)
{
	if (u != NULL)
	{
		if (u->i == -1 && u->j == -1 && u->id == -1)
		{
			return true;
		}
	}

	return false;
}

Node* Build_GSTree(char* l_seq, int start_ind[], int count)
{
	seq_len = node_count = inter_node = leafs = 0;

	// Create root node which isnt counted
	pRoot = New_Node(-1, -1, 0, -1);
	mix_colour = count + 1;
	pRoot->colour = mix_colour;
	pRoot->pPar = pRoot;

	clock_t start = clock();

	Node* exit_node = Insert_Sequence(l_seq, start_ind, count);

	clock_t end = clock();

	clock_t tree_ct = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Tree Construction Time: %f", tree_ct);

	/*printf("\n\nTree Before Colouring:\n");
	Tree_Detailed_Print(exit_node, 0);*/

	start = clock();

	Colour_Tree(exit_node);

	end = clock();

	clock_t colouring_time = ((double)(end - start)) / CLOCKS_PER_SEC;

	printf("Tree Colouring Time: %f", colouring_time);

	/*printf("\n\nTree After Colouring:\n");
	Tree_Detailed_Print(exit_node, 0);*/

	gSeq = NULL;
	return exit_node;
}

int Colour_Tree(Node* u)
{
	if (u == NULL)
		return 0;

	Print_Node(u);

	int new_colour = Colour_Tree(u->pCh);
	if (new_colour != 0 && new_colour != u->colour)
		u->colour = mix_colour;

	int colour = Colour_Tree(u->pSib);
	if (colour == 0)
		colour = u->colour;
	else if (u->colour == mix_colour)
		colour = mix_colour;
	else if (u->colour != colour)
		colour = mix_colour;

	//printf("OUT ");
	Print_Node(u);

	return colour;
}

LcsCoordinate* Get_LCS(Node* node)
{
	LcsCoordinate* lcs = (LcsCoordinate*)malloc(sizeof(LcsCoordinate));
	lcs->x1 = lcs->y1 = lcs->x2 = lcs->y2 = -1;
	srand(time(0));

	Node* lcs_Node = Find_LCS_Node(node);
	if (Is_Root(lcs_Node) == true)
		return NULL;

	Node* temp = lcs_Node->pCh;
	Node* s1 = NULL;
	Node* s2 = NULL;

	while (temp != NULL)
	{
		if (temp->colour == 1)
		{
			s1 = temp;
		}
		else if (temp->colour == 2)
		{
			s2 = temp;
		}

		temp = temp->pSib;
	}

	if (s1 == NULL || s2 == NULL)
		return NULL;

	lcs->x1 = s1->i - lcs_Node->sd;
	lcs->y1 = s2->i - lcs_Node->sd;
	lcs->x2 = s1->i - 1;
	lcs->y2 = s2->i - 1;

	return lcs;
}

Node* Find_LCS_Node(Node* node)
{
	if (node == NULL)
		return NULL;

	if (node->colour == 3)
	{
		Node* sib = Find_LCS_Node(node->pSib);
		Node* ch  = Find_LCS_Node(node->pCh);
		Node* temp = NULL;

		if (sib == NULL && ch == NULL)
			return node;
		if (sib == NULL)
		{
			temp = ch;
		}
		else if (ch == NULL)
		{
			temp = sib;
		}
		else
		{
			if (ch->sd > sib->sd)
			{
				temp = ch;
			}
			else if (ch->sd < sib->sd)
			{
				temp = sib;
			}
			else
			{
				int choice = rand() % 2;
				if (choice == 0)
				{
					temp = ch;
				}
				else
					temp = sib;
			}
		}

		if (node->sd > temp->sd)
		{
			return node;
		}
		else if (node->sd < temp->sd)
		{
			return temp;
		}
		else
		{
			int choice = rand() % 2;
			if (choice == 0)
			{
				return temp;
			}
			else
				return node;
		}
	}
	else
	{
		// Nothing to compare, if NULL, no other nodes
 		// If not null will return the mixed node
		 return Find_LCS_Node(node->pSib);
	}
}

void Print_Node(Node* node)
{
	/*
	switch (node->colour)
	{
	case 1:
		printf("\033[1;31m");
		break;
	case 2:
		printf("\033[1;32m");
		break;
	case 3:
		printf("\033[1;33m");
		break;
	case 4:
		printf("\033[1;34m");
		break;
	case 5:
		printf("\033[1;35m");
		break;
	case 6:
		printf("\033[1;36m");
		break;
	}
	printf("Node %d: %d:%c --- %d:%c | ", node->id, node->i, gSeq[node->i], node->j, gSeq[node->j]);
	printf("Colour: % d | ", node->colour);
	if (Is_Leaf(node) == true)
	{
		printf("Leaf Node | ");
	}
	else if(Is_Root(node) == true)
	{
		printf("Root Node | ");
	}
	else
	{
		printf("Internal Node | ");
	}
	printf("SDepth: %d\n", node->sd);
	printf("\033[0m");*/
}

Node* Insert_Sequence(char* seq, int start_ind[], int count)
{
	gSeq = seq;
	seq_len = strlen(seq);
	gJ = seq_len - 1;
	int i = 0;				// Current insert index in sequence
	int string_count = 1;
	tipping_point = start_ind[string_count];
	cur_j = tipping_point - 1;
	string_count++;
	int j = gJ;	// Index of last char "$" in string
	cur_colour = 1;
	node_count = inter_node = leafs = 0; // Set everything to zero
	pRoot->sl = pRoot;

	// First node is made and inserted into tree
	Node* cur = New_Node(i, cur_j, cur_j - i + 1, leafs);
	cur->colour = 1;
	leafs++;
	pRoot->pCh = cur;
	cur->pPar = pRoot;

	i++;

	Node* u = NULL;
	Node* v = NULL;
	Node* u_prime = NULL;	// u', no ' in c
	Node* v_prime = NULL;	// v', no ' in c

	//Print_Node(pRoot);
	//Print_Node(cur);

	// Main loop in charge of inserting every suffix in sequence
	while (i <= j)
	{
		// Tipping point to start colouring in the next sequences colour
		cur_i = i;
		if (i >= tipping_point && string_count < count)
		{
			cur = pRoot;
			tipping_point = start_ind[string_count];
			cur_j = tipping_point - 1;
			string_count++;
			cur_colour++;
		}
		else if (i >= tipping_point && string_count == count)
		{
			cur = pRoot;
			tipping_point = gJ + 1;
			cur_j = gJ;
			string_count++;
			cur_colour++;
		}
		//printf("\nInserting i:%d --- j:%d | %c -- %c\n", i, cur_j, gSeq[i], gSeq[j]);
		//printf("Index %d\n", (i+1));
		// Current is never root, its always the inserted node
		u = cur->pPar;	// Step 1 (All Cases)
		v = u->sl;		// Step 2 (IA & IB cases)
		if (v != NULL)
		{
			if (Is_Root(u) != true)
			{
				int alpha = v->sd;
				cur = Case_IA(v, i, alpha);		// Step 3
			}
			else
			{
				cur = Case_IB(pRoot, i);		// Step 3
			}
		}
		else
		{
			u_prime = u->pPar;						// Step 2 (IIA & IIB cases)
			v_prime = u_prime->sl;					// Step 3 (IIA & IIB cases)
			if (Is_Root(u_prime) != true)
			{
				int alpha_prime = u_prime->sd - 1;
				int beta = u->sd - u_prime->sd;
				cur = Case_IIA(u, v_prime, i, alpha_prime, beta);	// Steps 4-6
			}
			else
			{
				int beta_prime = u->sd - 1;
				cur = Case_IIB(u, pRoot, i, beta_prime);	// Steps 4-6
			}
		}

		u = NULL;
		v = NULL;
		u_prime = NULL;
		v_prime = NULL;
		i++;
	}

	Node* exit_node = pRoot;		// Saves root location
	pRoot = NULL;
	seq_len = node_count = inter_node = leafs = 0;

	return exit_node;			// returns root node
}

Node* Case_IA(Node* v, int i, int alpha)
{
	//printf("Case IA\n");
	Node* cur = FindPath(v, i + alpha); // Step 3
	return cur;
}

Node* Case_IB(Node* root, int i)
{
	//printf("Case IB\n");
	Node* cur = FindPath(root, i);		// Step 3
	return cur;
}

Node* Case_IIA(Node* u, Node* v_prime, int i, int alpha_prime, int beta)
{
	//printf("Case IIA\n");
	Node* v = NodeHops(v_prime, i + alpha_prime, beta);	// Step 4
	u->sl = v;								// Step 5
	int alpha = alpha_prime + beta;
	Node* cur = FindPath(v, i + alpha);		// Step 6
	return cur;
}

Node* Case_IIB(Node* u, Node* root, int i, int beta_prime)
{
	//printf("Case IIB\n");
	Node* v = NodeHops(root, i, beta_prime);// Step 4
	u->sl = v;								// Step 5
	Node* cur = FindPath(v, i + beta_prime);// Step 6
	return cur;
}

Node* FindPath(Node* u, int i)
{
	//printf("Find Path at node\n");
	//Print_Node(u);
	Node* cur = u;
	Node* temp = NULL;

	if (Find_Branch(cur->pCh, gSeq[i], &temp) == false)
	{
		//printf("Creating New Leaf\n");
		// Create new leaf node
		Node* new_leaf = New_Node(i, cur_j, cur_j - cur_i + 1 , leafs);
		new_leaf->colour = cur_colour;
		leafs++;
		new_leaf->pPar = cur;			// Connects to parent
		if (temp == NULL)				// Needs to be inserted as first node
		{
			new_leaf->pSib = cur->pCh;	// If no children just will point to NULL
			cur->pCh = new_leaf;		// Is now child
		}
		else	// Temp is a node after which to insert
		{
			new_leaf->pSib = temp->pSib;// Moves sibling or null pointer to new leaf
			temp->pSib = new_leaf;
		}
		Print_Node(new_leaf);
		return new_leaf;				// New Leaf made and inserted, returned
	}
	else		// Child has needed branch;
	{
		//printf("Branch Found\n");
		Print_Node(temp);
		cur = temp;
		int j = cur->i;
		// While i and j are within seq len and have the same char increment
		while (j <= cur->j && gSeq[i] == gSeq[j])
		{
			i++;
			j++;
		}

		// This case  means that
		//  string has ligned up in such a way that its end is an internal node
		//	crucial in execution for correct tree colouring!
		if ((i-1) == cur_j)
		{
			//printf("String and leaf matched up\n");
			if (cur_colour != cur->colour)
				cur->colour = mix_colour;
			return cur;
		}

		if (j > cur->j)
		{
			return FindPath(cur, i);
		}
		else
		{
			//printf("Mismatch, new node created\n");
			Node* new_internal = New_Node(cur->i, j-1, cur->sd - (cur->j-(j-1)), seq_len + inter_node);
			if (cur->colour != cur_colour)
			{
				new_internal->colour = mix_colour;
			}
			else
			{
				new_internal->colour = cur_colour;
			}
			inter_node++;
			new_internal->pCh = cur;
			new_internal->pPar = cur->pPar;
			cur->pPar = new_internal;

			new_internal->pSib = cur->pSib;
			cur->pSib = NULL;

			Node* child = new_internal->pPar->pCh;
			if (child != cur)
			{
				while (child->pSib != cur)
					child = child->pSib;
				child->pSib = new_internal;
			}
			else
			{
				new_internal->pPar->pCh = new_internal;
			}
			cur->i = j;
			Print_Node(new_internal);
			return FindPath(new_internal, i);
		}
	}
}

Node* NodeHops(Node* u, int i, int beta)
{
	//printf("Node hop on:\n");
	Print_Node(u);
	if (beta <= 0)
		return u;

	Node* cur = u;
	Node* temp = NULL;
	if (Find_Branch(cur->pCh, gSeq[i], &temp) == true)
	{
		//printf("Branch found\n");
		Print_Node(temp);
		cur = temp;
		int len = cur->j - cur->i + 1;

		if (len <= beta)
			return NodeHops(cur, i + len, beta - len);
		else // if beta ends early
		{
			//printf("Node Hop ended early, new node made\n");
			Node* new_internal = New_Node(cur->i, cur->i + beta - 1, cur->pPar->sd + beta, seq_len + inter_node);
			new_internal->colour = cur_colour;
			inter_node++;
			new_internal->pCh = cur;
			new_internal->pPar = cur->pPar;
			cur->pPar = new_internal;

			new_internal->pSib = cur->pSib;
			cur->pSib = NULL;

			Node* child = new_internal->pPar->pCh;
			if (child != cur)
			{
				while (child->pSib != cur)
					child = child->pSib;
				child->pSib = new_internal;
			}
			else
			{
				new_internal->pPar->pCh = new_internal;
			}
			cur->i = new_internal->j + 1;
			Print_Node(new_internal);
			return new_internal;
		}
	}
	//printf("Node Hop went out of control, should not get here!\n");
	return NULL;
}

bool Find_Branch(Node* u, char c, Node** found)
{
	//printf("Looking for %c\n", c);
	//Get_Children(u);
	// If child node doesnt exist or if the char is
	// smaller than the first char of the first branch
	if (u == NULL || c < gSeq[u->i])
	{
		(*found) = NULL;
		return false;
	}

	Node* temp = u;

	// If the first child is the needed branch
	if (c == gSeq[temp->i])
	{
		(*found) = temp;
		return true;
	}

	// move along the siblings till last one or till char c is equal
	// to or smaller than the next nodes char
	while (temp->pSib != NULL && c > gSeq[temp->pSib->i])
	{
		temp = temp->pSib;
	}

	// if last child reached and not found, return last node
	// if next nodes char is larger and currents is smaller,
	// return current
	if (temp->pSib == NULL || (c < gSeq[temp->pSib->i] && c > gSeq[temp->i]))
	{
		(*found) = temp;
		return false;
	}

	//if match found return true and match
	if (c == gSeq[temp->pSib->i])
	{
		(*found) = temp->pSib;
		return true;
	}

	//printf("Error in Find Branch, should never reach here!\n");
	return false;
}

void Get_Children(Node* u, FILE* fp, char* seq)
{
	//fprintf(fp, "\nParent ID: %d, Index Start: %d, String Depth: %d, Start char:%c\n", u->id, u->i, u->sd, seq[u->i]);
	Node* temp = u->pCh;
	if (temp == NULL)
	{
		//fprintf(fp, "Node has no Children\n");
		return;
	}
	while (temp != NULL)
	{
		fprintf(fp, "Child ID: %d, Index Start: %d, String Depth: %d, Start char:%c\n", temp->id, temp->i, temp->sd, seq[temp->i]);
		temp = temp->pSib;
	}
	return;
}

int DFS(Node* u, int count, FILE* fp)
{
	if (u == NULL)
		return count;

	int tot = count;
	if (Is_Root(u) != true)
	{
		tot++;
		if (tot >= 10)
		{
			fprintf(fp, "%d\n", u->sd);
			tot = 0;
		}
		else
			fprintf(fp, "%d ", u->sd);
	}

	tot = DFS(u->pCh, tot, fp);
	tot = DFS(u->pSib, tot, fp);
	return tot;
}

int Post_Order(Node* u, int count, FILE* fp)
{
	if (u == NULL)
		return count;

	int tot = count;

	tot = Post_Order(u->pCh, tot, fp);

	if (Is_Root(u) != true)
	{
		tot++;
		if (tot >= 10)
		{
			fprintf(fp, "%d\n", u->sd);
			tot = 0;
		}
		else
			fprintf(fp, "%d ", u->sd);
	}

	tot = Post_Order(u->pSib, tot, fp);
	return tot;
}

void BWT(Node* u, FILE* fp, char* seq)
{
	if (u == NULL)
		return;

	if (Is_Leaf(u) == true)
	{
		print_BWT(u->id, fp, seq);
		fprintf(fp, "\n");
	}

	BWT(u->pCh, fp, seq);
	BWT(u->pSib, fp, seq);
	return;
}

void BWT_Colour(Node* u, FILE* fp, char* seq)
{
	if (u == NULL)
		return;

	if (Is_Leaf(u) == true)
	{
		print_BWT(u->id, fp, seq);
		fprintf(" %d", u->colour);
		fprintf(fp, "\n");
	}

	BWT(u->pCh, fp, seq);
	BWT(u->pSib, fp, seq);
	return;
}

void print_BWT(int i, FILE* fp, char* seq)
{
	if (i == 0)
	{
		fprintf(fp, "%c", seq[gJ]);
	}
	else
	{
		fprintf(fp, "%c", seq[i - 1]);
	}
}

int Count_Internal_Nodes(Node* u)
{
	if (u == NULL)
		return 0;

	int count = 0;
	if (Is_Leaf(u) != true)
	{
		if (Is_Root(u) != true)
			count++;
	}

	count += Count_Internal_Nodes(u->pCh);
	count += Count_Internal_Nodes(u->pSib);
	return count;
}

int Count_Leaf_Nodes(Node* u)
{
	if (u == NULL)
		return 0;

	int count = 0;
	if (Is_Leaf(u) == true)
	{
		count++;
	}

	count += Count_Leaf_Nodes(u->pCh);
	count += Count_Leaf_Nodes(u->pSib);
	return count;
}

int Count_Total_Nodes(Node* u)
{
	if (u == NULL)
		return 0;

	int count = 0;
	if (Is_Root(u) != true)
	{
		count++;
	}

	count += Count_Total_Nodes(u->pCh);
	count += Count_Total_Nodes(u->pSib);
	return count;
}

int Total_SD(Node* u)
{
	if (u == NULL)
		return 0;

	int count = 0;
	if (Is_Leaf(u) != true)
	{
		if (Is_Root(u) != true)
		{
			count += u->sd;
		}
	}

	count += Total_SD(u->pCh);
	count += Total_SD(u->pSib);
	return count;
}

Node* Deepest_Internal_Node(Node* u)
{
	if (u == NULL)
		return NULL;

	Node* deepest = Deepest_Internal_Node(u->pCh);
	Node* deep2 = Deepest_Internal_Node(u->pSib);
	Node* deep3 = NULL;

	if (Is_Leaf(u) != true)
	{
		if (Is_Root(u) != true)
			deep3 = u;
	}
	;

	if (deep2 != NULL)
	{
		if (deepest == NULL)
		{
			deepest = deep2;
		}
		else
		{
			if (deepest->sd < deep2->sd)
				deepest = deep2;
		}
	}

	if (deep3 != NULL)
	{
		if (deepest == NULL)
		{
			deepest = deep3;
		}
		else
		{
			if (deepest->sd < deep3->sd)
				deepest = deep3;
		}
	}

	return deepest;
}

void Exact_Match_Repeat(Node* u, FILE* fp)
{
	Node* repeat = Deepest_Internal_Node(u);

	int depth = repeat->sd;
	fprintf(fp, "Exact matching repeat String-Depth: %d\n", depth);
	Node* temp = repeat->pCh;

	printf("Coordinates: ");
	while (temp != NULL)
	{
		fprintf(fp, "%d ", (temp->i - depth + 1));
		temp = temp->pSib;
	}
	fprintf(fp, "\n");
}



void GetFingerPrints(Node* curNode, Fingerprint* fingerprints, int mixedColor)
{
	if (curNode == NULL)
		return;
	if (curNode->colour != mixedColor && !(curNode->pCh == NULL && curNode->i == curNode->j))
	{
		int firstChar = curNode->i;
		int fingerPrintLocation = curNode->colour - 1;
		Node* tempNode = curNode->pPar;
		if (tempNode->j == -1)
		{
			fingerprints[fingerPrintLocation].pHead = malloc(sizeof(NodeListNode));
			fingerprints[fingerPrintLocation].pHead->myNode = curNode;
			fingerprints[fingerPrintLocation].pHead->pNext = NULL;
			fingerprints[fingerPrintLocation].size = 1;
		}
		else //if not the root
		{
			NodeListNode* nodeListHead = malloc(sizeof(NodeListNode));
			nodeListHead->myNode = curNode;
			nodeListHead->pNext = NULL;

			int size = 1;
			while (tempNode->i != -1) //while not the root
			{
				NodeListNode* newNode = malloc(sizeof(NodeListNode));
				newNode->myNode = tempNode;
				newNode->pNext = NULL;
				if (nodeListHead != NULL)
				{
					newNode->pNext = nodeListHead;
					nodeListHead = newNode;
					size += tempNode->j - tempNode->i + 1;
				}
				tempNode = tempNode->pPar;
			}
			if (size < fingerprints[fingerPrintLocation].size)
			{
				NodeListNode* curNode = fingerprints[fingerPrintLocation].pHead;
				NodeListNode* nextNode = NULL;
				while (curNode != NULL && curNode->myNode != NULL)
				{
					nextNode = curNode->pNext;
					free(curNode);
					curNode = nextNode;
				}
				fingerprints[fingerPrintLocation].pHead = nodeListHead;
				fingerprints[fingerPrintLocation].size = size;
			}
			else
			{
				NodeListNode* curNode = nodeListHead;
				NodeListNode* nextNode = NULL;
				while (curNode != NULL && curNode->myNode != NULL)
				{
					nextNode = curNode->pNext;
					free(curNode);
					curNode = nextNode;
				}
			}
		}
	}
	else
	{
		GetFingerPrints(curNode->pCh, fingerprints, mixedColor);
	}
	GetFingerPrints(curNode->pSib, fingerprints, mixedColor);
}

void Tree_Detailed_Print(Node* node, int depth)
{
	if (node == NULL)
		return NULL;
	printf("\n");
	Print_Spaces(depth);
	printf("\033[1;32m");
	printf("Depth: %d\n", depth);
	printf("\033[0m");

	Print_Spaces(depth);
	printf("At ");
	Print_Node(node);
	Print_Spaces(depth);

	printf("\033[1;31m");
	printf("Start of Children of %d ----->\n", node->id);
	printf("\033[0m");

	Tree_Detailed_Print(node->pCh, depth+1);
	Print_Spaces(depth);

	printf("\033[1;31m");
	printf("<---------- Children of %d End\n", node->id);
	printf("\033[0m");

	Tree_Detailed_Print(node->pSib, depth);
}

void Print_Spaces(int depth)
{
	for (int i = 0; i < depth; i++)
		printf("  ");
}