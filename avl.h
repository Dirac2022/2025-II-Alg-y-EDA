#ifndef __AVL_H__
#define __AVL_H__

#include "binarytree.h"

template <typename Traits>
class CAVLTree;

template <typename Traits>
class CAVLNode : public CBinaryTreeNode<Traits>{
public:
  using value_type = typename Traits::T;
  using Node       = CBinaryTreeNode<Traits>;
protected:
    int     m_balanceFactor = 0; // Balance factor for AVL tree
public:
    friend class CAVLTree<Traits>;
};

// template <typename _T>
// struct AVLAscTraits{
//     using  value_type = _T;
//     using  Node       = CAVLNode<T>;
//     using  CompareFn  = less<T>;
// };

// template <typename _T>
// struct AVLDescTraits{
//     using  value_type = _T;
//     using  Node       = CAVLNode<T>;
//     using  CompareFn  = greater<T>;
// };

template <typename Traits>
class CAVLTree : public CBinaryTree<Traits> {
public:
    using Base       = CBinaryTree<Traits>;
    using Node       = CAVLNode<Traits>;
    using value_type = typename Traits::value_type;  
    using CompareFn  = typename Traits::CompareFn;
    using Container  = CAVLTree<Traits>;
    using iterator   = binary_tree_iterator<Container>;
    using CBinaryTree<Traits>::internal_insert;
    using CBinaryTree<Traits>::insert;

protected:
    // Additional members for AVL tree balancing can be added here
    // Node *internal_insert(value_type &elem, Ref ref,
    //                       Node* pParent, Node*& rpOrigin)
    // {
    //     // Call base class insert
    //     Node* newNode = Base::internal_insert(elem, ref, pParent, rpOrigin);
    //     // TODO: Verificar balance y realizar rotaciones si es necesario
        
    //     return newNode;
    // }
public:
    CAVLTree() : Base() {} // Empty tree

    void insert(value_type elem, Ref ref) override
    {
        // m_pRoot = internal_insert(elem, ref, nullptr, m_pRoot);
        bool height_grew = false;
        internal_insert(elem, ref, nullptr, this->m_pRoot, height_grew);
    }

    void rotateRight(Node*& pNode) {
        Node* pChild = pNode->getChild(0); // hijo izquierdo (pChild) pasara a ser padre de pNode
        pChild->m_pParent = pNode->m_pParent; // padre de pChild sera ahora padre de pNode
        pNode->setPChild(pChild->getChild(1), 0); // hijo iz de pNode ahora sera sera el hijo derecho de pChild
        if (pNode->getChild(0)) { // Si habia hijo derecho de pchild actualiza su padre, ahora pNode
            pNode->getChild(0)->m_pParent = pNode;
        }
        pChild->setpChild(pNode, 1); // pNode ahora hijo iz de pChild (antiguo hijo iz ahora hijo de de pNode)
        pNode->m_pParent = pChild; 
        pNode = pChild; // pParent de pNode ahora apunta a un difente hijo (pChild)
    }

    void rorateLeft(Node*& pNode) {
        Node* pChild = pNode->getChild(1); // hijo der (pChild) pasara a ser padre de pNode
        pChild->m_pParent = pNode->m_pParent; // padre de pChild sera ahora padre de pNode
        pNode->setPChild(pChild->getChild(0), 1); // hijo der de pNode ahora sera sera el hijo iz de pChild
        if (pNode->getChild(1)) { // Si habia hijo izq de pchild actualiza su padre, ahora pNode
            pNode->getChild(1)->m_pParent = pNode;
        }
        pChild->setpChild(pNode, 0);
        pNode->m_pParent = pChild;
        pNode = pChild;
    }

    void balance(Node*& pNode) {
        if (pNode->m_balanceFactor > 1) {
            Node* pChild = pNode->getChild(1);
            if (pChild->m_balanceFactor < 0) // RL
                rotateRight(pNode->getChildRef(1));
            rotateLeft(pNode); // RR
        }
        else if (pNode->m_balanceFactor < -1) {
            Node* pChild = pNode->getChild(0);
            if (pChild->m_balanceFactor > 0) // LR
                rotateLeft(pNode->getChildRef(0));
            rotateRight(pNode); // LL
        }

        pNode->m_balanceFactor = 0;
        if (pNode->getChild(0))
            pNode->getChild(0)->m_balaceFactor = 0;
        if (pNode->getChiild(1))
            pNode->getChild(1)->m_balanceFactor = 0;
    }

    void internal_insert(value_type& elem, Ref ref, Node* pParent, 
                         Node*& rpOrigin, bool& height_grew)
    {
        if (!rpOrigin) {
            rpOrigin = CreateNode(pParent, elem, ref);  
            this->m_size ++;
            height_grew = true;
            return;
        }

        size_t branch = this->Compfn(elem, rpOrigin->getDataRef()) ? 0 : 1;
        internal_insert(elem, ref, rpOrigin, rpOrigin->getChildRef(branch), height_grew);

        if (height_grew) {
            rpOrigin->m_balanceFactor += (branch == 0 ? -1 : 1);
            if (rpOrigin->m_balanceFactor == 0) 
                height_grew = false;
            else if (abs(rpOrigin->m_balanceFactor) > 1) {
                balance(rpOrigin);
                height_grew = false;
            }
        }
    }
};

void testAVLTree();

#endif // __AVL_H__