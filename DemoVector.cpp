#include <iostream>
#include "DemoVector.h"
#include "vector.h"
using namespace std;

template <typename _T>
struct GeneralTraits{
    using value_type = _T;
    
};

void DemoVector(){
    // CVector< GeneralTraits<int> > vector(10);
    // vector.insert(5);
    // TODO  (Nivel 1) habilitar el uso de []
    // vector[3] = 8;
    // TODO  (Nivel 2) habilitar que el vector pueda ser escrito con cout <<
    // cout << vector << endl;

    std::cout << "\nProbando iterador en CVector\n";
    CVector<GeneralTraits<int>> avector(5);
    // avector.insert(2);
    // avector.insert(4);
    // avector.insert(8);
    // avector.insert(6);
    // avector.insert(1);
    avector[0] = 3;
    avector[1] = 6;
    avector[2] = 7;
    avector[3] = 8;
    avector[4] = 5;
    // Iterator
    std::cout << "\nContenido del avector: \n";
    for (auto it = avector.begin(); it != avector.end(); ++it) {
        std:: cout << *it << " ";
    }

    std::cout << std::endl;

}