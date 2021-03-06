#include "functions.h"

/* ---------- Printing functions ---------- */

/**
 * @brief Print a node
 * 
 * @param node 
 */
void printNode(Node* node) 
{
	if (!node) return;

	printf("Character: %c, Occurence: %d\n", node->box.name,  node->box.freq);
	printNode(node->next);
}

/**
 * @brief Print a table
 * 
 * @param table 
 */
void printTable(Table* table)
{
	if (!table) return;

	printf("Character : %c codé : ", table->name);
	printCodeList(table->listcode);
	printf("\n");
	printTable(table->next);
}

/**
 * @brief Print a list of type Code
 * 
 * @param codeList 
 */
void printCodeList(Code* codeList)
{
	if (!codeList) return;
	
	printf("%" PRIu8, codeList->code);
	printCodeList(codeList->next);
}

/**
 * @brief Free an entire Code List
 * 
 * @param codeList 
 */
void freeCodeList(Code* codeList)
{
	if (!codeList) return;

	freeCodeList(codeList->next);
	free(codeList);
}

/**
 * @brief Free an entire Way List
 * 
 * @param wayList 
 */
void freeWayList(Way* wayList)
{
	if (!wayList) return;

	freeWayList(wayList->next);
	free(wayList);
}

/**
 * @brief Free an entire Byte List
 * 
 * @param bList 
 */
void freeByteList(ByteList* bList)
{
	if (!bList) return;

	freeByteList(bList);
	free(bList);
}

/**
 * @brief Free an entire Table List
 * 
 * @param tabList 
 */
void freeTableList(Table* tabList)
{
	if (!tabList) return;

	freeTableList(tabList->next);
	
	freeCodeList(tabList->listcode);
	free(tabList);
}

/**
 * @brief Free an entire Node List
 * 
 * @param node 
 */
void freeNodeList(Node* node)
{
	if (!node) return;

	freeNodeList(node->next);
	freeNodeList(node->left);
	freeNodeList(node->right);

	freeWayList(node->way);

	free(node);
}

/**
 * @brief Print a ByteList
 * 
 * @param bList 
 */
void printByteList(ByteList* bList)
{
	if (!bList) return;

	printf("%" SCNd8, bList->Byt);
	printByteList(bList->next);
}

/**
 * @brief Print a tree (Taken from internet)
 * 
 * @param root 
 * @param space 
 */
