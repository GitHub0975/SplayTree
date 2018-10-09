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
	cout << "�п�J�峹�ɦW";
	cin >> fname;
	input.open(fname);
	double start, end;
	string row = "";	//�q�峹����X�C�ӳ�r
	string *word = new string[100000];	//�Ҧ��峹������r���X
	char letter;	//�r��
	int n = 0;	//�p��峹����r���`��
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
	//--------------------�Ĥ@�D-----------------------------------------------------------------------
	node *start = new node;
	for (int i = 0; i < n; ++i) 
		start = insert(word[i], start);
	//preOrder(start);	//��ǰl��
	//cout << "1.�r��إߦ��\" << endl;
	//--------------------�ĤG�D-------------------------------------------------------------------------
	start = clock() / 1000.0;
	while (1) {
		n = 0;
		cout << "2.�п�J�n�d�ߪ���r�ɡA���}�Ы�-1:";
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
		cout << "���\�v:" << s << "%" << endl;
	}
	end = clock() / 1000.0;
	double t = end - start;
	cout << t << endl;
	//----------------�ĤT�D-------------------------------------------------------------------
	while (1) {
		cout << "3.�п�J�n�d�ߪ���r(�j�g),���}�Ы�-1:" << endl;
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
	//--------------�ĥ|�D---------------------------------------------------------------------
	while (1) {
		cout << "4.�п�J�n�[�J����r(�j�g),���}�Ы�-1:" << endl;
		cin >> row;
		if (row == "-1")
			break;
		else 
			start = insert(row, start);
	}
	//--------------�Ĥ��D------------------------------------------------------------------------
	while (1) {
		cout << "5.�п�J�n�R������r�A���}�Ы�-1:";
		cin >> row;
		if (row == "-1")
			break;
		else
			start = deleteNode(start, row);
	}
	row = "";
	//-------------�T�{�ҧR���M�[�J����r----------------------------------------------------------
	while (1) {
		cout << "check:�п�J�n�d�ߪ���r(�j�g),���}�Ы�-1:" << endl;
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
	root = AdjustRoot(root, words);	//��̾a��ninsert�����l����root
	if (root->item == words)
		return root;
	node *newRoot = new node;
	newRoot->item = words;
	if (words < root->item) {	//���J����<root
		newRoot->right = root;
		newRoot->left = root->left;
		root->left = NULL;
	}
	else{	//���J����>root
		newRoot->left = root;
		newRoot->right = root->right;
		root->right = NULL;
	}
	return newRoot;
}

node* AdjustRoot(node *root, string key) {	//�վ��
	if (root->item == key || root == NULL) 		//���key��root��NULL
		return root;
	if (root->item>key) {	//zag
		if (root->left == NULL)
			return root;
		if ( root->left->item > key ) {	//zag-zag
			if (root->left->left != NULL)
				root->left->left = AdjustRoot(root->left->left, key);
			else
				root->left = AdjustRoot(root->left, key);
			root = rightR(root);	//�V�k��
		}
		else if ( root->left->item < key ) {	//zag-zig
			if(root->left->right!=NULL)	
				root->left->right = AdjustRoot(root->left->right, key);
			else	//root�����p�ĵL�k�p��
				root->left= AdjustRoot(root->left, key);
			if (root->left->right != NULL) 
				root->left = leftR(root->left);	//�V����
		}
		return (root->left == NULL) ? root : rightR(root);	//�V�k��
	}
	else {		//zig
		if (root->right == NULL)
			return root;
		if ( root->right->item > key ) {	//zig zag
			if(root->right->left!=NULL)
				root->right->left = AdjustRoot(root->right->left, key);
			else	//root���k�p�ĵL���p��
				root->right = AdjustRoot(root->right, key);
			if (root->right->left != NULL)
				root->right = rightR(root->right);	//�V�k��
		}
		else if (root->right->item < key ) {
			if(root->right->right!=NULL)
				root->right->right = AdjustRoot(root->right->right, key);
			else
				root->right = AdjustRoot(root->right, key);
			root = leftR(root);	//�V����
		}
		return (root->right == NULL) ? root : leftR(root);	//�V����
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
	if (root==NULL)	//�ž�
		return NULL;
	root = AdjustRoot(root, key);	//��n�R���I���root
	if (key != root->item)
		return root; 
	if (root->left!=NULL){	//�Y���l�𤣬��šA��ܥ��l��̤j�ȷ�root
		node* parent = new node;
		temp = root->left;
		while (temp->right != NULL) {
			parent = temp;
			temp = temp->right;
		}
		root->item = temp->item;	//���root����
		temp->left = parent->right;
		free(temp);
	}
	else{	//�Y���l�𬰪šA��ܥk�l��̤p�ȷ�root
		node* parent = new node;
		temp = root->right;
		while (temp->left != NULL) {
			parent = temp;
			temp = temp->left;
		}
		root->item = temp->item;	//���root����
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