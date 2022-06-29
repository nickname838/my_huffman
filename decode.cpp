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

    ofstream out(file.erase(file.find(".encoded")) + ".decoded", ios::binary);
    if(!out) {puts("Output file doesn't exist"); return;}

    // декодирование
    Node *temp = root;
    char count = 0;
    while(in.get(x)) {
        for(int i = 0; i < 8; i++) {
            bool b = x & (1 << (7 - i));
            if(b) temp = temp->right;
            else if(!b) temp = temp->left;
            if(!temp->left && !temp->right) {
                out << temp->ch;
                temp = root;
            }
        }
    }

    in.close();
    out.close();
    
    ifstream noncod(file, ios::binary);
    ifstream decode(file + ".decoded", ios::binary);

    // сравнение файлов
    char a, b; unsigned int equal = 0;
    while(noncod.get(a) && decode.get(b))
        if(a != b) equal++;

    while(noncod.get(a)) equal++;
    while(decode.get(b)) equal++;

    cout << "unequal bytes: " << equal << endl;

    noncod.close();
    decode.close();
}

int main() {
    string file;
    cout << "Enter file's name: ";
    cin >> file;
    decode(file);
    system("pause");
}