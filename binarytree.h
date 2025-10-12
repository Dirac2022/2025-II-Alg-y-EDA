#ifndef __BINARY_TREE_H__  
#define __BINARY_TREE_H__ 
//#include <utility>
//#include <algorithm>
#include <cassert>
#include <fstream>
#include "types.h"
#include <iostream> //#include "util.h"
//#include "traits.h"
#include <vector>
//using namespace std;

template <typename Traits>
class CBinaryTree;


template <typename Traits>
class CBinaryTreeNode{
public:
    using value_type = typename Traits::value_type;
    using Node       = CBinaryTreeNode<Traits>; // cambio de <T> a <Traits>

    friend class CBinaryTree<Traits>;

protected:
    Node          *m_pParent = nullptr; // Cambie el orden
    value_type     m_data;
    Ref            m_ref;
    std::vector<Node *> m_pChild  = {nullptr, nullptr}; // 2 hijos inicializados en nullptr

public:
    CBinaryTreeNode(Node* pParent, value_type data, Ref ref, Node* p0 = nullptr, Node* p1 = nullptr)
        : m_pParent(pParent), m_data(data), m_ref(ref)
    {
        m_pChild[0] = p0;
        m_pChild[1] = p1;
    }
    ~CBinaryTreeNode(){
        delete m_pChild[0]; m_pChild[0] = nullptr;
        delete m_pChild[1]; m_pChild[1] = nullptr;
    }

    value_type  getData()                {   return m_data;    }
    value_type &getDataRef()             {   return m_data;    }
 
private: // TODO: Add this class as friend of the BinaryTree
        // and make these methods private
    void      setpChild(Node *pChild, size_t pos)  {   m_pChild[pos] = pChild;  }
    Node    * getChild(size_t branch){ return m_pChild[branch];  }
    Node    *&getChildRef(size_t branch){ return m_pChild[branch];  }
    Node    * getParent() { return m_pParent;   }

};


template <typename Container>
class binary_tree_iterator// : public general_iterator<Container, binary_tree_iterator<Container> >  
{  
public:
    using Parent    = typename Container::Node; // class general_iterator<Container, binary_tree_iterator<Container> >;
    using Node      = typename Container::Node;
    //using Container = binary_tree_iterator<Container>; // shahows template parameter

  public:
    binary_tree_iterator(Container *pContainer, Node *pNode) : Parent (pContainer,pNode) {}
    binary_tree_iterator(Container &other)  : Parent (other) {}
    binary_tree_iterator(Container &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    // TODO: Revisar el avance de un iterator
    binary_tree_iterator operator++() {
        Parent::m_pNode = Parent::m_pNode ? (Node*)Parent::m_pNode->getpNext() : nullptr;
        return *this;
    }
};


// template <typename _T>
// struct BinaryTreeAscTraits{
//     using  T         = _T;
//     using  Node      = CBinaryTreeNode<T>;
//     using  CompareFn = less<T>;
// };

// template <typename _T>
// struct BinaryTreeDescTraits
// {
//     using  T         = _T;
//     using  Node      = CBinaryTreeNode<T>;
//     using  CompareFn = greater<T>;
// };

template <typename Traits>
class CBinaryTree{
public:
    using value_type    = typename Traits::value_type;
    using Node          = CBinaryTreeNode<Traits>; // typename Traits::Node;
    
    using CompareFn     = typename Traits::CompareFn;
    using Container     = CBinaryTree<Traits>;
    // using iterator      = binary_tree_iterator<Container>;

protected:
    Node    *m_pRoot = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
public: 
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }

    void insert(value_type elem, Ref ref) {
        m_pRoot = internal_insert(elem, ref, nullptr, m_pRoot);
    }

     Node* getExtremeNode(Node* startNode, int direction) const {
        if (!startNode) return nullptr;
        
        Node* pNode = startNode;
        while (pNode->getChild(direction)) {
            pNode = pNode->getChild(direction);
        }
        return pNode;
    }

protected:
    Node* CreateNode(Node* pParent, value_type elem, Ref ref) {
        return new Node(pParent, elem, ref);
    }
    virtual Node* internal_insert(value_type &elem, Ref ref,
                                  Node* pParent, Node*& rpOrigin)
    {
        if (!rpOrigin) {
            ++m_size;
            return (rpOrigin = CreateNode(pParent, elem, ref));
        }

        size_t branch = Compfn(elem, rpOrigin->getDataRef()) ? 0 : 1;
        Node *pNode = internal_insert(elem, ref, rpOrigin, rpOrigin->getChildRef(branch));
        return pNode;
    }
public:
    CBinaryTree(){} // Empty tree
    
