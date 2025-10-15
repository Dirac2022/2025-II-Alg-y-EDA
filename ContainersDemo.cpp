#include <iostream>
#include <fstream>
#include <vector>
#include <utility> // para std::pair

// #include "linkedlist.h"
#include "doublelinkedlist.h"
#include "binarytree.h"
#include "foreach.h"
#include "types.h"
#include "util.h"
// #include "avl.h"

void testInorderVariadic(CBinaryTree<BinaryTreeAscTraits<T1>> &bt);
void testPreorderVariadic(CBinaryTree<BinaryTreeAscTraits<T1>> &bt);
void testPostorderVariadic(CBinaryTree<BinaryTreeAscTraits<T1>> &bt);
void testAVLTree();

#if 0
void opex(int &n){ n++; }

template <typename T>
void PrintX(T &val, ostream &os){ os << n << " "; }

template <typename T>
void PrintY(T &val, T value1, T value2, ostream &os){ 
    val += value1 + value2; 
    os << val << " "; 
}

#endif

#if 0
void DemoLinkedList(){
    std::vector< std::pair<T1, Ref> > v1 = {
        {4, 8}, {2, 5}, {7, 3}, {1, 9}, {5, 2}
    };
    CLinkedList< AscendingTrait<T1> > l1;
    for (auto &par : v1)
        l1.Insert(par.first, par.second);
    std::cout << l1 << std::endl;

    std::vector< std::pair<T2, Ref> > v2 = {
        {4.5, 8}, {2.3, 5}, {7.8, 3}, {1.1, 9}, {5.7, 2}
    };
    CLinkedList< DescendingTrait<T2> > l2;
    // Insertar desde el vector
    for (auto &par : v2) {
        l2.Insert(par.first, par.second);
    }
    std::cout << l2 << std::endl;

    std::cout << "Ahora utilizando foreach #1..." << std::endl;
    foreach(l1, ::opex);
    std::cout << "Imprimiendo: l1 (debe haber aumentado en 1) ..." << std::endl;
    foreach(l1, ::Print<T1>);
    cout <<endl;
    std::cout << "l1 aplicando funcion lambda +2 ..." << std::endl;
    foreach(l1, [](T1 &n){ n += 2;  } );
    std::cout << "Imprimiendo: l1 (debe haber aumentado en 1) ..." << std::endl;
    foreach(l1, ::Print<T1>);

    std::cout << "Imprimiendo: l1 a través de begin() y end() ..." << std::endl;
    foreach(l1.begin(), l1.end(), ::Print<T1>);
    cout <<endl;

    std::cout << "Imprimiendo l1 con Write ..." << std::endl;
    l1.Write(cout);
    cout <<endl;

    std::ofstream of("LL.txt");
    l1.Write(of);
    of.close();
}

#endif

void DemoDoubleLinkedList()
{
    std::vector<std::pair<T1, Ref>> v1 = {
        {4, 8}, {2, 5}, {7, 3}, {1, 9}, {5, 2}};

    std::cout << "\nDemo Double Linked List" << std::endl;

    CDoubleLinkedList<AscendingTrait<T1>> l1;
    for (auto &par : v1)
    {
        l1.Insert(par.first, par.second);
    }

    std::cout << l1 << std::endl;

    std::cout << "Imprimiendo con forward iterator" << std::endl;
    foreach (l1.begin(), l1.end(), ::Print<T1>)
        ;
    std::cout << std::endl;

    std::cout << "Imprimiendo con backward iterator" << std::endl;
    foreach (l1.rbegin(), l1.rend(), ::Print<T1>)
        ;
    std::cout << std::endl;

    // l1.foreach(::PrintX<T1>,       std::cout);
    // l1.foreach(::PrintY<T1>, 1, 3, std::cout);
    // l1.foreach([](T1 &val, ostream &os){
    //     os << val << " ";
    // }, std::cout);

    // std::ofstream of("DLL.txt");
    // l1.foreach(::PrintY<T1>, 1, 3, of);
    // of.close();

    std::cout << "Probando Constructor por copia\n";
    CDoubleLinkedList<AscendingTrait<T1>> l1_copia(l1);
    std::cout << "Imprimiendo con forward iterator" << std::endl;
    foreach (l1_copia.begin(), l1_copia.end(), ::Print<T1>);
    std::cout << std::endl;
    std::cout << "¿DDL l1 y DDL l1_copia son iguales?\n";
    if (l1 == l1_copia)
        std::cout << "Si" << std::endl;
    else
        std::cout << "No" << std::endl;
}

