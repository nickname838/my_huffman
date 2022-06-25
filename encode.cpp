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
        uint64_t sum = left->freq + right->freq;
        pq.push(getnode('\0', sum, left, right));
    }
    return pq.top();
}

// кодирование
void encode (string file) {
    ifstream in(file, ios::binary);
    if(!in) {puts("Input file doesn't exist"); return;}

    // частота байтов
    char x; map<char, int> freq;
    while(in.get(x)) freq[x]++;
    in.clear(); in.seekg(0);

    // очередь узлов
    priority_queue<Node *, vector<Node *>, comp> pq;
    for(auto pair: freq) pq.push(getnode(pair.first, pair.second));

    // создание дерева
    Node *root = maketree(pq);
}

int main() {
    string file;
    cout << "Enter file's name: ";
    cin >> file;
}