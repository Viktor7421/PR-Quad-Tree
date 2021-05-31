#ifndef QUAD_TREE_UTILITIES_H
#define QUAD_TREE_UTILITIES_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

string getLine(fstream& file, int n) {
    string line;
    for (int i=0; i<n; i++) getline(file, line);
    return line;
}

std::ifstream::pos_type filesize(string filename) {
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

vector<int> splitToInt(string str, char pattern) {
    int posInit = 0;
    int posFound = 0;
    string splitted;
    vector<int> results;

    while(posFound >= 0){
        posFound = str.find(pattern, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        results.push_back(atoi(splitted.c_str()));
    }
    return results; 
}

int getMidSize(int min, int max) {
    int range = (max - min);
    /*int size = range/2 + range%2;
    cout << size << " " << range%2<<endl;
    for (int i=0; i<4; i++) {
        // get child node values
        int _max_x = (i%2) ? max : max - size;
        int _max_y = (i<2) ? max : max - size;
        int _min_x = (i%2) ? max - size +1 : min;
        int _min_y = (i<2) ? max - size +1 : min;
        printf("%2d: %2d %2d %2d %2d\n", i, _min_x, _min_y, _max_x, _max_y);
        // set child node values
    }*/
    return range/2 + range%2;
}

#endif //QUAD_TREE_UTILITIES_H