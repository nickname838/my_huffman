#include <iostream>
#include <fstream>

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

// кодирование
void encode (string file) {
    ifstream in(file, ios::binary);
    if(!in) {puts("Input file doesn't exist"); return;}
}

int main() {
    string file;
    cout << "Enter file's name: ";
    cin >> file;
}