#include <cstring>
#include<iostream>
#include<fstream>
#include<string>
#include<time>

using namespace std;

struct node {
	string item;
	node *left = NULL;
	node *right = NULL;
};
struct node* newNode(string key)
{
	struct node* node = (struct node*)malloc(sizeof(struct node));
	node->item = key;
	node->left = node->right = NULL;
	return (node);
}
node* insert(string word, node* root);
void preOrder(struct node *root);
node* AdjustRoot(node *root, string key);
node* leftR(node* root);
node* rightR(node* root);
node* deleteNode(node* root, string key);
int main() {
	fstream input;
	char fname[40];
	cout << "請輸入文章檔名";
	cin >> fname;
	input.open(fname);
	double start, end;
	string row = "";	//從文章中拆出每個單字
	string *word = new string[100000];	//所有文章中的單字集合
	char letter;	//字母
	int n = 0;	//計算文章中單字的總數
	bool find;
	while (!(input.eof())) {
		letter = input.get();
		letter = toupper(letter);
		if ((int)(letter) < 91 && (int)(letter) > 64 || (int)(letter) == 39)
			row = row + letter;
		else {
			if (row != "") {
				word[n] = row;
				++n;
			}
			row = "";
		}
	}
	input.close();
	//--------------------第一題-----------------------------------------------------------------------
	node *start = new node;
	for (int i = 0; i < n; ++i) 
		start = insert(word[i], start);
	//preOrder(start);	//後序追蹤
	//cout << "1.字典建立成功" << endl;
	//--------------------第二題-------------------------------------------------------------------------
	start = clock() / 1000.0;
	while (1) {
		n = 0;
		cout << "2.請輸入要查詢的單字檔，離開請按-1:";
		cin >> fname;
		if (strcmp(fname, "-1") == 0)
			break;
		input.open(fname);
		while (!(input.eof())) {
			letter = input.get();
			letter = toupper(letter);
			if ((int)(letter) < 91 && (int)(letter) > 64 || (int)(letter) == 39)
				row = row + letter;
			else {
				if (row != "") {
					word[n] = row;
					++n;
				}
				row = "";
			}
		}
		input.close();
		int success = 0;
		for (int i = 0; i < n; ++i) {
			start = AdjustRoot(start, word[i]);
			if (start->item == word[i]) {
				cout << word[i] << ":1" << endl;
				success++;
			}
			else
				cout << word[i] << ":0" << endl;
		}
		double s = (double)(success) / double(n )*100.0;
		cout << "成功率:" << s << "%" << endl;
	}
	end = clock() / 1000.0;
	double t = end - start;
	cout << t << endl;
	//----------------第三題-------------------------------------------------------------------
	while (1) {
		cout << "3.請輸入要查詢的單字(大寫),離開請按-1:" << endl;
		cin >> row;
		if (row == "-1")
			break;
		else {
			start = AdjustRoot(start, row);
			if (start->item == row)
				cout << row << ":1" << endl;
			else
				cout << row << ":0" << endl;
		}
	}
	//--------------第四題---------------------------------------------------------------------
	while (1) {
		cout << "4.請輸入要加入的單字(大寫),離開請按-1:" << endl;
		cin >> row;
		if (row == "-1")
			break;
		else 
			start = insert(row, start);
	}
	//--------------第五題------------------------------------------------------------------------
	while (1) {
		cout << "5.請輸入要刪除的單字，離開請按-1:";
		cin >> row;
		if (row == "-1")
			break;
		else
			start = deleteNode(start, row);
	}
	row = "";
	//-------------確認所刪除和加入的單字----------------------------------------------------------
	while (1) {
		cout << "check:請輸入要查詢的單字(大寫),離開請按-1:" << endl;
		cin >> row;
		if (row == "-1")
			break;
		else {
			start = AdjustRoot(start, row);
			if (start->item == row)
				cout << row << ":1" << endl;
			else
				cout << row << ":0" << endl;
		}
	}

	system("pause");
	return 0;
}
node* insert(string words, node *root) {
	if (root->item == "") {
		root->item=words;
		return root;
	}
	root = AdjustRoot(root, words);	//把最靠近要insert的葉子移到root
	if (root->item == words)
		return root;
	node *newRoot = new node;
	newRoot->item = words;
	if (words < root->item) {	//插入的值<root
		newRoot->right = root;
		newRoot->left = root->left;
		root->left = NULL;
	}
	else{	//插入的值>root
		newRoot->left = root;
		newRoot->right = root->right;
		root->right = NULL;
	}
	return newRoot;
}

node* AdjustRoot(node *root, string key) {	//調整樹
	if (root->item == key || root == NULL) 		//找到key或root為NULL
		return root;
	if (root->item>key) {	//zag
		if (root->left == NULL)
			return root;
		if ( root->left->item > key ) {	//zag-zag
			if (root->left->left != NULL)
				root->left->left = AdjustRoot(root->left->left, key);
			else
				root->left = AdjustRoot(root->left, key);
			root = rightR(root);	//向右轉
		}
		else if ( root->left->item < key ) {	//zag-zig
			if(root->left->right!=NULL)	
				root->left->right = AdjustRoot(root->left->right, key);
			else	//root的左小孩無右小孩
				root->left= AdjustRoot(root->left, key);
			if (root->left->right != NULL) 
				root->left = leftR(root->left);	//向左轉
		}
		return (root->left == NULL) ? root : rightR(root);	//向右轉
	}
	else {		//zig
		if (root->right == NULL)
			return root;
		if ( root->right->item > key ) {	//zig zag
			if(root->right->left!=NULL)
				root->right->left = AdjustRoot(root->right->left, key);
			else	//root的右小孩無左小孩
				root->right = AdjustRoot(root->right, key);
			if (root->right->left != NULL)
				root->right = rightR(root->right);	//向右轉
		}
		else if (root->right->item < key ) {
			if(root->right->right!=NULL)
				root->right->right = AdjustRoot(root->right->right, key);
			else
				root->right = AdjustRoot(root->right, key);
			root = leftR(root);	//向左轉
		}
		return (root->right == NULL) ? root : leftR(root);	//向左轉
	}
}
node* rightR(node* root) {
	node* newRoot = root->left;
	newRoot->item = root->left->item;
	root->left = newRoot->right;
	newRoot->right = root;
	return newRoot;
}
node* leftR(node* root) {
	node* newRoot = root->right;
	newRoot->item = root->right->item;
	root->right = newRoot->left;
	newRoot->left = root;
	return newRoot;
}
node* deleteNode(node* root, string key) {
	struct node *temp;
	if (root==NULL)	//空樹
		return NULL;
	root = AdjustRoot(root, key);	//把要刪的點放到root
	if (key != root->item)
		return root; 
	if (root->left!=NULL){	//若左子樹不為空，選擇左子樹最大值當root
		node* parent = new node;
		temp = root->left;
		while (temp->right != NULL) {
			parent = temp;
			temp = temp->right;
		}
		root->item = temp->item;	//更改root的值
		temp->left = parent->right;
		free(temp);
	}
	else{	//若左子樹為空，選擇右子樹最小值當root
		node* parent = new node;
		temp = root->right;
		while (temp->left != NULL) {
			parent = temp;
			temp = temp->left;
		}
		root->item = temp->item;	//更改root的值
		temp->right = parent->left;
		free(temp);
	}
	return root;
}
void preOrder(node *root)
{
	if (root != NULL)
	{
		cout << root->item << endl;
		preOrder(root->left);
		preOrder(root->right);
	}
}