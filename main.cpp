#include <iostream>
#include <stdio.h>
#include "QuadImage.h"

int main() {
    int ch = 0;
    cout << "Qué archivo desea cargar: \n";
    cout << "1: pgm \n2: binario\n";
    cin >> ch;

    QuadImage qimge{};

    if (ch == 1) {
        string file;
        cout << "Nombre del archivo: ";
        cin >> file;
        // "dragon.pgm"
        qimge.load(file);
        cout << "Guardadndo binario...\n";
        qimge.saveBinary();
        qimge.buildImage();
    } else if (ch == 2) {
        qimge.loadBinary();
        //qimge.showNodes();
        qimge.buildImage();
    }
    
}