void printTree(Node* root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += 10;

    // Process right child first
    printTree(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%c%d\n", root->box.name, root->box.freq);

    // Process left child
    printTree(root->left, space);
}

/*--------------------------------------------*/
/*------------Merge Sort functions------------*/
/*--------------------------------------------*/

/**
 * @brief Get the lenght of a node
 * 
 * @param node 
 * @return int 
 */
int LenghtList(Node* node)
{
	if (!node) return 0;
	return LenghtList(node->next) + 1;
}

/**
 * @brief Merge a and b node in one
 * 
 * @param a 
 * @param b 
 * @return Node* 
 */
Node* Merge(Node* a,Node* b) {

	Node* result = NULL;

	if (!a) return (b);
	else if (!b) return (a);

	if (a->box.freq <= b->box.freq) {
		result = a;
		result->next = Merge(a->next, b);
	}
	else {
		result = b;
		result->next = Merge(b->next, a);
	}

	return result;
}

/**
 * @brief Split a node into A and B
 * 
 * @param node 
 * @param A 
 * @param B 
 */
void Split(Node* node, Node** A, Node** B)
{
	Node* fast = node->next;
	Node* slow = node;

	while (fast) {
		fast = fast->next;
		if (fast) {
			slow = slow->next;
			fast = fast->next;
		}
	}

	*A = node;
	*B = slow->next;
	slow->next = NULL;

}

/**
 * @brief Merge sort a node
 * 
 * @param node 
 * @return Node* 
 */
Node* MergeSort(Node* node)
{
	if (!node) return NULL;

	int lenght = LenghtList(node);
	Node* a = NULL;
	Node* b = NULL;


	if (lenght >= 2) 
	{
		Split(node, &a, &b);

		a = MergeSort(a);
		b = MergeSort(b);

		return node = Merge(a,b);

	} else if (lenght == 2) {
		if (node->box.freq > node->next->box.freq) 
		{
			node->next->next = node;
			node = node->next;
			node->next->next = NULL;
			return node;
		}
	}

	return node;
}
/*--------------------------------------------*/
/*----------------Compress function-----------*/
/*--------------------------------------------*/

/**
 * @brief Applie a delay
 * 
 * @param milli_seconds 
 */
void delay(int milli_seconds) 
{
	int milli = milli_seconds *1000;
    clock_t start_time = clock();
    while (clock() < start_time + milli);
}

/**
 * @brief 
 * 
 * @param node 
 * @param filename 
 * @return Node* 
 */
Node* FillList(Node* node, FILE* inFile) 
{

	if (inFile) {

		fseek(inFile,0,SEEK_SET); //set cursor to the beginning

		char buffer;
		buffer = fgetc(inFile); //get the first char

		/*Initialise a node*/
		node->box.name = buffer;
		node->box.freq = 0;
		node->next = NULL;
		node->isLeaf = 1;
		node->left = NULL;
		node->right = NULL;

		while(buffer != EOF){
			AddChar(node, buffer);
			buffer = fgetc(inFile);
		}

		return node;

	} else {
		printf("Error : file not found\n");
		return NULL;
	}
}

/**
 * @brief 
 * 
 * @param node 
 * @param name 
 */
void AddChar(Node* node, char name) 
{
	if (node) 
	{
		if (node->box.name == name) node->box.freq ++;

		else {
			if (!node->next) 
			{
				Node* new = calloc(1, sizeof *new);

				if (!node || !new) exit(EXIT_FAILURE);

				new->box.name = name;
				new->box.freq = 1;
				new->isLeaf = 1;

				node->next = new;

			} else AddChar(node->next, name);
		}
	}
}

/**
 * @brief 
 * 
 * @param node 
 * @param outFile 
 */
void fprintNode(Node* node, FILE* outFile) 
{
	if (!node) return;

	fprintf(outFile,"%c%d ", node->box.name,  node->box.freq);
	fprintNode(node->next, outFile);
}

/**
 * @brief 
 * 
 * @param node 
 * @return Node* 
 */
Node* MakeTree(Node* node) 
{
	if  (LenghtList(node) == 1) return node;

	node = MergeSort(node);

	Node* new = calloc(1, sizeof *new);

	if (!node || !new) exit(EXIT_FAILURE);

	new->box.freq = node->box.freq + node->next->box.freq;
	new->next = node->next->next;
	new->isLeaf = 0;
	new->left = node;
	new->right = node->next;

	return MakeTree(new);
}

/**
 * @brief 
 * 
 * @param node 
 * @param buffer 
 * @param table 
 */
void ReadTree(Node* node, Code* buffer, Table** table) 
{
	if (!node->left & !node->right) 
	{
		buffer = AddIntToBuffer(buffer, buffer, 0);
		*table = AddCharTable(*table, node->box.name, buffer);
	} else ReadTreeRec(node, node, buffer, table);
}

/**
 * @brief 
 * 
 * @param node 
 * @param root 
 * @param buffer 
 * @param table 
 */
void ReadTreeRec(Node* node, Node* root, Code* buffer, Table** table) 
{
	if (node->left) 
	{
		if (!node->left->left && !node->left->right && node->left->isLeaf == 0) {
			node->left = NULL;
			if (root->left != NULL || root->right != NULL) ReadTreeRec(root, root, NULL, table);

		} else if (node->left->isLeaf) 
		{
			buffer = AddIntToBuffer(buffer, buffer, 0);
			*table = AddCharTable(*table,node->left->box.name, buffer);
			node->left = NULL;
			if (root->left != NULL || root->right != NULL) ReadTreeRec(root, root, NULL, table);
		
		} else {
			buffer = AddIntToBuffer(buffer, buffer, 0);
			ReadTreeRec(node->left,root,buffer, table);
		}
	}

	if (node->right) 
	{
		if (!node->right->left && !node->right->right && node->right->isLeaf == 0) 
		{
			node->right = NULL;
			if (root->left || root->right) ReadTreeRec(root, root, NULL, table);

		} else if (node->right->isLeaf) 
		{
			buffer = AddIntToBuffer(buffer, buffer, 1);
			*table = AddCharTable(*table,node->right->box.name, buffer);
			node->right = NULL;
			if (root->left || root->right) ReadTreeRec(root, root, NULL, table);
		
		} else {
			buffer = AddIntToBuffer(buffer, buffer, 1);
			ReadTreeRec(node->right,root,buffer, table);
		}
	}

}

/**
 * @brief 
 * 
 * @param buffer 
 * @param root 
 * @param value 
 * @return Code* 
 */
Code* AddIntToBuffer(Code* buffer, Code* root, uint8_t value)
{
	if (!buffer) 
	{
		Code* new = calloc(1, sizeof(*new));
		new->code = value;
		new->next = NULL;
		return new;
	
	}
	if (buffer->next)
	{
		AddIntToBuffer(buffer->next,root, value);
		return buffer;

	} else {
		Code* new = calloc(1, sizeof(*new));
		new->code = value;
		new->next = NULL;
		buffer->next = new;
		return root;
	}
}

/**
 * @brief 
 * 
 * @param table 
 * @param name 
 * @param code 
 * @return Table* 
 */
Table* AddCharTable(Table* table, char name, Code* code) 
{
	Table* new = calloc(1, sizeof(*new));

	new->name = name;
	new->listcode = code;
	new->next = table;

	return new;
}

/**
 * @brief 
 * 
 * @param inFile 
 * @param outFile 
 * @param table 
 * @param totalChar 
 */
void ReplaceText(FILE* inFile, FILE* outFile, Table* table, int totalChar) 
{
	int countByte_v = 1;
	int* countByte = &countByte_v;

	int countLoop = 0;
	int percent = 0;

	if (!inFile || !outFile) exit(EXIT_FAILURE);

	char charbuffer = fgetc(inFile);

	Code* codeList = NULL;

	ByteList* bList = calloc(1, sizeof(*bList));

	bList->count = 0;
	bList->Byt = 0;
	bList->next = NULL;
	
	while (charbuffer != EOF) 
	{
		codeList = Encode(charbuffer,table);
		Bytify(bList, codeList, countByte);
		charbuffer = fgetc(inFile);

		if (totalChar >= 100 && countLoop % (totalChar / 100) == 0) 
		{
			fputs("\033[A\033[2K",stdout);
			printf("Compressing [ %d% ]\n", percent);
			percent += 1;
		}
		countLoop += 1;
	}

	fprintf(outFile, "$%d $$ ", *countByte);

	WriteByte(bList, outFile);
}

/**
 * @brief 
 * 
 * @param bList 
 * @param codeList 
 * @param countByte 
 */
void Bytify(ByteList* bList, Code* codeList, int* countByte) 
{
	if (!bList) return;
	else if (codeList->code != 1 && codeList->code != 0) exit(3);
	else if (bList->count == 8 && bList->next != NULL) Bytify(bList->next, codeList, countByte);
	else if (bList->count < 8) 
	{
		bList->Byt = BitAdd(bList->Byt, codeList->code);
		bList->count ++;
		if (codeList->next != NULL) Bytify(bList, codeList->next, countByte);
	
	} else if (!bList->next) 
	{
		*countByte += 1;
		ByteList* new = calloc(1, sizeof(*new));

		new->count = 1;
		new->Byt = BitAdd(bList->Byt, codeList->code);
		
		new->next = NULL;

		bList->next = new;

		if (codeList->next != NULL) Bytify(bList->next, codeList->next, countByte);
	}
}

/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @return uint8_t 
 */
uint8_t BitAdd(uint8_t x, uint8_t y) 
{
    x <<= 1;
	x |= y;
	return x;
}

/**
 * @brief 
 * 
 * @param name 
 * @param table 
 * @return Code* 
 */
Code* Encode(char name, Table* table) 
{
	if (!table) exit(1);

	while(name != table->name) 
	{
		table = table->next;
		if (!table) exit(2);
	}

	return table->listcode;
}

/**
 * @brief 
 * 
 * @param bList 
 * @param outFile 
 */
void WriteByte(ByteList* bList, FILE* outFile) 
{
	if (!bList) return;

	fwrite(&bList->Byt, 1, 1, outFile);
	WriteByte(bList->next, outFile);
}

/*--------------------------------------------*/
/*------------Decompress function-------------*/
/*--------------------------------------------*/

/**
 * @brief 
 * 
 * @param inFile 
 * @param countByte 
 * @return Node* 
 */
Node* fscanNode(FILE* inFile, int* countByte) 
{	
	fseek(inFile,0,SEEK_SET);
	char charbuffer;
	int intbuffer = 0;
	Node* oldNode = NULL;

	while (1) 
	{
		charbuffer = fgetc(inFile);

		Node* new = calloc(1, sizeof(*new));
		new->next = oldNode;
		new->isLeaf = 1;
		new->box.name = charbuffer;

		charbuffer = fgetc(inFile);

		while(charbuffer != ' ') {
			intbuffer = concatenate(intbuffer, (int)(charbuffer)-48);
			charbuffer = fgetc(inFile);
		}
		new->box.freq = intbuffer;
		intbuffer = 0;
		if (new->box.freq == -12) {
			*countByte = new->next->box.freq;
			return new->next->next;
		}
		oldNode = new;
	}
	
}

/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @return int 
 */
int concatenate(int x, int y) 
{
    int pow = 10;

    while(y >= pow)
        pow *= 10;

    return x * pow + y;        
}

/**
 * @brief 
 * 
 * @param node 
 */
void ReverseNode(Node** node)
{  
   if (!node) return;

   ReverseNodeRec(*node, NULL, node);
}

/**
 * @brief 
 * 
 * @param node 
 * @param buffer 
 * @param root 
 */
void ReverseNodeRec(Node* node, Node* buffer, Node** root) 
{
	if (!node->next) 
	{
   		*root = node;
		node->next = buffer;
		return;
	}

	Node* next = node->next;
	node->next = buffer;
	ReverseNodeRec(next, node, root);
}

/**
 * @brief 
 * 
 * @param inFile 
 * @param countByte 
 * @param buffFile 
 * @return FILE* 
 */
FILE* MakeBinList(FILE* inFile, int* countByte, FILE* buffFile) 
{
	char charbuffer = fgetc(inFile);
	uint8_t bytebuffer[8];
	uint8_t mask = 254;
	int count = 0;

	if (!buffFile) exit(11);


	while (count < *countByte) 
	{

		for (int i=0; i<8; i++) 
		{
			if ((mask | (uint8_t)(charbuffer)) == 254) bytebuffer[i] = 0;
			else if ((mask | (uint8_t)(charbuffer)) == 255) bytebuffer[i] = 1;
			charbuffer >>= 1;
		}

		for (int i=7; i>=0; i--) fprintf(buffFile,"%d",bytebuffer[i]);

		charbuffer = fgetc(inFile);
		count++;
	}

	return buffFile;
}

/**
 * @brief 
 * 
 * @param node 
 * @param outFile 
 * @param buffFile 
 */
void WriteFile(Node* node, FILE* outFile, FILE* buffFile) 
{
	int exitvalue_v = 0;
	int* exitvalue = &exitvalue_v;

	char charbuffer_v = 0;
	char* charbuffer = &charbuffer_v;

	while (*charbuffer != EOF)
	{
		WriteFileRec(outFile, node, node, buffFile, exitvalue, charbuffer);
		*exitvalue = 0;
	}
}

/**
 * @brief 
 * 
 * @param outFile 
 * @param node 
 * @param root 
 * @param buffFile 
 * @param exitvalue 
 * @param charbuffer 
 */
void WriteFileRec(FILE* outFile, Node* node, Node* root, FILE* buffFile, int* exitvalue, char* charbuffer ) 
{
	if (!outFile) exit(10);

	if (!node) 
	{
		printf("Error : end of branch reached");
		exit(1);
	}

	*exitvalue += 1;

	if (node->isLeaf == 1) 
	{
		fputc((int)(node->box.name),outFile);
		if(*exitvalue >= 1000) return;
		return WriteFileRec(outFile, root, root, buffFile, exitvalue, charbuffer);
		
	}

	else {
		*charbuffer = fgetc(buffFile);

		if ((int)(*charbuffer) == EOF) return;

		else if ((int)(*charbuffer)-48 == 0)
			return WriteFileRec(outFile, node->left, root, buffFile, exitvalue, charbuffer);
		
		else if ((int)(*charbuffer)-48 == 1)
			return WriteFileRec(outFile, node->right, root, buffFile, exitvalue, charbuffer);
		
	}
}





























