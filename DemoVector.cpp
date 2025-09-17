#include <iostream>
#include <fstream>
#include <vector>
#include "DemoVector.h"
#include "vector.h"
#include "foreach.h"
using namespace std;

template <typename T>
void Print(T &v){
    cout << v << " ";
}

void DemoVector(){
    CVector<int> v1(10);
    // v1.insert(5);
    // TODO  (Nivel 1) habilitar el uso de []
    v1[3] = 8;
    // TODO (Done) (Nivel 2) habilitar que el vector pueda ser escrito con cout <<
    cout << v1 << endl;

    v1[5] = 10;
    ofstream of("test.txt");
    of << v1;
    of.close();

    vector<float> vf(10);
    vf[5] = 3.5;
    foreach(vf, &Print<float>);
}