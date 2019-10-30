#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

typedef struct Node {
    std::map<char, Node*> edges;
    Node* suffixLink;
    Node* parent;

    int start = 0;
    int depth = 0;

    Node(int start, int depth, Node* parent): start(start), parent(parent), depth(depth){};
    
    ~Node(){
        suffixLink = nullptr;
        parent = nullptr;
    }
} Node;

void prettyPrint(Node* node, const std::string& t){
    Node* iter = node;
    std::queue<Node*> q;
    std::map<Node*, int> numberedNodes;

    int nodeNumber = 0;

    std::cout << "Beginnig Pretty-Print with Root Node #"<<nodeNumber<<"\nNumber of edges: " << iter->edges.size()<< '\n' << std::endl;

    for (auto edges: iter->edges){
        q.push(edges.second);
    }


    while (q.size() > 0){
        numberedNodes[iter] = nodeNumber;

        iter = q.front();
        q.pop();
        nodeNumber++;

        for (auto edges: iter->edges){
            q.push(edges.second);
        }
        
        std::string_view subStr (t.c_str()+iter->start, iter->depth);
        std::string_view parentSubStr (t.c_str()+iter->parent->start, iter->parent->depth);

        std::cout << "Current node is node #"<<nodeNumber<< " ,formed by the prefix: " << subStr << std::endl;
        std::cout << "Current node's parent is: " << (iter->parent == node? "Root Node" : parentSubStr) << std::endl; 
        std::cout << "This node has " << iter->edges.size() << " edges...\n" << std::endl; 
    }

    std::cout << "Suffix Links:\n" << std::endl;

    for (auto node: numberedNodes){
        if (node.first->suffixLink != nullptr){
            std::cout << "Suffix link exists from node #"<< numberedNodes[node.first] << " to node #" << numberedNodes[node.first->suffixLink] << std::endl;
        }
    }
}

void createLeaf(std::string& t, int i, int d, Node* node){
    Node* w = new Node(i, t.size()-i, node);

    node->edges[t[i+d]] = w;
}

Node* createNode(Node* u, int d, std::string& t){
    int i = u->start;
    Node* p = u->parent;

    Node* v = new Node(i, d, p);
    
    v->edges[t[i+d]] = u;
    u->parent = v;
    p->edges[t[i+p->depth]] = v;

    return v;
}

void emptyTree(Node* node){
    for (auto child: node->edges){
        emptyTree(child.second);
        child.second = nullptr;
    }

    node->edges.clear();
    delete node;
}

std::pair<Node*, int> slink(Node* u, int d, std::string& t){
    Node* v = u->parent->suffixLink;

    while (v->depth < d-1){
        v = v->edges[t[u->start+v->depth+1]];
    }

    return std::make_pair(v, d-1);
}

void computeSLink(Node* u, std::string& t){
    int d = u->depth;
    Node* v = u->parent->suffixLink;

    while (v->depth < d -1){
        v = v->edges[t[u->start+v->depth+1]];
    }

    if (v->depth > d-1){
        v = createNode(v, d-1, t);
    }

    u->suffixLink = v;
}

int main () {
    Node* root = new Node(0, 0, nullptr);
    root->suffixLink = root;    

    std::string text;
    std::cin >> text;
    text.push_back('$');

    int n = text.size();

    Node* u = root;
    int d = 0;

    for (int i = 0; i < n; i++){
        while (d == u->depth && u->edges[text[i+d]] != nullptr){
            u = u->edges[text[i+d]];
            d++;

            while(d < u->depth && text[u->start+d] == text[i+d]){
                d++;
            }
        }

        if (d < u->depth){
            u = createNode(u, d, text);
        }

        createLeaf(text,i, d, u);

        if (u->suffixLink == nullptr){
            computeSLink(u, text);
        }
        
        u = u->suffixLink;
        
        if (d > 0){
            d--;
        }
    }

    prettyPrint(root, text);
    emptyTree(root);
    root = nullptr;

    return 0;
}