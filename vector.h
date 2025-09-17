#ifndef __VECTOR_H__
#define __VECTOR_H__
#include <algorithm>

// TODO (Nivel 2): Agregar Iterators (forward, backward)
template <typename Container>
class vector_forward_iterator{
private:
    using value_type = typename Container::value_type;
    using myself     = vector_forward_iterator<Container>;

    value_type *m_pValue = nullptr;
public:
    vector_forward_iterator(Container &container, value_type &pValue);
    vector_forward_iterator(myself &other);

    value_type& operator*();
    myself      operator++();
    myself     &operator=(myself &other);
    bool        operator!=(vector_forward_iterator<Container> &other);
};

// PC1: deben hacer:
//      2 problemas de nivel 2
//      3 problemas de nivel 1
// Cada solucion enviarla como un Pull request

// TODO (Nivel 2): Agregar Traits

// TODO (Nivel 1): Agregar Documentacion para generar con doxygen

// TODO  (Nivel 2): Agregar control de concurrencia en todo el vector
template <typename T>
class CVector{
   
    using  value_type = T;
    value_type   *m_pVect = nullptr;
    using myself = CVector<value_type>;
    using iterator = vector_forward_iterator<myself>;

    size_t  m_count = 0; // How many elements we have now?
    size_t  m_max   = 0; // Max capacity
public:
    // TODO (Done) (Nivel 1) Agregar un constructor por copia
    CVector(CVector &v);

    CVector(size_t n);
    // TODO  (Nivel 2): Agregar un move constructor
    CVector(CVector &&v);

    // TODO: (Done) (Nivel 1) implementar el destructor de forma segura
    virtual ~CVector();
    void insert(value_type &elem);
    value_type&   operator[](size_t index);
    size_t size() const { return m_count; }
private:
    void resize();
    void Init(size_t n);
    void Destroy();
    iterator begin();
    iterator end();
};

template <typename T>
CVector<T>::CVector(size_t n){
    Init(n);
}

template <typename T>
CVector<T>::CVector(CVector &v) 
          : m_max(v.m_max), 
            m_count(v.m_count) {
    if (m_max > 0)
        m_pVect = new value_type[m_max];
    for (size_t i = 0; i < m_count; ++i)
        m_pVect[i] = v[i];       
}

// TODO (Nivel 1): hacer dinamico el delta de crecimiento
template <typename T>
void CVector<T>::resize(){
    value_type *pTmp = new value_type[m_max+10];
    for(auto i=0; i < m_max ; ++i)
        pTmp[i] = m_pVect[i];
    delete [] m_pVect;
    m_max += 10;
    m_pVect = pTmp;
}

template <typename T>
void CVector<T>::Init(size_t n){
    Destroy();
    resize();
}

template <typename T>
CVector<T>::~CVector(){
    Destroy();
}

template <typename T>
void CVector<T>::Destroy(){
    m_count = 0; 
    m_max   = 0;
    delete [] m_pVect;
    m_pVect = nullptr;
}

// TODO (Done): la funcion insert debe permitir que el vector crezca si ha desbordado
template <typename T>
void CVector<T>::insert(value_type &elem){
    if(m_count == m_max)
        resize();
    m_pVect[m_count++] = elem;
}

template <typename T>
value_type& CVector<T>::operator[](size_t index) {
    if (index >= m_max) {
        std::cout << "index: " << index << " m_count: " << m_count << std::endl;
        throw std::out_of_range("Index out of range");
    }
    m_count = std::max(index+1, m_count);
    return m_pVect[index];
}

template <typename T>
std::ostream& operator<<(std::ostream& os, CVector<T>& vec) {
    // os << "[";
    for (size_t i = 0; i < vec.size(); ++i)
        os << vec[i] << " ";
    // os << "]";
    return os;
}

#endif // __VECTOR_H__