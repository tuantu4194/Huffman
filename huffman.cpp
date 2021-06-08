#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include<conio.h>
#include<fstream>

using namespace std;

struct Node
{
	char ch;
	int freq;
	Node* left, * right;
};
class  Huffman
{
private:
	Node* tree;
public:
	string ConvertStringToBit(const string codestring, int& remainbit);
	Node* getNode(char ch, int freq, Node* left, Node* right); // allocate a new tree node
	struct comp
	{
		bool operator()(Node* l, Node* r)
		{
			// highest priority ->lowest frequency
			return l->freq > r->freq;
		}
	};
	void encode(Node* root, string str, unordered_map<char, string>& huffmanCode);
	void decode(Node* root, int& index, string str);
	void buildHuffmanTree(string text);
};

string Huffman::ConvertStringToBit(const string codestring, int& remainbit)
{
	char temp;
	string ret;
	int m;
	int nbit = codestring.length();
	if (nbit % 8 == 0)  m = nbit / 8; else m = nbit / 8 + 1;
	int index = 0;
	for (int part = 0; part < m - 1; part++) {
		temp = 0;
		for (int bit = 0; bit < 8; bit++) {
			index = (part * 8 + bit);
			temp |= ((codestring[index] - '0') << bit);
		}
		ret.push_back(temp);
	}
	temp = 0;
	remainbit = (m * 8 - nbit);
	for (int bit = 0; bit < 8 - remainbit; bit++) {
		index = ((m - 1) * 8 + bit);
		temp |= ((codestring[index] - '0') << bit);
	}
	temp = temp << remainbit;
	ret.push_back(temp);
	return ret;
}


Node* Huffman::getNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

void Huffman::encode(Node* root, string str, unordered_map<char, string>& huffmanCode)
{
	if (root == nullptr)
		return;

	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}


void Huffman::decode(Node* root, int& index, string str)
{
	
	if (root == nullptr) {
		return;
	}

	if (!root->left && !root->right)
	{
		cout<< root->ch;
		return;
	}

	index++;

	if (str[index] == '0')
		decode(root->left, index, str);
	else
		decode(root->right, index, str);
}


void Huffman::buildHuffmanTree(string text)
{
	// count frequency and store in map

	unordered_map<char, int> freq;
	for (char ch : text)
	{
		freq[ch]++;
	}

	// Create a priority queue 
	priority_queue<Node*, vector<Node*>, comp> pq;

	// Create a leaf node for each character and add it to the priority queue.
	for (auto pair : freq) 
	{
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	// do till size >1
	while (pq.size() != 1)
	{
		// Remove the two nodes lowest freq
		Node* left = pq.top(); pq.pop();
		Node* right = pq.top();	pq.pop();
		//create new node by 2 nodes removed
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	// root stores pointer to root of Huffman Tree
	Node* root = pq.top();
	// traverse the Huffman Tree and store Huffman Codes, print
	unordered_map<char, string> huffmanCode;
	encode(root, "", huffmanCode);

	cout << "Huffman Codes are :\n" << '\n';
	for (auto pair : huffmanCode)
	{
		cout << pair.first << " " << pair.second << '\n';
	}


	// print encoded string
	string str = "";
	for (char ch : text) 
	{
		str += huffmanCode[ch];
	}
	int remain;
	string a;
	a = ConvertStringToBit(str, remain);

	cout << "\nEncoded string is :\n" << a << '\n';

	// decode the encoded string
	int index = -1;
	cout << "\nDecoded string is: \n";
	while (index < (int)str.size() - 2) 
	{
		decode(root, index, str);
	}
}

int main()
{
	ifstream file;
	file.open("text.txt");
	string str; char tmp;
	while (file.get(tmp)) str += tmp;
	Huffman abc;
	abc.buildHuffmanTree(str);
	file.close();
}


