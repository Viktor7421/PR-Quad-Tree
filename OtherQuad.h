#ifndef O_QUAD_TREE_H
#define O_QUAD_TREE_H

#include "Node.h"
#include "Utilities.h"

const bool TEST = 0;

template <typename T>
class QuadTree {
    Node<T>* root;
    vector<Node<T>*> leafs;
    T width;
    T height;

public:

    QuadTree() : root{nullptr} {}

    QuadTree(T width, T height) : root{nullptr} {
        setDimension(width, height);
    }

    void setDimension(T width, T height) {
        this->width = width;
        this->height = height;
    }

    void insert(T x, T y, bool fill = 1) {
        insert(root, x, y, fill);
    }

    bool deleteNode(T x, T y) {
        return deleteNode(root, x, y);
    }

    vector<Node<T>*> getLeafNodes() {
        leafs.clear();
        getLeafs(root);
        return leafs;
    }

    int getWidth() {
        return this->width;
    }

    int getHeight() {
        return this->height;
    }

    void exportInfo(string filename) {
        fstream file;
        file.open(filename, ios::out);

        file.write((char*)&width, sizeof(T));
        file.write((char*)&height, sizeof(T));

        exportLeafs(file);

        file.close();
    }

    ~QuadTree() {}

private:

    Node<T>* newEmptyNode(T max_x, T max_y, T min_x, T min_y) {
        Node<T>* node = new Node<T>();
        node->max_x = max_x;
        node->max_y = max_y;
        node->min_x = min_x;
        node->min_y = min_y;
        node->fill = 0;
        node->is_leaf = false;
        node->group = false;
        return node; 
    }
    
    Node<T>* newNode(T x, T y, T max_x, T max_y, T min_x, T min_y, bool fill = 0) {
        Node<T>* node = newEmptyNode(max_x, max_y, min_x, min_y);
        setNodeValues(node, x, y, fill);
        return node;
    }

    void setNodeValues(Node<T>* node, T x, T y, bool fill = 1, bool leaf = 1) {
        node->x = x;
        node->y = y;
        node->fill = fill;
        node->is_leaf = leaf;
    }

    void insert(Node<T>* node, T x, T y, bool fill) {
        if (node == nullptr) {
            root = newNode(x, y, width, height, 1, 1, fill);
            root->parent = nullptr;
            if(TEST) cout << "inserted!" << endl;
            return;
        }

        if (node->fill) {
            if (splitNode(node)) {
                Node<T>* retaken = getChildNode(node, node->x, node->y);
                setNodeValues(retaken, node->x, node->y, 1);
                if(TEST) cout << "inserted!" << endl;
                Node<T>* child = getChildNode(node, x, y);
                insert(child, x, y, fill);
            } else {
                if(TEST) cout << "Nodo indivisible, se obvia la insrción de: ";
                if(TEST) cout << x << " " << y << "\nEnd!" << endl;
            }
        }
        else {
            if (node->is_leaf) {
                setNodeValues(node, x, y, fill);
                if(TEST) cout << "inserted.!" << endl;
                mergeAddedSiblings(node);   // try merging
                if(TEST) cout << "End!" << endl;
            } else {
                Node<T>* child = getChildNode(node, x, y);
                insert(child, x, y, fill);
            }
        }
    }

    /*  +---+---+
        | 0 | 1 |
        +---+---+
        | 2 | 3 |
        +---+---+  */
    bool splitNode(Node<T>* node) {
        if(TEST) cout << "\nSplit2\n";
        // check if it's indivisible
        if (node->max_x == node->min_x ||
            node->max_y == node->min_y)
            return false;

        // clean node
        node->is_leaf = false;
        node->fill = 0;

        // create children
        if(TEST) cout << node->max_x << " " << node->min_y << endl;
        int msize_x = getMidSize(node->min_x, node->max_x);
        int msize_y = getMidSize(node->min_y, node->max_y);
        //int msize_y = node->max_y - (int)ceil((node->max_y - node->min_y) / 2);
        for (int i=0; i<4; i++) {
            // get child node values
            T _max_x = (i%2) ? node->max_x : node->max_x - msize_x;
            T _max_y = (i<2) ? node->max_y : node->max_y - msize_y;
            T _min_x = (i%2) ? node->max_x - msize_x +1 : node->min_x;
            T _min_y = (i<2) ? node->max_y - msize_y +1 : node->min_y;
            if(TEST) printf("%2d %2d %2d %2d\n", _min_x, _min_y, _max_x, _max_y);
            // set child node values
            node->children[i] = newEmptyNode(_max_x, _max_y, _min_x, _min_y);
            node->children[i]->is_leaf = true;
            node->children[i]->parent = node;
        }
        return true;
    }

