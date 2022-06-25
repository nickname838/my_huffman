#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <sys/stat.h>

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

    // таблца кодов
    map<char, string> table;
    huffman(root, "", table);

    ofstream out(file + ".encoded", ios::binary);
    if(!out) {puts("Output file doesn't exist"); return;}

    // запись частоты байтов
    out << (char)(freq.size() - 1);
    for(auto pair: freq) {
        out << pair.first;
        out.write((char*)&pair.second, sizeof(pair.second));
    }

    // кодирование
    char temp = 0, count = 0;
    while(in.get(x)) {
        for(auto iter = table[x].begin(); iter != table[x].end(); iter++) {
            temp |= ((*iter - '0') << (7 - count));
            count++;
            if(count == 8) {
                out << temp;
                temp = 0;
                count = 0;
            }
        }
    }

    in.close();
    out.close();

    // сравнение размеров
    struct stat sb; int bsize = 0;
    struct stat se; int esize = 0;

    stat(file.c_str(), &sb); bsize = sb.st_size;
    stat((file + ".encoded").c_str(), &se); esize = se.st_size;

    cout << "compression: " << (bsize + 0.0) / esize << endl;
}

int main() {
    string file;
    cout << "Enter file's name: ";
    cin >> file;
    encode(file);
    system("pause");
}