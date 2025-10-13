#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__
#include <utility>
#include <algorithm>
#include <cassert>
#include <fstream>
#include "types.h"
#include <iostream> //#include "util.h"
// #include "traits.h"
#include <vector>
// using namespace std;

template <typename Traits>
class CBinaryTree;

template <typename Container>
class binary_tree_iterator;

template <typename Traits>
class CBinaryTreeNode
{
public:
    using value_type = typename Traits::value_type;
    using Node = CBinaryTreeNode<Traits>; // cambio de <T> a <Traits>

    friend class CBinaryTree<Traits>;
    template <typename>
    friend class binary_tree_iterator;

protected:
    Node *m_pParent = nullptr; // Cambie el orden
    value_type m_data;
    Ref m_ref;
    std::vector<Node *> m_pChild = {nullptr, nullptr}; // 2 hijos inicializados en nullptr

public:
    CBinaryTreeNode(Node *pParent, value_type data, Ref ref, Node *p0 = nullptr, Node *p1 = nullptr)
        : m_pParent(pParent), m_data(data), m_ref(ref)
    {
        m_pChild[0] = p0;
        m_pChild[1] = p1;
    }
    ~CBinaryTreeNode() = default;

    value_type getData() { return m_data; }
    value_type &getDataRef() { return m_data; }

private: // todo: Add this class as friend of the BinaryTree
         // and make these methods private
    void setpChild(Node *pChild, size_t pos) { m_pChild[pos] = pChild; }
    Node *getChild(size_t branch) { return m_pChild[branch]; }
    Node *&getChildRef(size_t branch) { return m_pChild[branch]; }
    Node *getParent() { return m_pParent; }
};

template <typename Container>
class binary_tree_iterator // : public general_iterator<Container, binary_tree_iterator<Container> >
{
public:
    // using Parent    = typename Container::Node; // class general_iterator<Container, binary_tree_iterator<Container> >;
    // using Node      = typename Container::Node;
    using Node = typename Container::Node;
    using value_type = typename Container::value_type;
    // using Container = binary_tree_iterator<Container>; // shahows template parameter
private:
    Container *m_pTree;
    Node *m_pNode;

public:
    binary_tree_iterator(Container *pTree = nullptr, Node *pNode = nullptr)
        : m_pTree(pTree), m_pNode(pNode) {}

    //   public:
    //     binary_tree_iterator(Container *pContainer, Node *pNode) : Parent (pContainer,pNode) {}
    //     binary_tree_iterator(Container &other)  : Parent (other) {}
    //     binary_tree_iterator(Container &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    value_type &operator*()
    {
        assert(m_pNode != nullptr);
        return m_pNode->getDataRef();
    }

    // todo: Revisar el avance de un iterator
    binary_tree_iterator &operator++()
    {
        if (!m_pNode)
            return *this;

        if (m_pNode->getChild(1))
            m_pNode = m_pTree->getExtremeNode(m_pNode->getChild(1), 0);
        else
        {
            Node *pChild = m_pNode;
            m_pNode = m_pNode->getParent();
            while (m_pNode && m_pNode->getChild(1) == pChild)
            {
                pChild = m_pNode;
                m_pNode = m_pNode->getParent();
            }
        }
        return *this;
    }

    bool operator==(const binary_tree_iterator &other) const
    {
        return m_pNode == other.m_pNode;
    }