    // TODO: Copy Constructor. We have duplicate each node
    CBinaryTree(Container &other); // Cambio por Binary
    
    // TODO: Done: Move Constructor
    CBinaryTree(Container &&other) // Cambio por Binary
        : m_pRoot(std::exchange(other.m_pRoot, nullptr)), 
          m_size (std::exchange(other.m_size, 0)), 
          Compfn (std::exchange(other.Compfn, nullptr))
    { }

    // TODO: Recursivo y seguro. Destruir Nodes recursivamente
    virtual ~CBinaryTree(){  } 
    
    // TODO: begin dede comenzar el el nodo mas a la izquierda (0)
    // iterator begin() { 
    //     if (!m_pRoot) return end();
    //     return iterator(this, getExtremeNode(m_pRoot, 0));
    // }
    // iterator end()   { return iterator(this, nullptr); }

    // TODO: begin debe comenzar el el nodo mas a la derecha (1)
    // riterator rbegin(){ 
    //     if (!m_pRoot) return rend();
    //     return iterator(this, getExtremeNode(m_pRoot, 1));
    //  }
    // riterator rend()  { return iterator(this, nullptr); }

    // TODO: Generalizar estos recorridos para recibir cualquier funcion
    // con una cantidad flexible de parametros con variadic templates
    // Google: C++ parameter packs cplusplus
        void inorder  (std::ostream &os)    {   inorder  (m_pRoot, 0, os);  }
    // TODO: 
    void inorder(Node  *pNode, size_t level, std::ostream &os){
        if( pNode ){
            //Node *pParent = pNode->getParent();
            inorder(pNode->getChild(0), level+1, os);
            os << " --> " << pNode->getDataRef();
            inorder(pNode->getChild(1), level+1, os);
        }
    }

    // TODO: Generalize this function by using iterators and apply any function
    void inorder(Node  *pNode, void (*visit) (value_type& item)){
        if( pNode ){   
            inorder(pNode->getChild(0), *visit);
            (*visit)(pNode->getDataRef());
            inorder(pNode->getChild(1), *visit);
        }
    }

    // Variadic templates (See foreach.h)
    template <typename Function, typename... Args>
    void postorder(Function func, Args const&... args)
    {    postorder(m_pRoot, 0, func, args...);}

    template <typename Function,typename... Args>
    void postorder(Node* pNode, size_t level, 
                   Function func, Args const&... args) {
        if (pNode) {
            postorder(pNode->getChild(0), level + 1, func, args...);
            postorder(pNode->getChild(1), level + 1, func, args...);
            func(pNode, level); 
        }
    }
    // TODO: generalize this function to apply any function
    void postorder(Node  *pNode, size_t level, std::ostream &os){
        if( pNode ){   
            postorder(pNode->getChild(0), level+1, os);
            postorder(pNode->getChild(1), level+1, os);
            os << " --> " << pNode->getDataRef();
        }
    }

    // TODO: Generalize this function to apply any function
    void preorder (std::ostream &os)    {   preorder (m_pRoot, 0, os);  }
    // TODO: Generalize this function to apply any function
    void preorder(Node  *pNode, size_t level, std::ostream &os){
        if( pNode ){   
            os << " --> " << pNode->getDataRef();
            preorder(pNode->getChild(0), level+1, os);
            preorder(pNode->getChild(1), level+1, os);            
        }
    }

    void print    (std::ostream &os)    {   print    (m_pRoot, 0, os);  }
    // TODO: generalize this function to apply any function
    // Google: C++ parameter packs cplusplus
    void print(Node  *pNode, size_t level, std::ostream &os){
        if( pNode ){
            Node *pParent = pNode->getParent();
            print(pNode->getChild(1), level+1, os);
            os << std::string(" | ") << level << pNode->getDataRef() << "(" << (pParent?to_string(pParent->getData()):"Root") << ")" <<std::endl; // cambio << por *
            print(pNode->getChild(0), level+1, os);
        }
    }

    // TODO: Tip: recorrer el arbol en preorden
    void Write(std::ostream &os) { os << *this;  }

    // TODO: Leer en el arbol desde un stream asumiendo que esta en preorden
    void Read(std::istream &is)  { /* TODO */  }
};

// TODO: este operator << debe seguir estando fuera de la clase
template <typename Traits>
std::ostream & operator<<(std::ostream &os, CBinaryTree<Traits> &obj){
    os << "CBinaryTree with " << obj.size() << " elements.";
    obj.inorder(os);
    return os;
}

template <typename Traits>
std::istream & operator>>(std::istream &is, CBinaryTree<Traits> &obj){
    // Leer el arbol
    return is;
}

void DemoBinaryTree();

#endif // __BINARY_TREE_H__