void DemoBinaryTree()
{

    std::vector<std::pair<T1, Ref>> v1 = {
        {4, 8}, {2, 5}, {7, 3}, {1, 9}, {5, 2}};
    CBinaryTree<BinaryTreeAscTraits<T1>> bt;
    for (auto &par : v1)
        bt.insert(par.first, par.second);
    std::cout << bt << std::endl;

    std::cout << "Inorder traversal:" << std::endl;
    bt.inorder(std::cout);
    std::cout << std::endl;

    std::cout << "\nProbando constructor por copia\n";
    CBinaryTree<BinaryTreeAscTraits<T1>> bt_copia(bt);
    std::cout << "Se creo el bt bt_copia\n";
    std::cout << "Inorder traversal para bt_copia:" << std::endl;
    bt_copia.inorder(std::cout);
    std::cout << std::endl;
    std::cout << "Agrego un nodo {6, 4} a bt (original)\n";
    bt.insert(6, 4);
    std::cout << "Inorder traversal bt original" << std::endl;
    bt.inorder(std::cout);
    std::cout << std::endl;
    std::cout << "Inorder traversal para bt_copia:" << std::endl;
    bt_copia.inorder(std::cout);
    std::cout << std::endl;

    std::cout << "Preorder traversal:" << std::endl;
    bt.preorder(std::cout);
    std::cout << std::endl;

    std::cout << "\nVisual\n";
    bt.printVisual();
    std::cout << endl;

    // std::cout << "Imprimiendo con forward iterator" << std::endl;
    // foreach(l1. begin(), l1. end(), ::Print<T1>);
    std::cout << std::endl;
    std::cout << "Probando iterador" << std::endl;
    for (auto it = bt.begin(); it != bt.end(); ++it)
    {
        std::cout << *it << " -> ";
    }
    std::cout << std::endl;

    std::cout << "Probando iterador reverso" << std::endl;
    for (auto it = bt.rbegin(); it != bt.rend(); ++it)
    {
        std::cout << *it << " -> ";
    }
    std::cout << std::endl;

    std::cout << "\nProbando funcion imprimir\n";
    bt.print(std::cout);

    std::cout << "--------------------------------------------";
    std::cout << "\nProbando inorder generalizado";
    testInorderVariadic(bt);
    std::cout << "--------------------------------------------\n";

    std::cout << "--------------------------------------------";
    std::cout << "\nProbando preorder generalizado";
    testPreorderVariadic(bt);
    std::cout << "--------------------------------------------\n";

    std::cout << "--------------------------------------------";
    std::cout << "\nProbando postorder generalizado";
    testPostorderVariadic(bt);
    std::cout << "--------------------------------------------\n";


    std::ofstream of("BT.txt");
    bt.Write(of);
    of.close();
}

void testInorderVariadic(CBinaryTree<BinaryTreeAscTraits<T1>> &bt)
{

    std::cout << "\nInorder con Lambda: Multiplicar por 2 e imprimir en consola: ";

    const T1 factor{2};
    auto doubleAndPrint = [](T1 &data, size_t level, std::ostream &os, const T1 &factor)
    {
        data *= factor;
        os << "{" << data << "} ";
    };

    bt.inorder(doubleAndPrint, std::cout, factor);
    std::cout << std::endl;
    std::cout << bt << std::endl;

    // std::cout << "\nInorder insitu" << std::endl;
    // bt.inorder([](T1& data, std::ostream& os, const T1& factor){
    //     data *= factor;
    //     os << "{" << data << "}";
    // }, std::cout, factor);
    // std::cout << std::endl;

    std::cout << "\nInorder con iterador y variadic template\n";
    bt.foreachInorder(
        [](T1 &data, std::ostream &os, const T1 &factor)
        {
                data *= factor;
                os << "{" << data <<"} "; },
        std::cout,
        factor);
    std::cout << std::endl;
}

void testPreorderVariadic(CBinaryTree<BinaryTreeAscTraits<T1>> &bt)
{

    std::cout << "\nPreorder con Lambda: Multiplicar por 2 e imprimir en consola: ";

    const T1 factor{2};
    auto doubleAndPrint = [](T1 &data, size_t level, std::ostream &os, const T1 &factor)
    {
        data *= factor;
        os << "{" << data << "} ";
    };

    bt.preorder(doubleAndPrint, std::cout, factor);
    std::cout << std::endl;
    std::cout << bt << std::endl;
}

void testPostorderVariadic(CBinaryTree<BinaryTreeAscTraits<T1>> &bt)
{

    std::cout << "\nPostorder con Lambda: Multiplicar por 2 e imprimir en consola: ";

    const T1 factor{2};
    auto doubleAndPrint = [](T1 &data, size_t level, std::ostream &os, const T1 &factor)
    {
        data *= factor;
        os << "{" << data << "} ";
    };

    bt.postorder(doubleAndPrint, std::cout, factor);
    std::cout << std::endl;
    std::cout << bt << std::endl;
}

#if 0
void testAVLTree()
{
    std::vector<std::pair<T1, Ref>> v1 = {
        {4, 8}, {3, 5}, {2, 3}, {1, 9}};
    CAVLTree<AVLAscTraits<T1>> avl;
    for (auto &par : v1)
        avl.insert(par.first, par.second);
    std::cout << avl << std::endl;
    std::cout << "\nVisual\n";
    avl.printVisual();
    std::cout << endl;
}

void multiplicaMuestra(T1 &item)
{
    item *= 2;
    std::cout << "{" << item << "}";
}
#endif
