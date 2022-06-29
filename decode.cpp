#include <iostream>
#include <fstream>
#include <map>
#include <queue>

using namespace std;

// структура узла дерева
struct Node {
    char ch;
    int freq;
    Node *left;
    Node *right;
};

// создание узла
Node *getnode(char ch, int freq, Node *left = nullptr, Node *right = nullptr) {
    Node *node = new Node;
    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

// сравнение для очереди
struct comp {
    bool operator()(Node *l, Node *r) {return l->freq > r->freq;}
};

// создание дерева
Node *maketree(priority_queue<Node *, vector<Node *>, comp> &pq) {
    while(pq.size() > 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        int sum = left->freq + right->freq;
        pq.push(getnode('\0', sum, left, right));
    }
    return pq.top();
}

// создание кодов по дереву
void huffman(Node *&root, string code, map<char, string> &table) {
    if(!root) return;
    if(!root->left && !root->right) table[root->ch] = code;
    huffman(root->left, code + "0", table);
    huffman(root->right, code + "1", table);
}

// декодирование
void decode(string file) {
    ifstream in(file, ios::binary);
    if(!in) {puts("Input file doesn't exist"); return;}

    char x;
    in.get(x); 
    map<char, int> freq;

    // чтение частоты байтов
    for(int i = 0; i <= x; i++) {
        char y;
        int z;
        in.get(y);
        in.read((char*)&z, sizeof(z));
        freq[y] = z;
    }

    // очередь узлов
    priority_queue<Node *, vector<Node *>, comp> pq;
    for(auto pair: freq) {pq.push(getnode(pair.first, pair.second));}

    // созание дерева
    Node *root = maketree(pq);

    // создание таблицы кодов
    map<char, string> table;
    huffman(root, "", table);
}

int main() {
    string file;
    cout << "Enter file's name: ";
    cin >> file;
    decode(file);
    system("pause");
}