    bool operator!=(const binary_tree_iterator &other) const
    {
        return !(*this == other);
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
class CBinaryTree
{
public:
    using value_type = typename Traits::value_type;
    using Node = CBinaryTreeNode<Traits>; // typename Traits::Node;

    using CompareFn = typename Traits::CompareFn;
    using Container = CBinaryTree<Traits>;
    using iterator = binary_tree_iterator<Container>;

protected:
    Node *m_pRoot = nullptr;
    size_t m_size = 0;
    CompareFn Compfn;

public:
    size_t size() const { return m_size; }
    bool empty() const { return size() == 0; }

    void insert(value_type elem, Ref ref)
    {
        // m_pRoot = internal_insert(elem, ref, nullptr, m_pRoot);
        internal_insert(elem, ref, nullptr, m_pRoot);
    }

    Node *getExtremeNode(Node *startNode, int direction) const
    {
        if (!startNode)
            return nullptr;

        Node *pNode = startNode;
        while (pNode->getChild(direction))
        {
            pNode = pNode->getChild(direction);
        }
        return pNode;
    }

protected:
    Node *CreateNode(Node *pParent, value_type elem, Ref ref)
    {
        return new Node(pParent, elem, ref);
    }
    virtual void internal_insert(value_type &elem, Ref ref,
                                 Node *pParent, Node *&rpOrigin)
    {
        if (!rpOrigin)
        {
            ++m_size;
            // return (rpOrigin = CreateNode(pParent, elem, ref));
            rpOrigin = CreateNode(pParent, elem, ref);
            return;
        }

        size_t branch = Compfn(elem, rpOrigin->getDataRef()) ? 0 : 1;
        // Node *pNode = internal_insert(elem, ref, rpOrigin, rpOrigin->getChildRef(branch));
        // return pNode;
        internal_insert(elem, ref, rpOrigin, rpOrigin->getChildRef(branch));
    }

private:
    void destroy(Node *pNode)
    { // Para eliminar recursivamente los nodos
        if (pNode)
        {
            destroy(pNode->getChild(0));
            destroy(pNode->getChild(1));
            delete pNode;
        }
    }

    Node *copyNodes(Node *pOtherNode, Node *pParent)
    {
        if (!pOtherNode)
            return nullptr;
        Node *pNewNode = CreateNode(pParent, pOtherNode->getData(), pOtherNode->m_ref);
        pNewNode->setpChild(copyNodes(pOtherNode->getChild(0), pNewNode), 0);
        pNewNode->setpChild(copyNodes(pOtherNode->getChild(1), pNewNode), 1);
        return pNewNode;
    }

    template <typename Function, typename... Args>
    void internal_inorder(Node *pNode, Function &func, Args &...args)
    {
        if (pNode)
        {
            internal_inorder(pNode->getChild(0), func, args...);
            func(pNode->getDataRef(), args...);
            internal_inorder(pNode->getChild(1), func, args...);
        }
    }

    // template <typename Func>
    // void internal_inorder(Node *pNode, Func visit)
    // {
    //     if (pNode)
    //     {
    //         internal_inorder(pNode->getChild(0), visit);
    //         visit(pNode->getDataRef());
    //         internal_inorder(pNode->getChild(1), visit);
    //     }
    // }

public:
    CBinaryTree() {} // Empty tree

    // todo: Copy Constructor. We have duplicate each node
    CBinaryTree(const Container &other)
    {
        m_pRoot = copyNodes(other.m_pRoot, nullptr);
        m_size = other.m_size;
        Compfn = other.Compfn;
    }

    // todo: Done: Move Constructor
    CBinaryTree(Container &&other) // Cambio por Binary
        : m_pRoot(std::exchange(other.m_pRoot, nullptr)),
          m_size(std::exchange(other.m_size, 0)),
          Compfn(std::move(other.Compfn)) // se cambio por exchange
    {
    }

    // todo: Recursivo y seguro. Destruir Nodes recursivamente
    virtual ~CBinaryTree() { destroy(m_pRoot); }

    // todo: begin dede comenzar el el nodo mas a la izquierda (0)
    iterator begin()
    {
        if (!m_pRoot)
            return end();
        return iterator(this, getExtremeNode(m_pRoot, 0));
    }
    iterator end() { return iterator(this, nullptr); }

    // TODO: begin debe comenzar el el nodo mas a la derecha (1)
    // riterator rbegin(){
    //     if (!m_pRoot) return rend();
    //     return iterator(this, getExtremeNode(m_pRoot, 1));
    //  }
    // riterator rend()  { return iterator(this, nullptr); }

    // TODO: Generalizar estos recorridos para recibir cualquier funcion
    // con una cantidad flexible de parametros con variadic templates
    // Google: C++ parameter packs cplusplus
    template <typename Function, typename... Args>
    void inorder(Function func, Args &...args) {
        internal_inorder(m_pRoot, func, args...);
    }

    // Para poder usar bt.inorder(std::cout)
    void inorder(std::ostream &os){
        inorder([&os](value_type &data){    os << " --> " << data;  });
    }
    // todo:
    // void inorder(Node *pNode, size_t level, std::ostream &os)
    // {
    //     if (pNode)
    //     {
    //         // Node *pParent = pNode->getParent();
    //         inorder(pNode->getChild(0), level + 1, os);
    //         os << " --> " << pNode->getDataRef();
    //         inorder(pNode->getChild(1), level + 1, os);
    //     }
    // }

    // todo: Generalize this function by using iterators and apply any function
    template <typename Function, typename... Args>
    void foreachInorder(Function func, Args&... args) {
        for(auto it = begin(); it != end(); ++it)
            func(*it, args...);       
    }
    // template <typename Func>
    // void inorder(Func visit) { internal_inorder(m_pRoot, visit); }
    // void inorder(Node *pNode, void (*visit)(value_type &item))
    // {
    //     if (pNode)
    //     {
    //         inorder(pNode->getChild(0), *visit);
    //         (*visit)(pNode->getDataRef());
    //         inorder(pNode->getChild(1), *visit);
    //     }
    // }

    // Variadic templates (See foreach.h)
    template <typename Function, typename... Args>
    void postorder(Function func, Args const &...args)
    {
        postorder(m_pRoot, 0, func, args...);
    }

    template <typename Function, typename... Args>
    void postorder(Node *pNode, size_t level,
                   Function func, Args const &...args)
    {
        if (pNode)
        {
            postorder(pNode->getChild(0), level + 1, func, args...);
            postorder(pNode->getChild(1), level + 1, func, args...);
            func(pNode, level);
        }
    }
    // TODO: generalize this function to apply any function
    void postorder(Node *pNode, size_t level, std::ostream &os)
    {
        if (pNode)
        {
            postorder(pNode->getChild(0), level + 1, os);
            postorder(pNode->getChild(1), level + 1, os);
            os << " --> " << pNode->getDataRef();
        }
    }

    // TODO: Generalize this function to apply any function
    void preorder(std::ostream &os) { preorder(m_pRoot, 0, os); }
    // TODO: Generalize this function to apply any function
    void preorder(Node *pNode, size_t level, std::ostream &os)
    {
        if (pNode)
        {
            os << " --> " << pNode->getDataRef();
            preorder(pNode->getChild(0), level + 1, os);
            preorder(pNode->getChild(1), level + 1, os);
        }
    }

    void print(std::ostream &os) { print(m_pRoot, 0, os); }
    // TODO: generalize this function to apply any function
    // Google: C++ parameter packs cplusplus
    void print(Node *pNode, size_t level, std::ostream &os)
    {
        if (pNode)
        {
            Node *pParent = pNode->getParent();
            print(pNode->getChild(1), level + 1, os);
            os << std::string(" | ") << level << pNode->getDataRef() << "(" << (pParent ? to_string(pParent->getData()) : "Root") << ")" << std::endl; // cambio << por *
            print(pNode->getChild(0), level + 1, os);
        }
    }

    // TODO: Tip: recorrer el arbol en preorden
    void Write(std::ostream &os) { os << *this; }

    // TODO: Leer en el arbol desde un stream asumiendo que esta en preorden
    void Read(std::istream &is) { /* TODO */ }
};

// TODO: este operator << debe seguir estando fuera de la clase
template <typename Traits>
std::ostream &operator<<(std::ostream &os, CBinaryTree<Traits> &obj)
{
    os << "CBinaryTree with " << obj.size() << " elements.";
    obj.inorder(os);
    return os;
}

template <typename Traits>
std::istream &operator>>(std::istream &is, CBinaryTree<Traits> &obj)
{
    // Leer el arbol
    return is;
}

void DemoBinaryTree();

#endif // __BINARY_TREE_H__s