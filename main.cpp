#include <iostream>
#include "vector.h"
#include "DemoVector.h"
//#include "hilos.h"
#include "linkedlist.h"
#include "doublelinkedlist.h"
#include "binarytree.h"
#include "avl.h"

using namespace std;

/* Revisado por:
   1. Ernesto Cuadros-Vargas
   4. Héctor Bobbio Hermoza 
   2. Jharvy Jonas Cadillo Tarazona
   20. Ortiz Lozano Eric Hernan
   22. Chandler Steven Perez Cueva
*/

// Forma 1 de Compilar: 
// g++ -std=c++17 -Wall -g -pthread -o main main.cpp
// Forma #2 de Compilar (requiere el archivo Makefile)
// make

int main(){
    cout << "Hello Alg y EDA-UNI" << endl;
    // DemoThreads();
    //[[maybe_unused]] int x = 5;
    std::cout << "\n--------------------------- PRUEBAS PARA EL VECTOR ---------------------------------\n";
    DemoVector();
    // DemoVector();
    // DemoLinkedList();
    std::cout << "\n---------------------- PRUEBAS PARA EL DOUBLELINKEDLIST -----------------------------\n";
    DemoDoubleLinkedList();

    // std::cout << "\n---------------------- PRUEBAS PARA EL BINARYTREE -----------------------------\n";
    // DemoBinaryTree();

    // std::cout << "\n---------------------- PRUEBAS PARA EL AVL TREE -----------------------------\n";
    // testAVLTree();

    return 0;
}

// int main(int nArgs, char *pArgs[]){
//     cout << "Hello Alg y EDA-UNI (forma #2)" << endl;
//     int i;
//     for(i = 0 ; i < nArgs ; ++i){
//         cout << pArgs[i] << endl;
//     }
// }



