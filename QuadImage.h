#ifndef QUAD_IMAGE_H
#define QUAD_IMAGE_H

#include "OtherQuad.h"
#include "Utilities.h"

class QuadImage {
    int width;
    int height;
    bool oper; // 0: read pgm | 1: read binary
    QuadTree<int> tree{};
    vector<Node<int>*> leafs;

public:
    QuadImage() {}

    void load(string filename) {
        oper = 0;
        read_pgm(filename);
        leafs = tree.getLeafNodes();
    }

    void loadBinary(string filename="binario.dot") {
        oper = 1;
        readSavedLeafs(filename);
    }

    void buildImage() {
        print_pgm();
    }

    void saveBinary(string filename = "") {
        if (oper) {
            cout << "Operación innecesaria\n";
            return;
        } 
        if (filename == "")
            filename = "binario.dot";
        tree.exportInfo(filename);
    }

    void showNodes() {
        for (auto n : leafs) {
            cout << n->x << " " << n->y << endl;
        }
        cout << leafs.size() << endl;
    }

    ~QuadImage() {}

private:
    void read_pgm(string filename){
        fstream image;
        image.open(filename, ios::in);

        string line = getLine(image, 3);
        vector<int> numbers = splitToInt(line, ' ');
        
        this->width = numbers[0];
        this->height = numbers[1];
        this->tree.setDimension(width, height);

        cout << width << endl << height << endl;
        getline(image, line);
        int threshold = atoi(line.c_str())/2;
        int c=0;
        cout << "cargando datos..\n";
        for (int i=1; i<=height; i++) {
            for (int j=1; j<=width; j++) {
                getline(image, line);
                int val = atoi(line.c_str());
                int color = val < threshold ? 0 : 1;
                if (color == 0) {
                    c++;
                    tree.insert(j, height-i+1);
                }
            }
        }
        cout << "\ndatos cargados: " << c << endl;
        image.close();
    }

    void print_pgm(){
        //auto leafs = tree.getLeafNodes();
        remove("sample_1.pgm");
        fstream image;
        image.open("sample_1.pgm", ios::out);
        image << "P2" << endl;
        image << "# feep.pgm" << endl;
        image << width << ' ' << height << endl;
        image << "255" << endl;
        int matriz [width][height];
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                matriz[j][i] = 255;
            }
        }
        for(auto k : leafs){
            matriz[width-k->y][k->x-1] = 0;
            if(k->group){
              for(int i = k->min_x; i <= k->max_x; i++){
                for(int j = k->min_y; j <= k->max_y; j++){
                  matriz[width-j][i-1] = 0;
                }
              }
            }
        }
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                image << to_string(matriz[i][j]) << endl;
            }
        }
        image.close();
    }

    void readSavedLeafs(string filename) {
        fstream file;
        file.open(filename, ios::in);

        int width, height;
        file.read((char*)&width, sizeof(int));
        file.read((char*)&height, sizeof(int));

        cout << width << endl << height << endl;

        while(file.peek() != EOF) {
            Node<int> *n =  new Node<int>();
            file.read((char*)&n->x, sizeof(int));
            file.read((char*)&n->y, sizeof(int));
            file.read((char*)&n->min_x, sizeof(int));
            file.read((char*)&n->min_y, sizeof(int));
            file.read((char*)&n->max_x, sizeof(int));
            file.read((char*)&n->max_y, sizeof(int));
            file.read((char*)&n->group, sizeof(bool));
            leafs.push_back(n);
        }
        file.close();
    }

};

#endif //QUAD_IMAGE_H