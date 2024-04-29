#include <vector>

using namespace std;

struct Node {
    //node components:
    double p; //point
    double cr; // covering radius: max distance between p and any point in the tree
    Node *a; // direction to point's child disk page identified by (p,cr,a) of its intern node
    // other components:
    bool isLeaf;  // true if it is a leaf node, false otherwise
};

struct MTree {
    Node *root;
    vector<Node *> childs;
};