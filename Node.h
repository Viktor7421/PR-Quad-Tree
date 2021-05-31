#ifndef QUAD_TREE_NODE_H
#define QUAD_TREE_NODE_H

template <typename T>
struct Node {
    T x, y;
    T max_x;
    T max_y;
    T min_x;
    T min_y;
    bool fill;
    bool is_leaf;
    bool group;
    Node<T>* parent;
    Node<T>* children[4];
};

#endif //QUAD_TREE_NODE_H