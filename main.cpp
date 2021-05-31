#include <iostream>
#include "QuadImage.h"

int main() {
    
    /*QuadTree<int> tree(43, 43);
    tree.insert(3, 3);
    tree.insert(3, 4);
    tree.insert(3, 2);

    tree.exportInfo("binario.dat");

    QuadImage qimge{};
    qimge.loadBinary("binario.dat");
    //qimge.load("dragon.pgm");
    qimge.showNodes();*/
    //qimge.buildImage();
    QuadImage qimge_dragon{};
    qimge_dragon.load("dragon.pgm");
    qimge_dragon.saveBinary();
    qimge_dragon.buildImage();

    
    /**/

    //getMidSize(8,1);
}
