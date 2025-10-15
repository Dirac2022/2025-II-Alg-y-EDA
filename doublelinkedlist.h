#ifndef __DOUBLE_LINKEDLIST_H__
#define __DOUBLE_LINKEDLIST_H__
#include <iostream>
#include <mutex>
#include <utility>
#include "types.h"
#include "traits.h"

template <typename Traits>
class CDoubleLinkedList;

template <typename Traits>
std::ostream &operator<<(std::ostream &os, CDoubleLinkedList<Traits> &obj);

template <typename Traits>
class DLLNode{
private:
    using    value_type = typename Traits::value_type;
    using    Node       = DLLNode<Traits>;

    // Fields go here
    value_type          m_data;
    Ref                 m_ref;
    Node               *m_pNext = nullptr;
    Node               *m_pPrev = nullptr;

public:
    DLLNode(value_type &elem, Ref ref, Node *pNext = nullptr)
        : m_data(elem), m_ref(ref), m_pNext(pNext){
    }
    value_type   GetData()    { return m_data;     }
    value_type  &GetDataRef() { return m_data;     }
    Ref    GetRef()     { return m_ref;      }
    Node * GetNext()    { return m_pNext;    }
    Node *&GetNextRef() { return m_pNext;    }
    // Diff
    void   SetNext(Node *pNext){    m_pNext = pNext; }

    // Particular para la double LinkedList
    Node * GetPrev()    { return m_pPrev;    }
    Node *&GetPrevRef() { return m_pPrev;    }
    // Diff
    void   SetPrev(Node *pPrev){    m_pPrev = pPrev; }
};

// 
// todo Activar el forward_iterator
template <typename Container>
class forward_double_linkedlist_iterator{
 private:
     using value_type = typename Container::value_type;
     using Node       = typename Container::Node;
     // Diff
     using iterator   = forward_double_linkedlist_iterator<Container>;

     Container *m_pList = nullptr;
     Node      *m_pNode = nullptr;
 public:
     forward_double_linkedlist_iterator(Container *pList, Node *pNode)
             : m_pList(pList), m_pNode(pNode){}
     forward_double_linkedlist_iterator(iterator &other)
             : m_pList(other.m_pList), m_pNode(other.m_pNode){}   
     bool operator==(iterator other){ return m_pList == other.m_pList && m_pNode == other.m_pNode; }
     bool operator!=(iterator other){ return !(*this == other);    }

     // Diff
     iterator operator++(){ 
         if(m_pNode)
             m_pNode = m_pNode->GetNext();
         return *this;
     }
     value_type &operator*(){    return m_pNode->GetDataRef();   }
};

template <typename Container>
class backward_double_linkedlist_iterator{
 private:
     using value_type = typename Container::value_type;
     using Node       = typename Container::Node;
     // Diff
     using iterator   = backward_double_linkedlist_iterator<Container>;

     Container *m_pList = nullptr;
     Node      *m_pNode = nullptr;
 public:
     backward_double_linkedlist_iterator(Container *pList, Node *pNode)
             : m_pList(pList), m_pNode(pNode){}
     backward_double_linkedlist_iterator(iterator &other)
             : m_pList(other.m_pList), m_pNode(other.m_pNode){}   
     bool operator==(iterator other){ return m_pList == other.m_pList && 
                                             m_pNode == other.m_pNode;
                                    }
     bool operator!=(iterator other){ return !(*this == other);    }

     // Diff
     iterator operator++(){ 
         if(m_pNode)
             m_pNode = m_pNode->GetPrev();
         return *this;
     }
     value_type &operator*(){    return m_pNode->GetDataRef();   }
};

// TODO Agregar control de concurrencia

template <typename Traits>
class CDoubleLinkedList{
public:
    using value_type = typename Traits::value_type; 
    using Func       = typename Traits::Func;
    using Node       = DLLNode<Traits>; 
    using Container  = CDoubleLinkedList<Traits>;
    using forward_iterator   = forward_double_linkedlist_iterator<Container>;
    using backward_iterator  = backward_double_linkedlist_iterator<Container>;
    
private:
    Node   *m_pRoot = nullptr;
    Node   *m_pTail = nullptr;
    size_t m_nElem = 0;
    Func   m_fCompare;

    mutable std::mutex m_mutex;

public:
    // Constructor
    CDoubleLinkedList();
    CDoubleLinkedList(const CDoubleLinkedList &other);

    // TODO: Done Move Constructor
    CDoubleLinkedList(CDoubleLinkedList &&other);

    // Destructor seguro
    virtual ~CDoubleLinkedList();

