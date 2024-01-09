#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
using namespace std;

template<typename T>
struct Node {
    T data;
    unsigned int height;
    Node* lchild;
    Node* rchild;
    Node(T key):lchild(nullptr), rchild(nullptr) , data(key) , height(0) {}
};

template<typename T>
class AVLTree {
private:
    Node<T>* root;
    // Insertion management functions
    unsigned int NodeHeight(Node<T>* p) {
        unsigned int hl;
        unsigned int hr;

        hl = (p && p->lchild) ? p->lchild->height : 0;
        hr = (p && p->rchild) ? p->rchild->height : 0;

        return hl > hr ? hl+1: hr+1 ;
    }
    int BalanceFactor(Node<T>* p) {
        unsigned int hl;
        unsigned int hr;

        hl = (p && p->lchild) ? p->lchild->height : 0;
        hr = (p && p->rchild) ? p->rchild->height : 0;

        return hl - hr;
    }
    Node<T>* LLRotation(Node<T>* p) {
        Node<T>* pl = p->lchild;
        Node<T>* plr = pl->rchild;

        pl->rchild = p;
        p->lchild = plr;

        // Update height
        p->height = NodeHeight(p);
        pl->height = NodeHeight(pl);

        // Update root
        if (root == p) {
            root = pl;
        }
        return pl;
    }
    Node<T>* RRRotation(Node<T>* p) {
        Node<T>* pr = p->rchild;
        Node<T>* prl = pr->lchild;

        pr->lchild = p;
        p->rchild = prl;

        // Update height
        p->height = NodeHeight(p);
        pr->height = NodeHeight(pr);

        // Update root
        if (root == p) {
            root = pr;
        }
        return pr;
    }
    Node<T>* LRRotation(Node<T>* p) {
        Node<T>* pl = p->lchild;
        Node<T>* plr = pl->rchild;
        
        pl->rchild = plr->lchild;
        p->lchild = plr->rchild;

        plr->lchild = pl;
        plr->rchild = p;
        
        // Update height
        plr->height = NodeHeight(plr);
        pl->height = NodeHeight(pl);
        p->height = NodeHeight(p);

        if (root == p)
            root = plr;

        return plr;
    }
    Node<T>* RLRotation(Node<T>* p) {
        Node<T>* pr = p->rchild;
        Node<T>* prl = pr->lchild;

        pr->lchild = prl->rchild;
        p->rchild = prl->lchild;

        prl->lchild = p;
        prl->rchild = pr;

        // Update height
        prl->height = NodeHeight(prl);
        pr->height = NodeHeight(pr);
        p->height = NodeHeight(p);

        if (root == p)
            root = prl;

        return prl;
    }
    Node<T>* rInsert(Node<T>* p, T key) {
        Node<T>* newNode = new Node<T>(key);
        if (p == nullptr) {
            return newNode;
        }
        if (key < p->data) {
            p->lchild = rInsert(p->lchild, key);
        }
        else if (key > p->data) {
            p->rchild = rInsert(p->rchild, key);
        }

        // Update height
        p->height = NodeHeight(p);

        // Rotation based on balance factor
        if (BalanceFactor(p) == 2 && BalanceFactor(p->lchild) == 1) {
            return LLRotation(p);
        }
        else if (BalanceFactor(p) == 2 && BalanceFactor(p->lchild) == -1) {
            return LRRotation(p);
        }
        else if (BalanceFactor(p) == -2 && BalanceFactor(p->rchild) == -1) {
            return RRRotation(p);
        }
        else if (BalanceFactor(p) == -2 && BalanceFactor(p->rchild) == 1) {
            return RLRotation(p);
        }

        return p;
    }

    // traversal
    // void Inorder(Node<T>* p) {
    //     if (p) {
    //         Inorder(p->lchild);
    //         cout << p->data;
    //         if (p->rchild) {
    //             cout << "," << flush;
    //             Inorder(p->rchild);
    //         }
    //     }
    // }

    string Inorder(Node<T>* p) {
        string result = "";
        if (p) {
            result += Inorder(p->lchild);
            result += p->data.printString();
            result += ",";
            result += Inorder(p->rchild);
        }
        return result;
    }

    Node<T>* rsearch(Node<T>* p, T key) {
        if (!p)
            return nullptr;
        if (key == p->data)
            return p;
        else if (key < p->data)
            return rsearch(p->lchild, key);
        else if (key > p->data)
            return rsearch(p->rchild, key);

        return nullptr;
    }

    //deletion mangement function
    Node<T>* InPre(Node<T>* p) {
        while (p && p->rchild != nullptr) {
            p = p->rchild;
        }
        return p;
    }        
    Node<T>* InSucc(Node<T>* p) {
        while (p && p->lchild != nullptr) {
            p = p->lchild;
        }
        return p;
    }  
    Node<T>* rDelete(Node<T>* p, T key) {
        if (p == nullptr) {
            return nullptr;
        }

        if (p->lchild == nullptr && p->rchild == nullptr) {
            if (p == root) {
                root = nullptr;
            }
            delete p;
            return nullptr;
        }

        if (key < p->data) {
            p->lchild = rDelete(p->lchild, key);
        }
        else if (key > p->data) {
            p->rchild = rDelete(p->rchild, key);
        }
        else {
            Node<T>* q;
            if (NodeHeight(p->lchild) > NodeHeight(p->rchild)) {
                q = InPre(p->lchild);
                p->data = q->data;
                p->lchild = rDelete(p->lchild, q->data);
            }
            else {
                q = InSucc(p->rchild);
                p->data = q->data;
                p->rchild = rDelete(p->rchild, q->data);
            }
        }

        // Update height
        p->height = NodeHeight(p);

        // rebalance the tree after deletion based on balance factor of the deleted from side
        if (BalanceFactor(p) == 2 && BalanceFactor(p->lchild) == 1) {
            return LLRotation(p);
        }
        else if (BalanceFactor(p) == 2 && BalanceFactor(p->lchild) == -1) { 
            return LRRotation(p);
        }
        else if (BalanceFactor(p) == -2 && BalanceFactor(p->rchild) == -1) { 
            return RRRotation(p);
        }
        else if (BalanceFactor(p) == -2 && BalanceFactor(p->rchild) == 1) { 
            return RLRotation(p);
        }
        else if (BalanceFactor(p) == 2 && BalanceFactor(p->lchild) == 0) { 
            return LLRotation(p);
        }
        else if (BalanceFactor(p) == -2 && BalanceFactor(p->rchild) == 0) {
            return RRRotation(p);
        }

        return p;
    }

public:
    AVLTree() { root = nullptr; }
    void insert(T key) { root = rInsert(root, key); }

    string print() { 
        string result = "[";
        result += Inorder(root); 
        result.pop_back(); // to remove the "," to get a valid json
        result += "]";
        return result;
    }

    Node<T>* search(T key) { return rsearch(root, key); }
    Node<T>* deletee(T key) { return rDelete(root, key); }
    Node<T>* getRoot() { return root; }
};

#endif
