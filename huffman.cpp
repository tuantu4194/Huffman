#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

struct Node
{
	char ch;
	int freq;
	Node* left, * right;
};
class  Tree 
{
private:
	Node* tree;
public:

	//allocate a new tree node
	Node* getNode(char ch, int freq, Node* left, Node* right)
	{
		Node* node = new Node();

		node->ch = ch;
		node->freq = freq;
		node->left = left;
		node->right = right;

		return node;
	}
	// Comparison object to be used to order the heap
	struct comp
	{
		bool operator()(Node* l, Node* r)
		{
			// highest priority ->lowest frequency
			return l->freq > r->freq;
		}
	};

	void encode(Node* root, string str, unordered_map<char, string>& huffmanCode)
	{
		if (root == nullptr)
			return;

		if (!root->left && !root->right) {
			huffmanCode[root->ch] = str;
		}

		encode(root->left, str + "0", huffmanCode);
		encode(root->right, str + "1", huffmanCode);
	}

	void decode(Node* root, int& index, string str)
	{
		if (root == nullptr) {
			return;
		}

		if (!root->left && !root->right)
		{
			cout << root->ch;
			return;
		}

		index++;

		if (str[index] == '0')
			decode(root->left, index, str);
		else
			decode(root->right, index, str);
	}

	// Builds Huffman Tree and decode given input text
	void buildHuffmanTree(string text)
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
		for (auto pair : freq) {
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
		for (auto pair : huffmanCode) {
			cout << pair.first << " " << pair.second << '\n';
		}

		cout << "\nOriginal string was :\n" << text << '\n';

		// print encoded string
		string str = "";
		for (char ch : text) {
			str += huffmanCode[ch];
		}

		cout << "\nEncoded string is :\n" << str << '\n';

		// decode the encoded string
		int index = -1;
		cout << "\nDecoded string is: \n";
		while (index < (int)str.size() - 2) {
			decode(root, index, str);
		}
	}
};


int main()
{
	string text = "Huffman coding is a data compression algorithm.";
	Tree abc;
	abc.buildHuffmanTree(text);

	return 0;
}