    void Insert(value_type &elem, Ref ref);
    bool operator==(const Container& other) const 
    {   
        std::scoped_lock lock(m_mutex, other.m_mutex);
        return m_pRoot == other.m_pRoot 
            && m_pTail == other.m_pTail 
            && m_nElem == other.m_nElem;
    }
    bool operator!=(const Container& other) const {  return !(*this == other);  }
    friend std::ostream &operator<< <>(std::ostream &os, CDoubleLinkedList<Traits> &obj);
private:
    void InternalInsert(Node *&rParent, Node *pRev, value_type &elem, Ref ref);
    void InternalInsertTail(value_type& elem, Ref ref);
    Node *GetRoot()    {    return m_pRoot;     };

public:
    forward_iterator begin(){ return forward_iterator(this, m_pRoot); };
    forward_iterator end()  { return forward_iterator(this, nullptr); } 

    // todo: Done verifricar donde debe comenzar apuntando el iterator reverso
    backward_iterator rbegin(){ return backward_iterator(this, m_pTail); };
    backward_iterator rend()  { return backward_iterator(this, nullptr); } 


public:
    // Persistence
    std::ostream &Write(std::ostream &os) { return os << *this; }
    
    // TODO: Read (istream &is)
    std::istream &Read (std::istream &is);

    // TODO: crear foreach generico aplicando una funcion a cada elemento
    // template <typename Function, typename... Args>
    // void foreach(Function func, Args const&... args){
    //     ::foreach(begin(), end(), func, args...);
    //     // auto iter = begin();
    //     // for(; iter != end() ; ++iter )
    //     //     std::invoke(func, *iter, args...);
    // }
};

template <typename Traits>
void CDoubleLinkedList<Traits>::Insert(value_type &elem, Ref ref){
    std::lock_guard<std::mutex> lock(m_mutex);
    InternalInsert(m_pRoot, nullptr, elem, ref);
}


// todo: Done Agregar el enlace para el Prev()
template <typename Traits>
void CDoubleLinkedList<Traits>::InternalInsert(Node *&rParent, Node *pPrev, value_type &elem, Ref ref){
    if( !rParent || m_fCompare(elem, rParent->GetDataRef()) ){
        Node *pNew = rParent = new Node(elem, ref, rParent);
        if( !pNew->GetNext() ) // Final de la lista
            m_pTail = pNew;
        pNew->SetPrev(pPrev);
        // Puente hacia atras
        Node *pNext = pNew->GetNext();
        if( pNext ){ // Hay algo a continuacion
            // pNew ->SetPrev( pNext->GetPrev() );
            pNext->SetPrev( pNew ); 
        }
        m_nElem++;
        return;
    }
    // Tail recursion
    InternalInsert(rParent->GetNextRef(), rParent, elem, ref);
}

template <typename Traits>
void CDoubleLinkedList<Traits>:: InternalInsertTail(value_type& elem, Ref ref) {
    Node* pnewTail = new Node(elem, ref, nullptr);
    if(!m_pTail)
        m_pRoot = m_pTail = pnewTail;
    else {
        pnewTail->SetPrev(m_pTail);
        m_pTail->SetNext(pnewTail);
        m_pTail = pnewTail;
    }

    ++m_nElem;
    
}


template <typename Traits>
CDoubleLinkedList<Traits>::CDoubleLinkedList(){}

// todo Constructor por copia
//      Hacer loop copiando cada elemento
template <typename Traits>
CDoubleLinkedList<Traits>::CDoubleLinkedList(const CDoubleLinkedList &other)
{
    std::lock_guard<std::mutex> lock(other.m_mutex);
    Node* pCurrent = other.m_pRoot;
    while(pCurrent) {
        value_type elem = pCurrent->GetData();
        Ref ref         = pCurrent->GetRef();
        InternalInsertTail(elem, ref);
        pCurrent = pCurrent->GetNextRef();
    }
}

// Move Constructor
template <typename Traits>
CDoubleLinkedList<Traits>::CDoubleLinkedList(CDoubleLinkedList &&other) noexcept
{
    if (this == &other )    return;
    std::lock_guard<std::mutex> lock(other.m_mutex);
    m_pRoot    = std::move(other.m_pRoot);
    m_pTail    = std::move(other.m_pTail)
    m_nElem    = std::move(other.m_nElem);
    m_fCompare = std::move(other.m_fCompare);

    other.m_pRoot = nullptr;
    other.m_pTail = nullptr;
    other.m_nElem = 0;
}

// TODO: Implementar y liberar la memoria de cada Node
template <typename Traits>
CDoubleLinkedList<Traits>::~CDoubleLinkedList()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    Node* pCurrent = m_pRoot;
    while (pCurrent) {
        Node* pNext = pCurrent->GetNext();
        delete pCurrent;
        pCurrent = pNext;
    }
    m_pRoot = m_pTail = nullptr;
    m_nElem = 0;
}

// todo: Este operador debe quedar fuera de la clase
template <typename Traits>
std::ostream &operator<<(std::ostream &os, CDoubleLinkedList<Traits> &obj){
    std::lock_guard<std::mutex> lock(obj.m_mutex);
    auto pRoot = obj.m_pRoot;
    while( pRoot ) {
        os << pRoot->GetData() << " ";
        pRoot = pRoot->GetNext();
    }  
    return os;
}

void DemoDoubleLinkedList();

#endif // __DOUBLE_LINKEDLIST_H__