    Node<T>* getChildNode(Node<T>* node, T x, T y) {
        if(TEST) cout << "\nencontar nodo " << x << " " << y << endl;
        for (int i=0; i<4; i++) {
            T _max_x = node->children[i]->max_x;
            T _max_y = node->children[i]->max_y;
            T _min_x = node->children[i]->min_x;
            T _min_y = node->children[i]->min_y;

            if(TEST) printf("%2d: %2d %2d %2d %2d\n",i, _min_x, _min_y, _max_x, _max_y);
            if (x <= _max_x && x >= _min_x &&
                y <= _max_y && y >= _min_y ) {
                return node->children[i];
            }
        }
        throw runtime_error("No se encontró nodo hijo");
    }

    void mergeAddedSiblings(Node<T>* child) {
        if (child->parent == nullptr) return;
        Node<T>* parent = child->parent;

        // check filled child nodes
        int count = 0;
        for (int i=0; i<4; i++) {
            if (parent->children[i]->fill > 0) count++;
        }
        if (count < 4) return;

        // reset parent & delete children
        parent->fill = 1;
        parent->is_leaf = true;
        deleteChildNodes(parent);
        // recursive call
        parent->group = true;
        mergeAddedSiblings(parent);
    }
    
    void getLeafs(Node<T>* node) {
        if (node == nullptr) return;
        if (node->is_leaf) {
            if (node->fill) 
                this->leafs.push_back(node);
            return;
        }
        for (int i=0; i<4; i++) {
            getLeafs(node->children[i]);
        }
    }

    bool deleteNode(Node<T>* node, T x, T y) {
        Node<T>* temp = getExactNode(node, x, y);
        if (temp == nullptr) {
            return false;
        }
        // clear node
        temp->fill = 0;
        mergeDelSiblings(temp); // try merging
        return true;
    }

    Node<T>* getExactNode(Node<T>* node, T x, T y) {
        Node<T>* search_node = getRelativeNode(node, x, y);
        if (search_node->x == x && 
            search_node->y == y) {
            return search_node;
        } else {
            return nullptr;
        }
    }

    Node<T>* getRelativeNode(Node<T>* node, T x, T y) {
        if (node->is_leaf) {
            return node;
        }
        Node<T>* child = getChildNode(node, x, y);
        return getRelativeNode(child, x, y);
    }

    void mergeDelSiblings(Node<T>* node) {
        if (node->parent == nullptr) {
            delete node; node = nullptr;
            return;
        }
        Node<T>* parent = node->parent;

        // check filled child nodes
        Node<T>* temp = nullptr;
        for (int i=0; i<4; i++)
            if (parent->children[i]->fill > 0 ||
                !parent->children[i]->is_leaf) {
                // there are at least two children
                if (temp) return;
                // else, get the first filled node
                temp = parent->children[i];
            }
        
        // merging
        setNodeValues(parent, temp->x, temp->y);
        if (temp->is_leaf) {
            deleteChildNodes(parent);
            mergeDelSiblings(parent);
        }
    }

    void deleteChildNodes(Node<T>* parent) {
        for (int i=0; i<4; i++) {
            delete parent->children[i];
            parent->children[i] = nullptr;
        }
    }

    void exportLeafs(fstream &file) {
        if (leafs.size() == 0) getLeafs(root);

        for (auto &n : leafs) {
            file.write((char*)&n->x, sizeof(T));
            file.write((char*)&n->y, sizeof(T));
            file.write((char*)&n->min_x, sizeof(T));
            file.write((char*)&n->min_y, sizeof(T));
            file.write((char*)&n->max_x, sizeof(T));
            file.write((char*)&n->max_y, sizeof(T));
            file.write((char*)&n->group, sizeof(bool));
        }
    }
};

/*
Theory reference:
https://opendsa-server.cs.vt.edu/OpenDSA/Books/Everything/html/PRquadtree.html
*/

#endif //O_QUAD_TREE_H